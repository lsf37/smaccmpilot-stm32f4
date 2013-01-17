#ifndef __SMAVLINK_MESSAGE_RC_CHANNELS_OVERRIDE_H__
#define __SMAVLINK_MESSAGE_RC_CHANNELS_OVERRIDE_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <ivory.h>
#include <smavlink/channel.h>
#include <smavlink/system.h>
struct rc_channels_override_msg {
    uint16_t chan1_raw;
    uint16_t chan2_raw;
    uint16_t chan3_raw;
    uint16_t chan4_raw;
    uint16_t chan5_raw;
    uint16_t chan6_raw;
    uint16_t chan7_raw;
    uint16_t chan8_raw;
    uint8_t target_system;
    uint8_t target_component;
};
void smavlink_send_rc_channels_override(struct rc_channels_override_msg* var0,
                                        struct smavlink_out_channel* var1,
                                        struct smavlink_system* var2);

#ifdef __cplusplus
}
#endif
#endif /* __SMAVLINK_MESSAGE_RC_CHANNELS_OVERRIDE_H__ */