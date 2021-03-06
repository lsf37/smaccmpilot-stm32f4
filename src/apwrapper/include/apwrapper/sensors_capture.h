
#ifndef __FLIGHT_SUPPORT_SENSORS_CAPTURE_H__
#define __FLIGHT_SUPPORT_SENSORS_CAPTURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

void sensors_begin(bool flipped);
void sensors_update(void);

/* Write roll, pitch, yaw into an array of 3 floats
 * [ roll, pitch, yaw ]
 * units: radians
 */
void sensors_get_rpy(float *rpy, uint32_t *time);

/* Write omega (angular rate) into an array of 3 floats
 * [ omega_x, omega_y, omega_z ]
 * units: radians/sec
 */
void sensors_get_omega(float *omega);

/* Write accel values to an array of 3 floats.
 * [ accel_x, accel_y, accel_z ]
 * units: mg
 */
void sensors_get_accel(float *capt);

/* Get barometric altitude estimate
 * unit: meters
 */
float sensors_get_baro_alt(uint32_t *time);

/* Set velocity measurement from GPS
 * units: velocities in cm/s
 *        heading in degrees
 */
void sensors_set_gps_velocity(int32_t v_north, int32_t v_east, int32_t v_down,
        uint32_t speed_ground, float heading);

/* Set position measurement from GPS, only effects AHRS compensation
 * units: degrees * 10,000,000
 */
void sensors_set_gps_position(int32_t lat, int32_t lon);

/* Set position measurement from GPS, only effects compass declination
 * units: degrees * 10,000,000
 */
void sensors_set_gps_position_for_compass(int32_t lat, int32_t lon);

/* Set satellite fix information from GPS
 */
void sensors_set_gps_fix(bool fix2d, bool fix3d, uint8_t num_sats);

#ifdef __cplusplus
}
#endif

#endif // __FLIGHT_SUPPORT_SENSORS_CAPTURE_H__

