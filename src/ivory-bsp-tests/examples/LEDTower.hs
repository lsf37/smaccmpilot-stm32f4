{-# LANGUAGE DataKinds #-}

module LEDTower where

import Ivory.Language
import Ivory.Tower

import Ivory.HW.Module
import Ivory.BSP.STM32F4.GPIO

data LEDPolarity = ActiveHigh | ActiveLow
data LED = LED GPIOPin LEDPolarity

ledSetup :: LED -> Ivory eff ()
ledSetup led@(LED pin _polarity) = do
  pinEnable pin
  pinSetOutputType pin gpio_outputtype_pushpull
  pinSetSpeed pin gpio_speed_2mhz
  pinSetPUPD pin gpio_pupd_none
  ledOff led

ledOn :: LED -> Ivory eff ()
ledOn (LED pin ActiveHigh) = pinHigh pin
ledOn (LED pin ActiveLow)  = pinLow  pin

ledOff :: LED -> Ivory eff ()
ledOff (LED pin _) = pinHiZ pin

pinLow :: GPIOPin -> Ivory eff ()
pinLow pin = do
  pinClear pin
  pinSetMode pin gpio_mode_output

pinHigh :: GPIOPin -> Ivory eff ()
pinHigh pin = do
  pinSet pin
  pinSetMode pin gpio_mode_output

pinHiZ :: GPIOPin -> Ivory eff ()
pinHiZ pin = pinSetMode pin gpio_mode_analog
------------------------------

-- | LED Controller: Given a set of leds and a control channel of booleans,
--   setup the pin hardware, and turn the leds on when the control channel is
--   true.
ledController :: (SingI n) => [LED] -> ChannelSink n (Stored IBool) -> Task p ()
ledController leds rxer = do
  -- Bookkeeping: this task uses Ivory.HW.Module.hw_moduledef
  taskModuleDef $ hw_moduledef
  -- Setup hardware before running any event handlers
  taskInit $
    mapM_ ledSetup leds
  -- Run a callback on each message posted to the channel
  onChannelV rxer "output" $ \out -> do
    -- Turn pins on or off according to event value
    ifte_ out
      (mapM_ ledOn  leds)
      (mapM_ ledOff leds)

-- | Blink task: Given a period and a channel source, output an alternating
--   stream of true / false on each period.
blink :: (SingI n) => Integer -> ChannelSource n (Stored IBool) -> Task p ()
blink per outSource = do
  -- Bring the emitter into scope for this Task
  outEmitter <- withChannelEmitter outSource "output"
  -- Declare a period for this Task
  onPeriod per $ \time ->
    -- Emit boolean value which will alternate each period.
    emitV_ outEmitter (time .% (2*p) <? p)
  where p = fromIntegral per :: Uint32

blinkApp :: Integer -> [LED] -> Tower p ()
blinkApp period pins = do
  (src_led, sink_led) <- channel
  task "blink"  $ blink         period src_led
  task "ledhw"  $ ledController pins   sink_led

