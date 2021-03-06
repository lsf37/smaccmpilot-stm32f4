{-# LANGUAGE DataKinds #-}
{-# LANGUAGE Rank2Types #-}

module Platform where

import Ivory.Language
import Ivory.Tower
import Ivory.Tower.Frontend

import Ivory.BSP.STM32F4.RCC
import Ivory.BSP.STM32F4.UART

f24MHz :: Uint32
f24MHz = 24000000
f8MHz :: Uint32
f8MHz = 8000000

data PX4FMU17_IOAR = PX4FMU17_IOAR
data PX4FMU17_Bare = PX4FMU17_Bare
data Open407VC     = Open407VC

class GPSUart p where
  consoleUart :: Proxy p -> UART
  gpsUart     :: Proxy p -> UART

instance BoardHSE PX4FMU17_IOAR where
  hseFreq _ = f24MHz
instance GPSUart PX4FMU17_IOAR where
  consoleUart _ = uart1
  gpsUart _ = uart6

instance BoardHSE PX4FMU17_Bare where
  hseFreq _ = f24MHz
instance GPSUart PX4FMU17_Bare where
  consoleUart _ = uart1
  gpsUart _ = uart6

instance BoardHSE Open407VC where
  hseFreq _ = f8MHz
instance GPSUart Open407VC where
  consoleUart _ = uart1
  gpsUart _ = uart2

gpsPlatforms :: (forall p . (GPSUart p, BoardHSE p) => Tower p ())
               -> [(String, Twr)]
gpsPlatforms app =
    [("px4fmu17_ioar", Twr (app :: Tower PX4FMU17_IOAR ()))
    ,("px4fmu17_bare", Twr (app :: Tower PX4FMU17_Bare ()))
    ,("open407vc",     Twr (app :: Tower Open407VC ()))
    ]
