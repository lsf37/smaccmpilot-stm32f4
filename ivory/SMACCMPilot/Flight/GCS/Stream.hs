{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE ExistentialQuantification #-}
{-# LANGUAGE Rank2Types #-}

module SMACCMPilot.Flight.GCS.Stream where

import Prelude hiding (last)

import Ivory.Language

import qualified SMACCMPilot.Mavlink.Enums.MavDataStreams as MavDS

import SMACCMPilot.Flight.Types.GCSStreamTiming

defaultPeriods :: Init (Struct "gcsstream_timing")
defaultPeriods =
  istruct
    [ heartbeat            .= ival 1000
    , servo_output_raw     .= ival 0
    , attitude             .= ival 0
    , gps_raw_int          .= ival 0
    , vfr_hud              .= ival 0
    , global_position_int  .= ival 0
    , params               .= ival 100
    ]

-- hide the scope from the typechecker until application. once we have an
-- ivory monad without a codescope parameter, we wont need this or the ugly
-- recursion below
newtype SelectorAction =
  SelectorAction
    { unwrapSelectorAction :: forall s . Label "gcsstream_timing" (Stored Uint32)
                           -> Ivory s () () }

updateGCSStreamPeriods :: Ref s1 (Struct "gcsstream_timing")
                       -> Uint8  -- request stream id (mavlink enum typed)
                       -> IBool  -- enabled
                       -> Uint16 -- stream rate in hertz
                       -> Ivory s2 () ()
updateGCSStreamPeriods periods streamid enabled rate = do
  ifte (streamid ==? (fromIntegral MavDS.id_ALL))
    (mapM_ setrate allstreams)
    (withSelectorFromId streamid (SelectorAction setrate))
  where
  allstreams = [ servo_output_raw, attitude, gps_raw_int
               , vfr_hud, global_position_int ]
  setrate :: Label "gcsstream_timing" (Stored Uint32) -> Ivory s () ()
  setrate selector =
    ifte enabled
      (store (periods ~> selector) newperiod)
      (store (periods ~> selector) 0)
    where
    --newperiod = castWith 0 $ 1000 / ((safeCast rate) :: IFloat)
    newperiod = 1000 `iDiv` (safeCast rate)

  withSelectorFromId :: Uint8
                     -> SelectorAction
                     -> Ivory s () ()
  withSelectorFromId tofind act = aux tbl
    where -- explicit recursion and existential quantification is a little weird
          -- not foldr, because of nested block typing which is going away soon anyway
    aux :: forall s . [(Integer, Label "gcsstream_timing" (Stored Uint32))] -> Ivory s () ()
    aux ((sid, sel):ts) = ifte ((fromIntegral sid) ==? tofind)
                            --((unwrapSelectorAction act) sel)
                            ((unwrapSelectorAction act) sel)
                            (aux ts)
    aux [] = return ()

  tbl ::[(Integer, Label "gcsstream_timing" (Stored Uint32))]
  tbl = [ (MavDS.id_RAW_CONTROLLER,  servo_output_raw)
        , (MavDS.id_EXTRA1,          attitude)
        , (MavDS.id_POSITION,        global_position_int)
        , (MavDS.id_EXTENDED_STATUS, gps_raw_int)
        , (MavDS.id_EXTRA2,          vfr_hud)
        ]

setNewPeriods :: ConstRef s1 (Struct "gcsstream_timing") -- NEW periods
            -> Ref s2 (Struct "gcsstream_timing") -- STATE periods
            -> Ref s3 (Struct "gcsstream_timing") -- schedule
            -> Uint32 -- Now
            -> Ivory s4 () () -- update schedule
setNewPeriods new state schedule now = do
  mapM_ update selectors
  where
  update :: Label "gcsstream_timing" (Stored Uint32) -> Ivory s () ()
  update selector = do
    n <- deref (new ~> selector)
    s <- deref (state ~> selector)
    ifte (n ==? s) (return ()) $ do
      store (state ~> selector) n
      setNextTime (constRef state) schedule selector now
  selectors = [ heartbeat, servo_output_raw, attitude, gps_raw_int
              , vfr_hud, global_position_int, params]

setNextTime :: ConstRef s (Struct "gcsstream_timing") -- periods
            -> Ref s1 (Struct "gcsstream_timing") -- schedule
            -> Label "gcsstream_timing" (Stored Uint32) -- selector for packet sent
            -> Uint32 -- Now
            -> Ivory s2 () () -- update schedule

setNextTime periods schedule selector now = do
  per <- deref (periods ~> selector)
  prev <- deref (schedule ~> selector)
  -- Schedule always for a time ahead of now, but keep consistent
  -- period by adding to prev, if possible.
  -- XXX rollover creates a problem here but that should take 49 days
  next <- assign $ ((prev + per) <? now ) ? (now + per, prev + per)
  store (schedule ~> selector) next

streamDue :: ConstRef s1 (Struct "gcsstream_timing")    -- periods
          -> ConstRef s2 (Struct "gcsstream_timing")    -- schedule
          -> Label "gcsstream_timing" (Stored Uint32) -- stream selector
          -> Uint32  -- last update
          -> Uint32  -- now
          -> Ivory s3 () IBool
streamDue periods schedule selector last now = do
  p <- deref (periods ~> selector)
  active <- assign (p >? 0)
  duetime <- deref (schedule ~> selector)
  return (active .&& (duetime >? last).&& (duetime <=? now))


nextDueTime :: ConstRef s1 (Struct "gcsstream_timing")    -- periods
            -> ConstRef s2 (Struct "gcsstream_timing")    -- schedule
            -> Uint32  -- now
            -> Ivory s3 () Uint32
nextDueTime _periods _schedule now = do
  -- XXX should implement this later.
  return (now + 100)
