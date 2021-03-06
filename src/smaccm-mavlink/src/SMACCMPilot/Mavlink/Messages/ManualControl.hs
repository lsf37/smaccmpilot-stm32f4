{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE MultiParamTypeClasses #-}

-- Autogenerated Mavlink v1.0 implementation: see smavgen_ivory.py

module SMACCMPilot.Mavlink.Messages.ManualControl where

import SMACCMPilot.Mavlink.Pack
import SMACCMPilot.Mavlink.Unpack
import SMACCMPilot.Mavlink.Send

import Ivory.Language
import Ivory.Stdlib

manualControlMsgId :: Uint8
manualControlMsgId = 69

manualControlCrcExtra :: Uint8
manualControlCrcExtra = 243

manualControlModule :: Module
manualControlModule = package "mavlink_manual_control_msg" $ do
  depend packModule
  depend mavlinkSendModule
  incl mkManualControlSender
  incl manualControlUnpack
  defStruct (Proxy :: Proxy "manual_control_msg")

[ivory|
struct manual_control_msg
  { x :: Stored Sint16
  ; y :: Stored Sint16
  ; z :: Stored Sint16
  ; r :: Stored Sint16
  ; buttons :: Stored Uint16
  ; target :: Stored Uint8
  }
|]

mkManualControlSender ::
  Def ('[ ConstRef s0 (Struct "manual_control_msg")
        , Ref s1 (Stored Uint8) -- seqNum
        , Ref s1 (Struct "mavlinkPacket") -- tx buffer/length
        ] :-> ())
mkManualControlSender =
  proc "mavlink_manual_control_msg_send"
  $ \msg seqNum sendStruct -> body
  $ do
  arr <- local (iarray [] :: Init (Array 11 (Stored Uint8)))
  let buf = toCArray arr
  call_ pack buf 0 =<< deref (msg ~> x)
  call_ pack buf 2 =<< deref (msg ~> y)
  call_ pack buf 4 =<< deref (msg ~> z)
  call_ pack buf 6 =<< deref (msg ~> r)
  call_ pack buf 8 =<< deref (msg ~> buttons)
  call_ pack buf 10 =<< deref (msg ~> target)
  -- 6: header len, 2: CRC len
  let usedLen    = 6 + 11 + 2 :: Integer
  let sendArr    = sendStruct ~> mav_array
  let sendArrLen = arrayLen sendArr
  if sendArrLen < usedLen
    then error "manualControl payload of length 11 is too large!"
    else do -- Copy, leaving room for the payload
            arrayCopy sendArr arr 6 (arrayLen arr)
            call_ mavlinkSendWithWriter
                    manualControlMsgId
                    manualControlCrcExtra
                    11
                    seqNum
                    sendStruct

instance MavlinkUnpackableMsg "manual_control_msg" where
    unpackMsg = ( manualControlUnpack , manualControlMsgId )

manualControlUnpack :: Def ('[ Ref s1 (Struct "manual_control_msg")
                             , ConstRef s2 (CArray (Stored Uint8))
                             ] :-> () )
manualControlUnpack = proc "mavlink_manual_control_unpack" $ \ msg buf -> body $ do
  store (msg ~> x) =<< call unpack buf 0
  store (msg ~> y) =<< call unpack buf 2
  store (msg ~> z) =<< call unpack buf 4
  store (msg ~> r) =<< call unpack buf 6
  store (msg ~> buttons) =<< call unpack buf 8
  store (msg ~> target) =<< call unpack buf 10

