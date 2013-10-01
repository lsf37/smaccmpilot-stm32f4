{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE Rank2Types #-}

{- Module automatically generated by smavgen_ivory.py -}

module SMACCMPilot.Mavlink.Messages
  ( mavlinkMessageModules
  , messageLensCRCs
  ) where

import Data.Word (Word8)
import Ivory.Language

import qualified SMACCMPilot.Mavlink.Messages.Data16
import qualified SMACCMPilot.Mavlink.Messages.Data32
import qualified SMACCMPilot.Mavlink.Messages.Data64
--import qualified SMACCMPilot.Mavlink.Messages.Data96
import qualified SMACCMPilot.Mavlink.Messages.Heartbeat
import qualified SMACCMPilot.Mavlink.Messages.SysStatus
import qualified SMACCMPilot.Mavlink.Messages.SystemTime
import qualified SMACCMPilot.Mavlink.Messages.Ping
import qualified SMACCMPilot.Mavlink.Messages.ChangeOperatorControl
import qualified SMACCMPilot.Mavlink.Messages.ChangeOperatorControlAck
import qualified SMACCMPilot.Mavlink.Messages.AuthKey
import qualified SMACCMPilot.Mavlink.Messages.SetMode
import qualified SMACCMPilot.Mavlink.Messages.ParamRequestRead
import qualified SMACCMPilot.Mavlink.Messages.ParamRequestList
import qualified SMACCMPilot.Mavlink.Messages.ParamValue
import qualified SMACCMPilot.Mavlink.Messages.ParamSet
import qualified SMACCMPilot.Mavlink.Messages.GpsRawInt
--import qualified SMACCMPilot.Mavlink.Messages.GpsStatus
import qualified SMACCMPilot.Mavlink.Messages.ScaledImu
import qualified SMACCMPilot.Mavlink.Messages.RawImu
import qualified SMACCMPilot.Mavlink.Messages.RawPressure
import qualified SMACCMPilot.Mavlink.Messages.ScaledPressure
import qualified SMACCMPilot.Mavlink.Messages.Attitude
import qualified SMACCMPilot.Mavlink.Messages.AttitudeQuaternion
import qualified SMACCMPilot.Mavlink.Messages.LocalPositionNed
import qualified SMACCMPilot.Mavlink.Messages.GlobalPositionInt
import qualified SMACCMPilot.Mavlink.Messages.RcChannelsScaled
import qualified SMACCMPilot.Mavlink.Messages.RcChannelsRaw
import qualified SMACCMPilot.Mavlink.Messages.ServoOutputRaw
import qualified SMACCMPilot.Mavlink.Messages.MissionRequestPartialList
import qualified SMACCMPilot.Mavlink.Messages.MissionWritePartialList
import qualified SMACCMPilot.Mavlink.Messages.MissionItem
import qualified SMACCMPilot.Mavlink.Messages.MissionRequest
import qualified SMACCMPilot.Mavlink.Messages.MissionSetCurrent
import qualified SMACCMPilot.Mavlink.Messages.MissionCurrent
import qualified SMACCMPilot.Mavlink.Messages.MissionRequestList
import qualified SMACCMPilot.Mavlink.Messages.MissionCount
import qualified SMACCMPilot.Mavlink.Messages.MissionClearAll
import qualified SMACCMPilot.Mavlink.Messages.MissionItemReached
import qualified SMACCMPilot.Mavlink.Messages.MissionAck
import qualified SMACCMPilot.Mavlink.Messages.SetGpsGlobalOrigin
import qualified SMACCMPilot.Mavlink.Messages.GpsGlobalOrigin
import qualified SMACCMPilot.Mavlink.Messages.SetLocalPositionSetpoint
import qualified SMACCMPilot.Mavlink.Messages.LocalPositionSetpoint
import qualified SMACCMPilot.Mavlink.Messages.GlobalPositionSetpointInt
import qualified SMACCMPilot.Mavlink.Messages.SetGlobalPositionSetpointInt
import qualified SMACCMPilot.Mavlink.Messages.SafetySetAllowedArea
import qualified SMACCMPilot.Mavlink.Messages.SafetyAllowedArea
import qualified SMACCMPilot.Mavlink.Messages.SetRollPitchYawThrust
import qualified SMACCMPilot.Mavlink.Messages.SetRollPitchYawSpeedThrust
import qualified SMACCMPilot.Mavlink.Messages.RollPitchYawThrustSetpoint
import qualified SMACCMPilot.Mavlink.Messages.RollPitchYawSpeedThrustSetpoint
import qualified SMACCMPilot.Mavlink.Messages.SetQuadMotorsSetpoint
import qualified SMACCMPilot.Mavlink.Messages.SetQuadSwarmRollPitchYawThrust
import qualified SMACCMPilot.Mavlink.Messages.NavControllerOutput
import qualified SMACCMPilot.Mavlink.Messages.SetQuadSwarmLedRollPitchYawThrust
import qualified SMACCMPilot.Mavlink.Messages.StateCorrection
import qualified SMACCMPilot.Mavlink.Messages.RequestDataStream
import qualified SMACCMPilot.Mavlink.Messages.DataStream
import qualified SMACCMPilot.Mavlink.Messages.ManualControl
import qualified SMACCMPilot.Mavlink.Messages.RcChannelsOverride
import qualified SMACCMPilot.Mavlink.Messages.VfrHud
import qualified SMACCMPilot.Mavlink.Messages.CommandLong
import qualified SMACCMPilot.Mavlink.Messages.CommandAck
import qualified SMACCMPilot.Mavlink.Messages.RollPitchYawRatesThrustSetpoint
import qualified SMACCMPilot.Mavlink.Messages.ManualSetpoint
import qualified SMACCMPilot.Mavlink.Messages.LocalPositionNedSystemGlobalOffset
import qualified SMACCMPilot.Mavlink.Messages.HilState
import qualified SMACCMPilot.Mavlink.Messages.HilControls
import qualified SMACCMPilot.Mavlink.Messages.HilRcInputsRaw
import qualified SMACCMPilot.Mavlink.Messages.OpticalFlow
import qualified SMACCMPilot.Mavlink.Messages.GlobalVisionPositionEstimate
import qualified SMACCMPilot.Mavlink.Messages.VisionPositionEstimate
import qualified SMACCMPilot.Mavlink.Messages.VisionSpeedEstimate
import qualified SMACCMPilot.Mavlink.Messages.ViconPositionEstimate
import qualified SMACCMPilot.Mavlink.Messages.HighresImu
import qualified SMACCMPilot.Mavlink.Messages.BatteryStatus
import qualified SMACCMPilot.Mavlink.Messages.Setpoint8dof
import qualified SMACCMPilot.Mavlink.Messages.Setpoint6dof
import qualified SMACCMPilot.Mavlink.Messages.MemoryVect
import qualified SMACCMPilot.Mavlink.Messages.DebugVect
import qualified SMACCMPilot.Mavlink.Messages.NamedValueFloat
import qualified SMACCMPilot.Mavlink.Messages.NamedValueInt
import qualified SMACCMPilot.Mavlink.Messages.Statustext
import qualified SMACCMPilot.Mavlink.Messages.Debug

mavlinkMessageModules :: [Module]
mavlinkMessageModules =
  [ SMACCMPilot.Mavlink.Messages.Data16.data16Module
  , SMACCMPilot.Mavlink.Messages.Data32.data32Module
  , SMACCMPilot.Mavlink.Messages.Data64.data64Module
--  , SMACCMPilot.Mavlink.Messages.Data96.data96Module
  , SMACCMPilot.Mavlink.Messages.Heartbeat.heartbeatModule
  , SMACCMPilot.Mavlink.Messages.SysStatus.sysStatusModule
  , SMACCMPilot.Mavlink.Messages.SystemTime.systemTimeModule
  , SMACCMPilot.Mavlink.Messages.Ping.pingModule
  , SMACCMPilot.Mavlink.Messages.ChangeOperatorControl.changeOperatorControlModule
  , SMACCMPilot.Mavlink.Messages.ChangeOperatorControlAck.changeOperatorControlAckModule
  , SMACCMPilot.Mavlink.Messages.AuthKey.authKeyModule
  , SMACCMPilot.Mavlink.Messages.SetMode.setModeModule
  , SMACCMPilot.Mavlink.Messages.ParamRequestRead.paramRequestReadModule
  , SMACCMPilot.Mavlink.Messages.ParamRequestList.paramRequestListModule
  , SMACCMPilot.Mavlink.Messages.ParamValue.paramValueModule
  , SMACCMPilot.Mavlink.Messages.ParamSet.paramSetModule
  , SMACCMPilot.Mavlink.Messages.GpsRawInt.gpsRawIntModule
--  , SMACCMPilot.Mavlink.Messages.GpsStatus.gpsStatusModule
  , SMACCMPilot.Mavlink.Messages.ScaledImu.scaledImuModule
  , SMACCMPilot.Mavlink.Messages.RawImu.rawImuModule
  , SMACCMPilot.Mavlink.Messages.RawPressure.rawPressureModule
  , SMACCMPilot.Mavlink.Messages.ScaledPressure.scaledPressureModule
  , SMACCMPilot.Mavlink.Messages.Attitude.attitudeModule
  , SMACCMPilot.Mavlink.Messages.AttitudeQuaternion.attitudeQuaternionModule
  , SMACCMPilot.Mavlink.Messages.LocalPositionNed.localPositionNedModule
  , SMACCMPilot.Mavlink.Messages.GlobalPositionInt.globalPositionIntModule
  , SMACCMPilot.Mavlink.Messages.RcChannelsScaled.rcChannelsScaledModule
  , SMACCMPilot.Mavlink.Messages.RcChannelsRaw.rcChannelsRawModule
  , SMACCMPilot.Mavlink.Messages.ServoOutputRaw.servoOutputRawModule
  , SMACCMPilot.Mavlink.Messages.MissionRequestPartialList.missionRequestPartialListModule
  , SMACCMPilot.Mavlink.Messages.MissionWritePartialList.missionWritePartialListModule
  , SMACCMPilot.Mavlink.Messages.MissionItem.missionItemModule
  , SMACCMPilot.Mavlink.Messages.MissionRequest.missionRequestModule
  , SMACCMPilot.Mavlink.Messages.MissionSetCurrent.missionSetCurrentModule
  , SMACCMPilot.Mavlink.Messages.MissionCurrent.missionCurrentModule
  , SMACCMPilot.Mavlink.Messages.MissionRequestList.missionRequestListModule
  , SMACCMPilot.Mavlink.Messages.MissionCount.missionCountModule
  , SMACCMPilot.Mavlink.Messages.MissionClearAll.missionClearAllModule
  , SMACCMPilot.Mavlink.Messages.MissionItemReached.missionItemReachedModule
  , SMACCMPilot.Mavlink.Messages.MissionAck.missionAckModule
  , SMACCMPilot.Mavlink.Messages.SetGpsGlobalOrigin.setGpsGlobalOriginModule
  , SMACCMPilot.Mavlink.Messages.GpsGlobalOrigin.gpsGlobalOriginModule
  , SMACCMPilot.Mavlink.Messages.SetLocalPositionSetpoint.setLocalPositionSetpointModule
  , SMACCMPilot.Mavlink.Messages.LocalPositionSetpoint.localPositionSetpointModule
  , SMACCMPilot.Mavlink.Messages.GlobalPositionSetpointInt.globalPositionSetpointIntModule
  , SMACCMPilot.Mavlink.Messages.SetGlobalPositionSetpointInt.setGlobalPositionSetpointIntModule
  , SMACCMPilot.Mavlink.Messages.SafetySetAllowedArea.safetySetAllowedAreaModule
  , SMACCMPilot.Mavlink.Messages.SafetyAllowedArea.safetyAllowedAreaModule
  , SMACCMPilot.Mavlink.Messages.SetRollPitchYawThrust.setRollPitchYawThrustModule
  , SMACCMPilot.Mavlink.Messages.SetRollPitchYawSpeedThrust.setRollPitchYawSpeedThrustModule
  , SMACCMPilot.Mavlink.Messages.RollPitchYawThrustSetpoint.rollPitchYawThrustSetpointModule
  , SMACCMPilot.Mavlink.Messages.RollPitchYawSpeedThrustSetpoint.rollPitchYawSpeedThrustSetpointModule
  , SMACCMPilot.Mavlink.Messages.SetQuadMotorsSetpoint.setQuadMotorsSetpointModule
  , SMACCMPilot.Mavlink.Messages.SetQuadSwarmRollPitchYawThrust.setQuadSwarmRollPitchYawThrustModule
  , SMACCMPilot.Mavlink.Messages.NavControllerOutput.navControllerOutputModule
  , SMACCMPilot.Mavlink.Messages.SetQuadSwarmLedRollPitchYawThrust.setQuadSwarmLedRollPitchYawThrustModule
  , SMACCMPilot.Mavlink.Messages.StateCorrection.stateCorrectionModule
  , SMACCMPilot.Mavlink.Messages.RequestDataStream.requestDataStreamModule
  , SMACCMPilot.Mavlink.Messages.DataStream.dataStreamModule
  , SMACCMPilot.Mavlink.Messages.ManualControl.manualControlModule
  , SMACCMPilot.Mavlink.Messages.RcChannelsOverride.rcChannelsOverrideModule
  , SMACCMPilot.Mavlink.Messages.VfrHud.vfrHudModule
  , SMACCMPilot.Mavlink.Messages.CommandLong.commandLongModule
  , SMACCMPilot.Mavlink.Messages.CommandAck.commandAckModule
  , SMACCMPilot.Mavlink.Messages.RollPitchYawRatesThrustSetpoint.rollPitchYawRatesThrustSetpointModule
  , SMACCMPilot.Mavlink.Messages.ManualSetpoint.manualSetpointModule
  , SMACCMPilot.Mavlink.Messages.LocalPositionNedSystemGlobalOffset.localPositionNedSystemGlobalOffsetModule
  , SMACCMPilot.Mavlink.Messages.HilState.hilStateModule
  , SMACCMPilot.Mavlink.Messages.HilControls.hilControlsModule
  , SMACCMPilot.Mavlink.Messages.HilRcInputsRaw.hilRcInputsRawModule
  , SMACCMPilot.Mavlink.Messages.OpticalFlow.opticalFlowModule
  , SMACCMPilot.Mavlink.Messages.GlobalVisionPositionEstimate.globalVisionPositionEstimateModule
  , SMACCMPilot.Mavlink.Messages.VisionPositionEstimate.visionPositionEstimateModule
  , SMACCMPilot.Mavlink.Messages.VisionSpeedEstimate.visionSpeedEstimateModule
  , SMACCMPilot.Mavlink.Messages.ViconPositionEstimate.viconPositionEstimateModule
  , SMACCMPilot.Mavlink.Messages.HighresImu.highresImuModule
  , SMACCMPilot.Mavlink.Messages.BatteryStatus.batteryStatusModule
  , SMACCMPilot.Mavlink.Messages.Setpoint8dof.setpoint8dofModule
  , SMACCMPilot.Mavlink.Messages.Setpoint6dof.setpoint6dofModule
  , SMACCMPilot.Mavlink.Messages.MemoryVect.memoryVectModule
  , SMACCMPilot.Mavlink.Messages.DebugVect.debugVectModule
  , SMACCMPilot.Mavlink.Messages.NamedValueFloat.namedValueFloatModule
  , SMACCMPilot.Mavlink.Messages.NamedValueInt.namedValueIntModule
  , SMACCMPilot.Mavlink.Messages.Statustext.statustextModule
  , SMACCMPilot.Mavlink.Messages.Debug.debugModule
  ]


-- [(Message Id, (Message Length, Message CRC))]
messageLensCRCs :: [(Word8, (Word8, Word8))]
messageLensCRCs =
  [ (169, ( 18,  46)) -- 0xA9 DATA16
  , (170, ( 34, 240)) -- 0xAA DATA32
  , (171, ( 66, 170)) -- 0xAB DATA64
  , (172, ( 98, 185)) -- 0xAC DATA96
  , (  0, (  9,  50)) -- 0x00 HEARTBEAT
  , (  1, ( 31, 124)) -- 0x01 SYS_STATUS
  , (  2, ( 12, 137)) -- 0x02 SYSTEM_TIME
  , (  4, ( 14, 237)) -- 0x04 PING
  , (  5, ( 28, 217)) -- 0x05 CHANGE_OPERATOR_CONTROL
  , (  6, (  3, 104)) -- 0x06 CHANGE_OPERATOR_CONTROL_ACK
  , (  7, ( 32, 119)) -- 0x07 AUTH_KEY
  , ( 11, (  6,  89)) -- 0x0B SET_MODE
  , ( 20, ( 20, 214)) -- 0x14 PARAM_REQUEST_READ
  , ( 21, (  2, 159)) -- 0x15 PARAM_REQUEST_LIST
  , ( 22, ( 25, 220)) -- 0x16 PARAM_VALUE
  , ( 23, ( 23, 168)) -- 0x17 PARAM_SET
  , ( 24, ( 30,  24)) -- 0x18 GPS_RAW_INT
  , ( 25, (101,  23)) -- 0x19 GPS_STATUS
  , ( 26, ( 22, 170)) -- 0x1A SCALED_IMU
  , ( 27, ( 26, 144)) -- 0x1B RAW_IMU
  , ( 28, ( 16,  67)) -- 0x1C RAW_PRESSURE
  , ( 29, ( 14, 115)) -- 0x1D SCALED_PRESSURE
  , ( 30, ( 28,  39)) -- 0x1E ATTITUDE
  , ( 31, ( 32, 246)) -- 0x1F ATTITUDE_QUATERNION
  , ( 32, ( 28, 185)) -- 0x20 LOCAL_POSITION_NED
  , ( 33, ( 28, 104)) -- 0x21 GLOBAL_POSITION_INT
  , ( 34, ( 22, 237)) -- 0x22 RC_CHANNELS_SCALED
  , ( 35, ( 22, 244)) -- 0x23 RC_CHANNELS_RAW
  , ( 36, ( 21, 222)) -- 0x24 SERVO_OUTPUT_RAW
  , ( 37, (  6, 212)) -- 0x25 MISSION_REQUEST_PARTIAL_LIST
  , ( 38, (  6,   9)) -- 0x26 MISSION_WRITE_PARTIAL_LIST
  , ( 39, ( 37, 254)) -- 0x27 MISSION_ITEM
  , ( 40, (  4, 230)) -- 0x28 MISSION_REQUEST
  , ( 41, (  4,  28)) -- 0x29 MISSION_SET_CURRENT
  , ( 42, (  2,  28)) -- 0x2A MISSION_CURRENT
  , ( 43, (  2, 132)) -- 0x2B MISSION_REQUEST_LIST
  , ( 44, (  4, 221)) -- 0x2C MISSION_COUNT
  , ( 45, (  2, 232)) -- 0x2D MISSION_CLEAR_ALL
  , ( 46, (  2,  11)) -- 0x2E MISSION_ITEM_REACHED
  , ( 47, (  3, 153)) -- 0x2F MISSION_ACK
  , ( 48, ( 13,  41)) -- 0x30 SET_GPS_GLOBAL_ORIGIN
  , ( 49, ( 12,  39)) -- 0x31 GPS_GLOBAL_ORIGIN
  , ( 50, ( 19, 214)) -- 0x32 SET_LOCAL_POSITION_SETPOINT
  , ( 51, ( 17, 223)) -- 0x33 LOCAL_POSITION_SETPOINT
  , ( 52, ( 15, 141)) -- 0x34 GLOBAL_POSITION_SETPOINT_INT
  , ( 53, ( 15,  33)) -- 0x35 SET_GLOBAL_POSITION_SETPOINT_INT
  , ( 54, ( 27,  15)) -- 0x36 SAFETY_SET_ALLOWED_AREA
  , ( 55, ( 25,   3)) -- 0x37 SAFETY_ALLOWED_AREA
  , ( 56, ( 18, 100)) -- 0x38 SET_ROLL_PITCH_YAW_THRUST
  , ( 57, ( 18,  24)) -- 0x39 SET_ROLL_PITCH_YAW_SPEED_THRUST
  , ( 58, ( 20, 239)) -- 0x3A ROLL_PITCH_YAW_THRUST_SETPOINT
  , ( 59, ( 20, 238)) -- 0x3B ROLL_PITCH_YAW_SPEED_THRUST_SETPOINT
  , ( 60, (  9,  30)) -- 0x3C SET_QUAD_MOTORS_SETPOINT
  , ( 61, ( 34, 240)) -- 0x3D SET_QUAD_SWARM_ROLL_PITCH_YAW_THRUST
  , ( 62, ( 26, 183)) -- 0x3E NAV_CONTROLLER_OUTPUT
  , ( 63, ( 46, 130)) -- 0x3F SET_QUAD_SWARM_LED_ROLL_PITCH_YAW_THRUST
  , ( 64, ( 36, 130)) -- 0x40 STATE_CORRECTION
  , ( 66, (  6, 148)) -- 0x42 REQUEST_DATA_STREAM
  , ( 67, (  4,  21)) -- 0x43 DATA_STREAM
  , ( 69, ( 11, 243)) -- 0x45 MANUAL_CONTROL
  , ( 70, ( 18, 124)) -- 0x46 RC_CHANNELS_OVERRIDE
  , ( 74, ( 20,  20)) -- 0x4A VFR_HUD
  , ( 76, ( 33, 152)) -- 0x4C COMMAND_LONG
  , ( 77, (  3, 143)) -- 0x4D COMMAND_ACK
  , ( 80, ( 20, 127)) -- 0x50 ROLL_PITCH_YAW_RATES_THRUST_SETPOINT
  , ( 81, ( 22, 106)) -- 0x51 MANUAL_SETPOINT
  , ( 89, ( 28, 231)) -- 0x59 LOCAL_POSITION_NED_SYSTEM_GLOBAL_OFFSET
  , ( 90, ( 56, 183)) -- 0x5A HIL_STATE
  , ( 91, ( 42,  63)) -- 0x5B HIL_CONTROLS
  , ( 92, ( 33,  54)) -- 0x5C HIL_RC_INPUTS_RAW
  , (100, ( 26, 175)) -- 0x64 OPTICAL_FLOW
  , (101, ( 32, 102)) -- 0x65 GLOBAL_VISION_POSITION_ESTIMATE
  , (102, ( 32, 158)) -- 0x66 VISION_POSITION_ESTIMATE
  , (103, ( 20, 208)) -- 0x67 VISION_SPEED_ESTIMATE
  , (104, ( 32,  56)) -- 0x68 VICON_POSITION_ESTIMATE
  , (105, ( 62,  93)) -- 0x69 HIGHRES_IMU
  , (147, ( 16,  42)) -- 0x93 BATTERY_STATUS
  , (148, ( 33, 241)) -- 0x94 SETPOINT_8DOF
  , (149, ( 25,  15)) -- 0x95 SETPOINT_6DOF
  , (249, ( 36, 204)) -- 0xF9 MEMORY_VECT
  , (250, ( 30,  49)) -- 0xFA DEBUG_VECT
  , (251, ( 18, 170)) -- 0xFB NAMED_VALUE_FLOAT
  , (252, ( 18,  44)) -- 0xFC NAMED_VALUE_INT
  , (253, ( 51,  83)) -- 0xFD STATUSTEXT
  , (254, (  9,  46)) -- 0xFE DEBUG
  ]


