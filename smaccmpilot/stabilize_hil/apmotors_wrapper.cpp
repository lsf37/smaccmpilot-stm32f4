
#include "apmotors_wrapper.h"

#include <AP_HAL.h>
#include <RC_Channel.h>
#include <AP_Motors.h>

#include "gcs.h"

/* copying arducopter's minimum throttle of 130. dont really know why? */
#define THROTTLE_MINIMUM 130
#define THROTTLE_MAXIMUM 1000

/* I don't approve of RC_Channels being hooked up to rcin/rcout IO, so
 * we're giving them all bogus channel values so they can do no harm. */
static RC_Channel s_roll     (255);
static RC_Channel s_pitch    (255);
static RC_Channel s_throttle (255);
static RC_Channel s_yaw      (255);

static AP_MotorsQuad motors(&s_roll, &s_pitch, &s_throttle, &s_yaw);

extern const AP_HAL::HAL& hal;

static void angular_channel_setup(RC_Channel* ch) {
    ch->set_angle(4500);
    ch->set_type(RC_CHANNEL_TYPE_ANGLE_RAW);
}

static void throttle_channel_setup(RC_Channel* ch) {
    ch->set_range(THROTTLE_MINIMUM, THROTTLE_MAXIMUM);
    ch->set_range_out(0,1000);
}

void apmotors_output_init(void) {

    angular_channel_setup(&s_roll);
    angular_channel_setup(&s_pitch);
    angular_channel_setup(&s_yaw);
    throttle_channel_setup(&s_throttle);

    motors.set_update_rate(50); /* Is this appropriate with IOAR?
                                     Otherwise? */
    motors.set_frame_orientation(AP_MOTORS_PLUS_FRAME);
    motors.Init();
    motors.set_min_throttle(THROTTLE_MINIMUM);
    motors.set_max_throttle(THROTTLE_MAXIMUM);

    motors.enable();
    motors.output_min();
    motors.auto_armed(true);
}


static int16_t angular_scale(float input) {
    /* Angular channels expect a value from -4500 to +4500
     * input should be from -1.0f to 1.0f */
    if (input < -1.0f) input = -1.0f;
    if (input >  1.0f) input =  1.0f;
    return (int16_t) (4500.0f * input);
}

static int16_t throttle_scale(float input) {
    /* Throttle expects a value from 0 to 1000
     * input should be from 0.0f to 1.0f */ 
    if (input < 0.0f) input = 0.0f;
    if (input > 1.0f) input = 1.0f;
    return (int16_t) (1000.0f * input);
}

void apmotors_output_set(const struct motorsoutput_result *state) {
    struct servos_result servo;

    if (motors.armed() && !(state->armed)) {
        motors.armed(false);
    } else if (!motors.armed() && (state->armed)) {
        motors.armed(true);
    }

    s_roll.servo_out     = angular_scale(state->roll);
    s_pitch.servo_out    = angular_scale(state->pitch);
    s_yaw.servo_out      = angular_scale(state->yaw);

    s_throttle.servo_out = throttle_scale(state->throttle);

    motors.output();

    servo.valid = true;
    servo.servo[0] = motors.motor_out[0];
    servo.servo[1] = motors.motor_out[1];
    servo.servo[2] = motors.motor_out[2];
    servo.servo[3] = motors.motor_out[3];
    gcs_servos_set(&servo);
}