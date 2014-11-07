{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeFamilies #-}

module SMACCMPilot.Flight.Recovery
  ( recoveryTower
  ) where

-- | Tower-tasks managing fault/attack recovery mechanisms.

import qualified SMACCMPilot.Mavlink.Messages.VehCommsec as V
--import qualified SMACCMPilot.Communications              as Comm

import           Ivory.Language
import           Ivory.Stdlib
import           Ivory.Tower
import qualified SMACCMPilot.Flight.Types.CommsecStatus as C

--------------------------------------------------------------------------------

recoveryTower :: DataSink   (Struct "veh_commsec_msg") -- Commsec/Decrypt task
              -> DataSource (Stored C.CommsecStatus)  -- To GCS Transmit
              -> Tower p ()
recoveryTower commsec_info_snk monitor_result_src =

  -- XXX We might have multiple recovery tasks here doing multiple things.  For
  -- now, we just have one task, looking at commsec messages.
  task "commsecRecoveryTask"
    $ commsecRecoveryTask commsec_info_snk monitor_result_src

--------------------------------------------------------------------------------

-- | True is OK, False is an alarm.
commsecRecoveryTask :: DataSink   (Struct "veh_commsec_msg")
                    -> DataSource (Stored C.CommsecStatus)
                    -> Task p ()
commsecRecoveryTask commsec_info_snk monitor_result_src = do
  commsec_info_snk_rx <- withDataReader commsec_info_snk "commsec_info_snk"
  monitor_res_tx      <- withDataWriter monitor_result_src "comm_mon_res"

  -- Property result
  result   <- taskLocalInit "result" (ival C.secure)

  onPeriod 20 $ \_now -> do
    commsecReader <- local izero
    readData commsec_info_snk_rx commsecReader
    commsecMonitor commsecReader result
    writeData monitor_res_tx (constRef result)

  taskModuleDef $ do
    depend V.vehCommsecModule

  where
  -- Set alarm if we've received 1 bad message (ever).
  commsecMonitor rx resRef = do
    totalBadMsgs <- rx ~>* V.bad_msgs
    ifte_ (totalBadMsgs ==? 0)
      (store resRef C.secure)
      (store resRef C.alarm)