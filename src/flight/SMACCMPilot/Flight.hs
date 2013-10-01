{-# LANGUAGE ScopedTypeVariables #-}
{-# LANGUAGE DataKinds #-}

module SMACCMPilot.Flight
  ( flight
  , hil
  ) where

import Ivory.Language
import Ivory.Tower

import Ivory.Stdlib.String (stdlibStringModule)

import SMACCMPilot.Flight.Types (typeModules)
import SMACCMPilot.Flight.Control (controlModules)
-- import SMACCMPilot.Flight.Datalink
-- import qualified SMACCMPilot.Flight.Datalink.TestHarness as DLink

import SMACCMPilot.Flight.Control.Task
import SMACCMPilot.Flight.Motors.Task
import SMACCMPilot.Flight.Motors.Platforms
import SMACCMPilot.Flight.Sensors.Task
import SMACCMPilot.Flight.Sensors.Platforms
import SMACCMPilot.Flight.UserInput.Task
import SMACCMPilot.Flight.BlinkTask
import SMACCMPilot.Flight.GCS.Tower
import SMACCMPilot.Flight.GCS.Transmit.MessageDriver (senderModules)

import SMACCMPilot.Console (consoleModule)

import SMACCMPilot.Mavlink.Messages (mavlinkMessageModules)
import SMACCMPilot.Mavlink.Send (mavlinkSendModule)
import SMACCMPilot.Mavlink.Pack (packModule)
import SMACCMPilot.Mavlink.CRC (mavlinkCRCModule)

import SMACCMPilot.Flight.GCS.Commsec
import Ivory.HXStream

import qualified Ivory.BSP.HWF4.EEPROM as HWF4
import qualified Ivory.BSP.HWF4.I2C as HWF4

import qualified Ivory.BSP.STM32F4.GPIO as GPIO
import qualified Ivory.BSP.STM32F4.UART as UART
import           Ivory.BSP.STM32F4.RCC (BoardHSE(..))

hil :: (BoardHSE p, MotorOutput p, SensorOrientation p)
    => Tower p ()
hil = do
  -- Communication primitives:
  sensors       <- channel

  -- Instantiate core:
  (flightmode, control, motors) <- core (snk sensors)
  motors_state  <- stateProxy motors
  control_state <- stateProxy control

  -- HIL-enabled GCS on uart1:
  (istream, ostream) <- uart UART.uart1
  gcsTowerHil "uart1" istream ostream flightmode
    control_state motors_state sensors

flight :: (BoardHSE p, MotorOutput p, SensorOrientation p)
    => Tower p ()
flight = do
  -- Communication primitives:
  sensors       <- channel
  sensor_state  <- stateProxy (snk sensors)
  position      <- dataport

  -- Instantiate core:
  (flightmode, control, motors) <- core (snk sensors)
  motors_state  <- stateProxy motors
  control_state <- stateProxy control

  -- Real sensors and real motor output:
  task "sensors" $ sensorsTask (src sensors)
  motorOutput motors

  -- GCS on UART1:
  (uart1istream, uart1ostream) <- uart UART.uart1
  gcsTower "uart1" uart1istream uart1ostream flightmode sensor_state
    (snk position) control_state motors_state

  -- -- Extra: for testing datalink code, not fully integrated yet...
  -- datalinkTest UART.uart5

core :: (SingI n)
       => ChannelSink n (Struct "sensors_result")
       -> Tower p ( DataSink (Struct "flightmode")
                  , ChannelSink 16 (Struct "controloutput")
                  , ChannelSink 16 (Struct "motors"))
core sensors = do
  motors  <- channel
  control <- channel

  (userinput, flightmode) <- userInputTower
  task "blink"     $ blinkTask lights flightmode
  task "control"   $ controlTask flightmode userinput sensors (src control)
  task "motmix"    $ motorMixerTask (snk control) flightmode (src motors)

  mapM_ addDepends typeModules
  mapM_ addModule otherms

  return (flightmode, snk control, snk motors)
  where
  lights = [relaypin, redledpin]
  relaypin = GPIO.pinB13
  redledpin = GPIO.pinB14


-- datalinkTest :: (BoardHSE p) => UART.UART -> Tower p ()
-- datalinkTest u = do
--   (byte_istream, byte_ostream) <- uart u
--   (  framed_istream
--    , framed_ostream
--    , stat_istream :: ChannelSink 1 (Struct "radio_stat")
--    , info_istream :: ChannelSink 1 (Struct "radio_info")
--    ) <- datalink byte_istream byte_ostream
--   DLink.frameLoopback framed_istream framed_ostream
--   -- XXX do something with stat and info

otherms :: [Module]
otherms = concat
  -- flight types
  [ typeModules
  -- control subsystem
  , controlModules
  -- mavlink system
  , mavlinkMessageModules
  ] ++
  [ packModule
  , mavlinkCRCModule
  , consoleModule
  -- hwf4 bsp is used for I2C, EEPROM
  , HWF4.eepromModule
  , HWF4.i2cModule
  -- the rest:
  , stdlibStringModule

  -- crypto
  , commsecModule
  -- hxstream
  , hxstreamModule

  , senderModules
  , mavlinkSendModule
  ]

-- Helper: a uartTower with 1k buffers and 57600 kbaud
uart :: (BoardHSE p)
     => UART.UART
     -> Tower p ( ChannelSink   1024 (Stored Uint8)
                , ChannelSource 1024 (Stored Uint8))
uart u = UART.uartTower u 57600
