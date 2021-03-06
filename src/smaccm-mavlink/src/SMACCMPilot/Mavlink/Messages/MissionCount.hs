{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE MultiParamTypeClasses #-}

-- Autogenerated Mavlink v1.0 implementation: see smavgen_ivory.py

module SMACCMPilot.Mavlink.Messages.MissionCount where

import SMACCMPilot.Mavlink.Pack
import SMACCMPilot.Mavlink.Unpack
import SMACCMPilot.Mavlink.Send

import Ivory.Language
import Ivory.Stdlib

missionCountMsgId :: Uint8
missionCountMsgId = 44

missionCountCrcExtra :: Uint8
missionCountCrcExtra = 221

missionCountModule :: Module
missionCountModule = package "mavlink_mission_count_msg" $ do
  depend packModule
  depend mavlinkSendModule
  incl mkMissionCountSender
  incl missionCountUnpack
  defStruct (Proxy :: Proxy "mission_count_msg")

[ivory|
struct mission_count_msg
  { count :: Stored Uint16
  ; target_system :: Stored Uint8
  ; target_component :: Stored Uint8
  }
|]

mkMissionCountSender ::
  Def ('[ ConstRef s0 (Struct "mission_count_msg")
        , Ref s1 (Stored Uint8) -- seqNum
        , Ref s1 (Struct "mavlinkPacket") -- tx buffer/length
        ] :-> ())
mkMissionCountSender =
  proc "mavlink_mission_count_msg_send"
  $ \msg seqNum sendStruct -> body
  $ do
  arr <- local (iarray [] :: Init (Array 4 (Stored Uint8)))
  let buf = toCArray arr
  call_ pack buf 0 =<< deref (msg ~> count)
  call_ pack buf 2 =<< deref (msg ~> target_system)
  call_ pack buf 3 =<< deref (msg ~> target_component)
  -- 6: header len, 2: CRC len
  let usedLen    = 6 + 4 + 2 :: Integer
  let sendArr    = sendStruct ~> mav_array
  let sendArrLen = arrayLen sendArr
  if sendArrLen < usedLen
    then error "missionCount payload of length 4 is too large!"
    else do -- Copy, leaving room for the payload
            arrayCopy sendArr arr 6 (arrayLen arr)
            call_ mavlinkSendWithWriter
                    missionCountMsgId
                    missionCountCrcExtra
                    4
                    seqNum
                    sendStruct

instance MavlinkUnpackableMsg "mission_count_msg" where
    unpackMsg = ( missionCountUnpack , missionCountMsgId )

missionCountUnpack :: Def ('[ Ref s1 (Struct "mission_count_msg")
                             , ConstRef s2 (CArray (Stored Uint8))
                             ] :-> () )
missionCountUnpack = proc "mavlink_mission_count_unpack" $ \ msg buf -> body $ do
  store (msg ~> count) =<< call unpack buf 0
  store (msg ~> target_system) =<< call unpack buf 2
  store (msg ~> target_component) =<< call unpack buf 3

