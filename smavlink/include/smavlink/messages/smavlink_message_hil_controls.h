#ifndef __SMAVLINK_MESSAGE_HIL_CONTROLS_H__
#define __SMAVLINK_MESSAGE_HIL_CONTROLS_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <ivory.h>
#include <smavlink/channel.h>
#include <smavlink/system.h>
struct hil_controls_msg {
    uint64_t time_usec;
    float roll_ailerons;
    float pitch_elevator;
    float yaw_rudder;
    float throttle;
    float aux1;
    float aux2;
    float aux3;
    float aux4;
    uint8_t mode;
    uint8_t nav_mode;
};
void smavlink_send_hil_controls(struct hil_controls_msg* var0,
                                struct smavlink_out_channel* var1,
                                struct smavlink_system* var2);

#ifdef __cplusplus
}
#endif
#endif /* __SMAVLINK_MESSAGE_HIL_CONTROLS_H__ */