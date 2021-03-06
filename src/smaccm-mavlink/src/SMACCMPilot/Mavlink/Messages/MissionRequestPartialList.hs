{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE MultiParamTypeClasses #-}

-- Autogenerated Mavlink v1.0 implementation: see smavgen_ivory.py

module SMACCMPilot.Mavlink.Messages.MissionRequestPartialList where

import SMACCMPilot.Mavlink.Pack
import SMACCMPilot.Mavlink.Unpack
import SMACCMPilot.Mavlink.Send

import Ivory.Language
import Ivory.Stdlib

missionRequestPartialListMsgId :: Uint8
missionRequestPartialListMsgId = 37

missionRequestPartialListCrcExtra :: Uint8
missionRequestPartialListCrcExtra = 212

missionRequestPartialListModule :: Module
missionRequestPartialListModule = package "mavlink_mission_request_partial_list_msg" $ do
  depend packModule
  depend mavlinkSendModule
  incl mkMissionRequestPartialListSender
  incl missionRequestPartialListUnpack
  defStruct (Proxy :: Proxy "mission_request_partial_list_msg")

[ivory|
struct mission_request_partial_list_msg
  { start_index :: Stored Sint16
  ; end_index :: Stored Sint16
  ; target_system :: Stored Uint8
  ; target_component :: Stored Uint8
  }
|]

mkMissionRequestPartialListSender ::
  Def ('[ ConstRef s0 (Struct "mission_request_partial_list_msg")
        , Ref s1 (Stored Uint8) -- seqNum
        , Ref s1 (Struct "mavlinkPacket") -- tx buffer/length
        ] :-> ())
mkMissionRequestPartialListSender =
  proc "mavlink_mission_request_partial_list_msg_send"
  $ \msg seqNum sendStruct -> body
  $ do
  arr <- local (iarray [] :: Init (Array 6 (Stored Uint8)))
  let buf = toCArray arr
  call_ pack buf 0 =<< deref (msg ~> start_index)
  call_ pack buf 2 =<< deref (msg ~> end_index)
  call_ pack buf 4 =<< deref (msg ~> target_system)
  call_ pack buf 5 =<< deref (msg ~> target_component)
  -- 6: header len, 2: CRC len
  let usedLen    = 6 + 6 + 2 :: Integer
  let sendArr    = sendStruct ~> mav_array
  let sendArrLen = arrayLen sendArr
  if sendArrLen < usedLen
    then error "missionRequestPartialList payload of length 6 is too large!"
    else do -- Copy, leaving room for the payload
            arrayCopy sendArr arr 6 (arrayLen arr)
            call_ mavlinkSendWithWriter
                    missionRequestPartialListMsgId
                    missionRequestPartialListCrcExtra
                    6
                    seqNum
                    sendStruct

instance MavlinkUnpackableMsg "mission_request_partial_list_msg" where
    unpackMsg = ( missionRequestPartialListUnpack , missionRequestPartialListMsgId )

missionRequestPartialListUnpack :: Def ('[ Ref s1 (Struct "mission_request_partial_list_msg")
                             , ConstRef s2 (CArray (Stored Uint8))
                             ] :-> () )
missionRequestPartialListUnpack = proc "mavlink_mission_request_partial_list_unpack" $ \ msg buf -> body $ do
  store (msg ~> start_index) =<< call unpack buf 0
  store (msg ~> end_index) =<< call unpack buf 2
  store (msg ~> target_system) =<< call unpack buf 4
  store (msg ~> target_component) =<< call unpack buf 5

