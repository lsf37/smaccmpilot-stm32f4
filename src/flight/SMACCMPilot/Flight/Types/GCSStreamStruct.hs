{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE QuasiQuotes #-}

{-# OPTIONS_GHC -fno-warn-orphans #-}

module SMACCMPilot.Flight.Types.GCSStreamStruct where

import Ivory.Language

[ivory|

struct gcsstream_data
  { period        :: Stored Uint32
  ; hard_deadline :: Stored IBool -- hard real-time or soft real-time
  }

|]

