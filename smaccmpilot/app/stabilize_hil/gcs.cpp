// -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*-
/*
 * gcs.cpp --- GCS communication with MAVLink.
 *
 * Copyright (C) 2012, Galois, Inc.
 * All Rights Reserved.
 *
 * This software is released under the "BSD3" license.  Read the file
 * "LICENSE" for more information.
 */

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <hwf4/usart.h>

#include <AP_HAL_SMACCM.h>
#include <AP_Math.h>

#include <include/mavlink/v1.0/mavlink_types.h>
#include <include/mavlink/v1.0/common/mavlink.h>

#include <smaccmpilot/motorsoutput.h>
#include <smaccmpilot/sensors.h>
#include <smaccmpilot/gps.h>
#include "gcs.h"

extern const AP_HAL::HAL& hal;

#define GCS_UART usart1

static const uint8_t SYS_ID       = 1;
static const uint8_t COMP_ID      = MAV_COMP_ID_IMU;
static const uint8_t SYSTEM_TYPE  = MAV_AUTOPILOT_GENERIC;
static const uint8_t MAV_TYPE     = MAV_TYPE_QUADROTOR;

// For now, just hardcode the system mode and status.
static const uint8_t SYSTEM_MODE  = MAV_MODE_STABILIZE_ARMED;
static const uint8_t CUSTOM_MODE  = 0;
static const uint8_t SYSTEM_STATE = MAV_STATE_ACTIVE;

// Rate (in ms) we send heartbeat messages at.
static const portTickType HEARTBEAT_TICKS = 1000;

// Buffer containing a packed MAVLink message for TX.
static uint8_t g_buf[MAVLINK_MAX_PACKET_LEN];

// Handle to the GCS thread.
static xTaskHandle g_gcs_task;

// Shared sensor state data and mutex protecting it.
static xSemaphoreHandle g_sensors_mutex;
static struct sensors_result g_sensors;

// Shared GPS state data and mutex protecting it.
static xSemaphoreHandle g_gps_mutex;
static struct gps_result g_gps;

// Shared servo state and mutex protecting it.
static xSemaphoreHandle g_servos_mutex;
static struct servo_result g_servos;

static void gcs_set_stream_rate(uint8_t stream, uint8_t enable, uint16_t rate);

//////////////////////////////////////////////////////////////////////
// Shared State

// Read the sensor result.
bool gcs_sensors_get(struct sensors_result *state)
{
    bool result = false;

    if (xSemaphoreTake(g_sensors_mutex, 1)) {
        memcpy(state, &g_sensors, sizeof(g_sensors));
        result = state->valid;
        xSemaphoreGive(g_sensors_mutex);
    } else {
        hal.scheduler->panic("PANIC: gcs_sensors_get took too long "
                             "to acquire mutex");
    }

    return result;
}

// Set the sensor result from inside the GCS thread.
static void gcs_sensors_set(const struct sensors_result *state)
{
    if (xSemaphoreTake(g_sensors_mutex, 1)) {
        memcpy(&g_sensors, state, sizeof(g_sensors));
        xSemaphoreGive(g_sensors_mutex);
    } else {
        hal.scheduler->panic("PANIC: gcs_sensors_set took too long "
                             "to acquire mutex");
    }
}

// Read the raw GPS data.
bool gcs_gps_get(struct gps_result *state)
{
    bool result = false;

    if (xSemaphoreTake(g_gps_mutex, 1)) {
        memcpy(state, &g_gps, sizeof(g_gps));
        result = state->valid;
        xSemaphoreGive(g_gps_mutex);
    } else {
        hal.scheduler->panic("PANIC: gcs_gps_get took too long "
                             "to acquire mutex");
    }

    return result;
}

// Set the sensor result from inside the GCS thread.
static void gcs_gps_set(const struct gps_result *state)
{
    if (xSemaphoreTake(g_gps_mutex, 1)) {
        memcpy(&g_gps, state, sizeof(g_gps));
        xSemaphoreGive(g_gps_mutex);
    } else {
        hal.scheduler->panic("PANIC: gcs_gps_set took too long "
                             "to acquire mutex");
    }
}

