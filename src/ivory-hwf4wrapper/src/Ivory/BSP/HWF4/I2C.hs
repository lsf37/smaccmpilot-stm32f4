{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE DataKinds #-}
{-# LANGUAGE ScopedTypeVariables #-}
{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE FlexibleInstances #-}

{-# OPTIONS_GHC -fno-warn-orphans #-}

--
-- I2C.hs --- HWF4 I2C driver interface.
--
-- Copyright (C) 2013, Galois, Inc.
-- All Rights Reserved.
--


-- For now, we just export the driver handles since that's all that is
-- needed for the EEPROM driver.  We may want to eventually flesh this
-- out enough so that we can write peripheral drivers in Ivory.

module Ivory.BSP.HWF4.I2C
  ( i2c1
  , i2c2
  , i2cModule
  ) where

import Ivory.Language

-- | Opaque structure for an I2C bus driver.
[ivory| abstract struct i2cdrv_t "hwf4/i2c.h"|]

-- | Ivory module for this driver.
i2cModule :: Module
i2cModule = package "bsp_hwf4wrapper_i2c" $ do
  inclHeader "hwf4/i2c.h"
  defStruct (Proxy :: Proxy "i2cdrv_t")
  defMemArea i2c1
  defMemArea i2c2

-- | Driver handle for the I2C1 bus.
i2c1 :: MemArea (Struct "i2cdrv_t")
i2c1 = importArea "_i2c1_drv" "hwf4/i2c.h"

-- | Driver handle for the I2C2 bus.
i2c2 :: MemArea (Struct "i2cdrv_t")
i2c2 = importArea "_i2c2_drv" "hwf4/i2c.h"

