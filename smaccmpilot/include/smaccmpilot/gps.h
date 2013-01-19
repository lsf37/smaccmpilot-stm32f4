
#ifndef __SMACCMPILOT_GPS_H__
#define __SMACCMPILOT_GPS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// This will need more fields for satellite info, DOP, etc.  Currently
// this contains only the fields sent in the MAVLink HIL_STATE
// message.
struct gps_result {
  bool valid;
  uint32_t time_ms;
  /* latitude and longitude (*1E7) */
  int32_t lat;
  int32_t lon;
  /* altitude in m*1000 */
  int32_t alt;
  /* ground speed in m/s*100 */
  int16_t vx;
  int16_t vy;
  int16_t vz;
};

#ifdef __cplusplus
}
#endif

#endif   // !defined __SMACCMPILOT_GPS_H__