// Read the current servo data.
static bool gcs_servos_get(struct servo_result *state)
{
    bool result = false;

    if (xSemaphoreTake(g_servos_mutex, 1)) {
        memcpy(state, &g_servos, sizeof(g_servos));
        result = state->valid;
        xSemaphoreGive(g_servos_mutex);
    } else {
        hal.scheduler->panic("PANIC: gcs_servos_get took too long "
                             "to acquire mutex");
    }

    return result;
}

// Set the servo data to send via MAVLink.
void gcs_servos_set(const struct servo_result *state)
{
    if (xSemaphoreTake(g_servos_mutex, 1)) {
        memcpy(&g_servos, state, sizeof(g_servos));
        xSemaphoreGive(g_servos_mutex);
    } else {
        hal.scheduler->panic("PANIC: gcs_servos_set took too long "
                             "to acquire mutex");
    }
}

//////////////////////////////////////////////////////////////////////
// MAVLink Transmit Functions

// Send a heartbeat message.
static void gcs_send_heartbeat()
{
    mavlink_message_t msg;
    uint16_t len;

    mavlink_msg_heartbeat_pack(
        SYS_ID, COMP_ID, &msg, SYSTEM_TYPE, MAV_TYPE, SYSTEM_MODE,
        CUSTOM_MODE, SYSTEM_STATE);
    len = mavlink_msg_to_send_buffer(g_buf, &msg);
    usart_write(GCS_UART, g_buf, len);
}

// Send an ATTITUDE message from the sensor state.
static void gcs_send_attitude()
{
    struct sensors_result sensors;
    mavlink_message_t msg;
    uint16_t len;

    if (!gcs_sensors_get(&sensors))
        return;

    mavlink_msg_attitude_pack(
        SYS_ID, COMP_ID, &msg,
        hal.scheduler->millis(),
        sensors.roll, sensors.pitch, sensors.yaw,
        sensors.omega_x, sensors.omega_y, sensors.omega_z);
    len = mavlink_msg_to_send_buffer(g_buf, &msg);
    usart_write(GCS_UART, g_buf, len);
}

// Send a GLOBAL_POSITION_INT message from the sensor state.
static void gcs_send_location()
{
    struct gps_result gps;
    mavlink_message_t msg;
    uint16_t len;

    if (!gcs_gps_get(&gps))
        return;

    mavlink_msg_global_position_int_pack(
        SYS_ID, COMP_ID, &msg,
        hal.scheduler->millis(),
        gps.lat, gps.lon, gps.alt,
        gps.alt,                // XXX what is ground?
        gps.vx, gps.vy, gps.vz,
        65535);                 // XXX where do we get heading?
    len = mavlink_msg_to_send_buffer(g_buf, &msg);
    usart_write(GCS_UART, g_buf, len);
}

// Send a GPS_RAW_INT message from the sensor state.
static void gcs_send_gps_raw_int()
{
    struct gps_result gps;
    mavlink_message_t msg;
    uint16_t len;

    if (!gcs_gps_get(&gps))
        return;

    mavlink_msg_gps_raw_int_pack(SYS_ID, COMP_ID, &msg,
                                 hal.scheduler->micros(), 3, // 3D fix
                                 gps.lat, gps.lon, gps.alt,
                                 65535, 65535, 65535, 65535, 255);
    len = mavlink_msg_to_send_buffer(g_buf, &msg);
    usart_write(GCS_UART, g_buf, len);
}

// Send a VFR_HUD message.
static void gcs_send_vfr_hud()
{
    struct sensors_result sensors;
    struct gps_result gps;
    mavlink_message_t msg;
    uint16_t len;

    if (!gcs_sensors_get(&sensors))
        return;
    if (!gcs_gps_get(&gps))
        return;

    mavlink_msg_vfr_hud_pack(
        SYS_ID, COMP_ID, &msg,
        0.0f, 0.0f,             // XXX air and ground speed?
        degrees(sensors.yaw),
        0,                      // XXX throttle
        gps.alt / 1000.0f,
        sensors.climb_rate);
    len = mavlink_msg_to_send_buffer(g_buf, &msg);
    usart_write(GCS_UART, g_buf, len);
}

