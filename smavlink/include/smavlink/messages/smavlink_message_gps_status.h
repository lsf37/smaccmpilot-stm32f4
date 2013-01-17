#ifndef __SMAVLINK_MESSAGE_GPS_STATUS_H__
#define __SMAVLINK_MESSAGE_GPS_STATUS_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <ivory.h>
#include <smavlink/channel.h>
#include <smavlink/system.h>
struct gps_status_msg {
    uint8_t satellites_visible;
    uint8_t satellite_prn[20U];
    uint8_t satellite_used[20U];
    uint8_t satellite_elevation[20U];
    uint8_t satellite_azimuth[20U];
    uint8_t satellite_snr[20U];
};
void smavlink_send_gps_status(struct gps_status_msg* var0,
                              struct smavlink_out_channel* var1,
                              struct smavlink_system* var2);

#ifdef __cplusplus
}
#endif
#endif /* __SMAVLINK_MESSAGE_GPS_STATUS_H__ */