#include <string.h>
#include <smaccmpilot/horizontal_controller.h>
#include <smaccmpilot/pid_stabilize.h>

#define MAX_TRAVERSE_RATE  0.75f

#define pitch_roll_P_gain 1.0f
#define pitch_roll_I_gain 0.0f // 0.05f
#define pitch_roll_I_lim  10.0f

static struct PID g_pid_pitch = {
    pitch_roll_P_gain, // p_gain
    pitch_roll_I_gain, // i_gain
    0.0f,              // i_state
    -pitch_roll_I_lim, // i_min
    pitch_roll_I_lim,  // i_max
    0.0f,              // d_state
    1,                 // reset
};

static struct PID g_pid_roll = {
    pitch_roll_P_gain, // p_gain
    pitch_roll_I_gain, // i_gain
    0.0f,              // i_state
    -pitch_roll_I_lim, // i_min
    pitch_roll_I_lim,  // i_max
    0.0f,              // d_state
    1,                 // reset
};

/* ------------------------------------------------------------------------ */

// Convert a normalized stick value [-1.0, 1.0] to a traverse
// rate in m/sec. Could add a dead zone here if we wanted to.
static float stick_to_velocity(float stick) {
    return stick * MAX_TRAVERSE_RATE;
}

/* ------------------------------------------------------------------------ */

static float controller( struct PID *pid,
                         const struct position_estimate *pos_est,
                         float user_stick_input,
                         float velocity )
{
    if (pos_est->horiz_conf > 9) {
        float desired_rate = stick_to_velocity(user_stick_input);
        float actual_rate  = velocity;
        float error        = desired_rate - actual_rate;
        float result       = pid_update(pid, error, actual_rate);
        return result;
    } else {
        /* hack - should be a method to trigger reset,
         * and reset should also clear iState. */
        pid->pid_reset = 1;
        pid->pid_iState = 0.0f;
        return user_stick_input;
   }
}

void horizontal_compensate(const struct position_estimate *pos,
                         const struct sensors_result *sensors,
                         const struct userinput_result *userin,
                         struct userinput_result *out)
{
    if (userin->mode == 2) { /* loiter */
        /* pass through userinput to output */
        memcpy(out, userin, sizeof(struct userinput_result));
        /* modify only the pitch and roll */
        out->roll  = controller(&g_pid_roll,  pos, userin->roll,  -1*pos->vx);
        out->pitch = controller(&g_pid_pitch, pos, userin->pitch, -1*pos->vy);
    } else {
        /* pass through user input to output */
        memcpy(out, userin, sizeof(struct userinput_result));
    }
}