// Send a SERVO_OUTPUT_RAW message from the servo state.
static void gcs_send_servo_output_raw()
{
    struct servo_result servo;
    mavlink_message_t msg;
    uint16_t len;

    if (!gcs_servos_get(&servo))
        return;

    mavlink_msg_servo_output_raw_pack(SYS_ID, COMP_ID, &msg,
                                      hal.scheduler->micros(), 0,
                                      servo.servo[0], servo.servo[1],
                                      servo.servo[2], servo.servo[3],
                                      0, 0, 0, 0);
    len = mavlink_msg_to_send_buffer(g_buf, &msg);
    usart_write(GCS_UART, g_buf, len);
}

//////////////////////////////////////////////////////////////////////
// MAVLink Receive Handlers

// Handle a HIL_STATE message with HIL sensor data.
static void gcs_handle_hil_state(const mavlink_message_t *msg)
{
    struct sensors_result sensors;
    struct sensors_result prev;
    struct gps_result gps;
    mavlink_hil_state_t m;

    mavlink_msg_hil_state_decode(msg, &m);

    sensors.valid      = true;
    sensors.time_ms    = m.time_usec / 1000;
    sensors.roll       = m.roll;
    sensors.pitch      = m.pitch;
    sensors.yaw        = m.yaw;
    sensors.omega_x    = m.rollspeed;
    sensors.omega_y    = m.pitchspeed;
    sensors.omega_z    = m.yawspeed;
    sensors.baro_alt   = (float)m.alt / 1000.0f;
    sensors.climb_rate = 0.0f;

    gps.valid   = true;
    gps.time_ms = m.time_usec / 1000;
    gps.lat     = m.lat;
    gps.lon     = m.lon;
    gps.alt     = m.alt;
    gps.vx      = m.vx;
    gps.vy      = m.vy;
    gps.vz      = m.vz;

    // Calculate climb rate based on the difference between this HIL
    // state and the current one.
    if (gcs_sensors_get(&prev)) {
        uint32_t dt = sensors.time_ms - prev.time_ms;

        // XXX Filter out bogus data from MAVproxy when it gets
        // confused about HIL_STATE packet timing.  Ouch.
        if (dt != 0 && dt < 25) {
            float dt_sec = ((float)dt / 1000.0f);
            sensors.climb_rate = (sensors.baro_alt - prev.baro_alt) / dt_sec;
            // hal.console->printf("alt %.2f prev %.2f dt %lu dt_sec %0.4f rate %.2f\r\n",
            //                     sensors.baro_alt, prev.baro_alt, dt, dt_sec,
            //                     sensors.climb_rate);
        }
    }

    gcs_sensors_set(&sensors);
    gcs_gps_set(&gps);
}

// Handle a REQUEST_STREAM message.
static void gcs_handle_request_data_stream(const mavlink_message_t *msg)
{
    mavlink_request_data_stream_t m;
    mavlink_msg_request_data_stream_decode(msg, &m);
    gcs_set_stream_rate(m.req_stream_id, m.start_stop,
                        m.req_message_rate);
}

// Handle a received MAVLink message.
static void gcs_handle_message(const mavlink_message_t *msg)
{
    switch (msg->msgid) {
        case MAVLINK_MSG_ID_HIL_STATE:
            gcs_handle_hil_state(msg);
            break;
        case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:
            gcs_handle_request_data_stream(msg);
        default:
            break;
    }
}

//////////////////////////////////////////////////////////////////////
// GCS Task and Timing

