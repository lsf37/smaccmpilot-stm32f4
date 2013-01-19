// -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*-
/*
 * stabilize.c --- Simple stabilizer for SMACCMPilot.
 *
 * Copyright (C) 2012, Galois, Inc.
 * All Rights Reserved.
 *
 * This software is released under the "BSD3" license.  Read the file
 * "LICENSE" for more information.
 */

#include <stdbool.h>
#include <stdint.h>

#include <smaccmpilot/userinput.h>
#include <smaccmpilot/sensors.h>
#include <smaccmpilot/stabilize.h>
#include <smaccmpilot/motorsoutput.h>

#include "ivory/pid_stabilize.h"

#define MAX_INPUT_ROLL   45.0f  /* deg */
#define MAX_INPUT_PITCH  45.0f  /* deg */
#define MAX_INPUT_YAW    45.0f  /* deg */

/* These numbers are from ArduPilot.  Do they still make sense? */
#define MAX_OUTPUT_ROLL  50.0f  /* deg/sec */
#define MAX_OUTPUT_PITCH 50.0f  /* deg/sec */
#define MAX_OUTPUT_YAW   45.0f  /* deg/sec */

// These PID gains are tuned experiementally for the "pysim" simulator
// and will surely be wrong for any real copter.  We will need
// something more dynamic than hardcoding these here.

static struct PID g_pi_roll_stabilize = {
    1.3f,                       // p_gain
    0.0f,                       // i_gain
    0.0f,                       // i_state
    -8.0f,                      // i_min
    8.0f,                       // i_max
};

static struct PID g_pi_roll_rate = {
    0.05f,                      // p_gain
    0.015f,                     // i_gain
    0.0f,                       // i_state
    -5.0f,                      // i_min
    5.0f,                       // i_max
};

static struct PID g_pi_pitch_stabilize = {
    1.3f,                       // p_gain
    0.0f,                       // i_gain
    0.0f,                       // i_state
    -8.0f,                      // i_min
    8.0f,                       // i_max
};

static struct PID g_pi_pitch_rate = {
    0.05f,                      // p_gain
    0.015f,                     // i_gain
    0.0f,                       // i_state
    -5.0f,                      // i_min
    5.0f,                       // i_max
};

#if 0                           // not used currently
static struct PID g_pi_yaw_stabilize = {
    1.3f,                       // p_gain
    0.0f,                       // i_gain
    0.0f,                       // i_state
    -8.0f,                      // i_min
    8.0f,                       // i_max
};
#endif

static struct PID g_pi_yaw_rate = {
    0.05f,                      // p_gain
    0.015f,                     // i_gain
    0.0f,                       // i_state
    -8.0f,                      // i_min
    8.0f,                       // i_max
};

static struct PID g_pi_throttle_climb_rate = {
    0.5f,                       // p_gain
    0.0f,                       // i_gain
    0.0f,                       // i_state
    -10.0f,                     // i_min
    10.0f,                      // i_max
};

#define MAX_CLIMB_RATE  2.0f

// Convert a normalized throttle stick value [0.0, 1.0] to a climb
// rate in m/sec.  This uses a wide dead zone to maintain a constant
// altitude.
float throttle_to_climb_rate(float stick)
{
    const float dead_zone = 0.4f;
    const float cutoff    = 0.9f;

    stick = (stick * 2.0f) - 1.0f; // convert to [-1.0, 1.0]

    if (stick >= cutoff) {
        return MAX_CLIMB_RATE;
    } else if (stick <= -cutoff) {
        return 0.0f;
    } else if (stick >= dead_zone) {
        stick = (stick - dead_zone) * (1.0f / (1.0f - dead_zone));
        return stick * MAX_CLIMB_RATE;
    } else if (stick <= -dead_zone) {
        stick = (stick + dead_zone) * (1.0f / (1.0f - dead_zone));
        return stick * MAX_CLIMB_RATE;
    } else {
        return 0.0f;
    }
}

float constrain(float x, float min, float max)
{
    return (x < min ? min : (x > max ? max : x));
}

float degrees(float rad)
{
    return rad * 57.295779513082320876798154814105;
}

float g_throttle_cruise = 0.3f;
float g_throttle_avg    = 0.0f;
static const float g_throttle_min = 0.2f;

static void update_throttle_cruise(float throttle,
                                   const struct sensors_result *sensors)
{
    if (g_throttle_avg == 0.0f) {
        g_throttle_avg = g_throttle_cruise;
    }

    // Calculate average throttle if we are in a level hover.
    if (throttle > g_throttle_min && fabsf(sensors->climb_rate) < 0.1 &&
        fabsf(degrees(sensors->roll)) < 5.0f &&
        fabsf(degrees(sensors->pitch)) < 5.0f) {
        g_throttle_avg = g_throttle_avg * 0.99f + throttle * 0.01f;
        g_throttle_cruise = g_throttle_avg;
    }
}

float stabilize_throttle_climb_rate(const struct userinput_result *in,
                                    const struct sensors_result *sensors)
{
    float desired_rate = throttle_to_climb_rate(in->throttle);
    float actual_rate  = sensors->climb_rate;
    float error        = desired_rate - actual_rate;
    float result       = pid_update(&g_pi_throttle_climb_rate, error);

    if (desired_rate < 0.1f && desired_rate >= -0.1f) {
        update_throttle_cruise(in->throttle, sensors);
    }

    return g_throttle_cruise + result;
}

void stabilize_motors(const struct userinput_result *in,
                      const struct sensors_result *sensors,
                      struct motorsoutput_result *out)
{
    out->roll  = stabilize_from_angle(&g_pi_roll_stabilize, &g_pi_roll_rate,
                                      in->roll, MAX_INPUT_ROLL,
                                      sensors->roll, sensors->omega_x,
                                      MAX_OUTPUT_ROLL);

    out->pitch = stabilize_from_angle(&g_pi_pitch_stabilize, &g_pi_pitch_rate,
                                      in->pitch, MAX_INPUT_PITCH,
                                      sensors->pitch, sensors->omega_y,
                                      MAX_OUTPUT_PITCH);

    out->yaw   = stabilize_from_rate(&g_pi_yaw_rate, in->yaw, MAX_INPUT_YAW,
                                     sensors->omega_z, MAX_OUTPUT_YAW);

    out->throttle = stabilize_throttle_climb_rate(in, sensors);

    out->armed    = in->armed;
    out->time     = in->time;
}
