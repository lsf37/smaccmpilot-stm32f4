// -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*-
/*
 * gcs.h --- GCS communication with MAVLink.
 *
 * Copyright (C) 2012, Galois, Inc.
 * All Rights Reserved.
 *
 * This software is released under the "BSD3" license.  Read the file
 * "LICENSE" for more information.
 */

#ifndef __APP_STABILIZE_HIL_GCS_H__
#define __APP_STABILIZE_HIL_GCS_H__

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations.
struct servo_result;
struct sensors_result;
struct gps_result;

// Initialize the GCS, starting a task for MAVLink.
void gcs_init();

// Read HIL raw sensor data from the GCS.  Returns true if valid data
// was available.
bool gcs_sensors_get(struct sensors_result *sensors);

// Read HIL raw GPS data from the GCS.  Returns true if valid data was
// available.
bool gcs_gps_get(struct gps_result *gps);

// Write servo data to the GCS.
void gcs_servos_set(const struct servo_result *servo);

#ifdef __cplusplus
}
#endif

#endif   // !defined __APP_STABILIZE_HIL_GCS_H__