// Information about a procedure to run at regular intervals from the
// GCS task.
//
// Set "rate" to a non-zero value to enable the action.  If "stream"
// is non-zero, the rate will be controlled by the MAVLink
// "REQUEST_DATA_STREAM" command.
//
// It is legal for multiple actions to have the same "stream", in
// which case they will be set together.
struct gcs_timed_action {
    void (*func)(void);        // function to run, NULL at end of list
    uint8_t stream;            // stream number if non-zero
    uint16_t rate;             // rate in Hz, 0=disabled
    uint32_t last;             // last time ran (ticks)
};

// Table of actions for "gcs_task" to perform on a regular scheduler,
// such as sending a heartbeat and servo data.
//
// The mapping of streams to messages here is intended to be
// compatible with ArduCopter.
static struct gcs_timed_action g_actions[] = {
    { gcs_send_heartbeat,        0,                               1, 0 },
    { gcs_send_servo_output_raw, MAV_DATA_STREAM_RAW_CONTROLLER,  0, 0 },
    { gcs_send_attitude,         MAV_DATA_STREAM_EXTRA1,          0, 0 },
    { gcs_send_location,         MAV_DATA_STREAM_POSITION,        0, 0 },
    { gcs_send_gps_raw_int,      MAV_DATA_STREAM_EXTENDED_STATUS, 0, 0 },
    { gcs_send_vfr_hud,          MAV_DATA_STREAM_EXTRA2,          0, 0 },
    { NULL,                      0,                               0, 0 },
};

// Enable/disable and set rate for a stream.  If "stream" is zero, all
// actions with a non-zero stream ID are set.
static void gcs_set_stream_rate(uint8_t stream, uint8_t enable, uint16_t rate)
{
    for (int i = 0; g_actions[i].func != NULL; ++i) {
        if ((stream == 0 && g_actions[i].stream != 0) ||
            (stream != 0 && g_actions[i].stream == stream)) {
            if (enable) {
                g_actions[i].rate = rate;
            } else {
                g_actions[i].rate = 0;
            }
        }
    }
}

// Run any actions that are at or past their expiration time.  Returns
// the time in ticks until the next action is due.
static portTickType gcs_run_actions()
{
    portTickType now = xTaskGetTickCount();
    portTickType result = portMAX_DELAY;

    for (int i = 0; g_actions[i].func != NULL; ++i) {
        portTickType rate_ticks;
        portTickType due;

        if (g_actions[i].rate == 0)
            continue;           // skip disabled actions

        rate_ticks = 1000 / g_actions[i].rate;
        due        = g_actions[i].last + rate_ticks;

        if (now >= due) {
            g_actions[i].last = now;
            g_actions[i].func();

            if (due + rate_ticks > now) {
                portTickType dt = (due + rate_ticks) - now;

                if (dt < result) {
                    result = dt;
                }
            } else {
                result = 0;
            }
        } else {
            portTickType dt = due - now;

            if (dt < result)
                result = dt;
        }
    }

    return result;
}

// GCS task that sends and receives MAVLink messages.  Runs any
// actions that are due to run, then waits until we receive a byte on
// the GCS UART or the next action is due.
static void gcs_task(void *arg)
{
    mavlink_message_t msg;
    mavlink_status_t status;
    portTickType delay;
    ssize_t count;
    uint8_t byte;

    for (;;) {
        delay = gcs_run_actions();
        count = usart_read_timeout(GCS_UART, delay, &byte, 1);

        if (count > 0 && mavlink_parse_char(0, byte, &msg, &status)) {
            gcs_handle_message(&msg);
        }
    }
}

// Initialize the GCS, starting the MAVLink task.
void gcs_init()
{
    memset(&g_sensors, 0, sizeof(g_sensors));
    memset(&g_gps,     0, sizeof(g_gps));
    memset(&g_servos,  0, sizeof(g_servos));

    g_sensors_mutex = xSemaphoreCreateMutex();
    g_gps_mutex     = xSemaphoreCreateMutex();
    g_servos_mutex  = xSemaphoreCreateMutex();

    xTaskCreate(gcs_task, (signed char *)"gcs", 512, NULL, 0, &g_gcs_task);
}
