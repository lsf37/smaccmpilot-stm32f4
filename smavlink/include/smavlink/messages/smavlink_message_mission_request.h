#ifndef __SMAVLINK_MESSAGE_MISSION_REQUEST_H__
#define __SMAVLINK_MESSAGE_MISSION_REQUEST_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <ivory.h>
#include <smavlink/channel.h>
#include <smavlink/system.h>
struct mission_request_msg {
    uint16_t mission_request_seq;
    uint8_t target_system;
    uint8_t target_component;
};
void smavlink_send_mission_request(struct mission_request_msg* var0,
                                   struct smavlink_out_channel* var1,
                                   struct smavlink_system* var2);

#ifdef __cplusplus
}
#endif
#endif /* __SMAVLINK_MESSAGE_MISSION_REQUEST_H__ */