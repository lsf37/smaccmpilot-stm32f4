{-# LANGUAGE ScopedTypeVariables #-}

-- Compiler imports:
import Ivory.Language
import Ivory.Tower.Frontend
import qualified Ivory.HW.SearchDir as HW
import qualified Ivory.BSP.STM32F4.SearchDir as BSP

-- App imports:
import Ivory.Tower
import LEDTower (blinkApp)
import Platforms

app :: forall p . (ColoredLEDs p) => Tower p ()
app = blinkApp period leds
  where
  period = 250
  leds = [redLED p, blueLED p]
  p = (Proxy :: Proxy p)

main :: IO ()
main = compilePlatforms conf (coloredLEDPlatforms app)
  where
  conf = searchPathConf [HW.searchDir, BSP.searchDir]


