{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE MultiParamTypeClasses #-}

-- Autogenerated Mavlink v1.0 implementation: see smavgen_ivory.py

module SMACCMPilot.Mavlink.Messages.GpsGlobalOrigin where

import SMACCMPilot.Mavlink.Pack
import SMACCMPilot.Mavlink.Unpack
import SMACCMPilot.Mavlink.Send

import Ivory.Language
import Ivory.Stdlib

gpsGlobalOriginMsgId :: Uint8
gpsGlobalOriginMsgId = 49

gpsGlobalOriginCrcExtra :: Uint8
gpsGlobalOriginCrcExtra = 39

gpsGlobalOriginModule :: Module
gpsGlobalOriginModule = package "mavlink_gps_global_origin_msg" $ do
  depend packModule
  depend mavlinkSendModule
  incl mkGpsGlobalOriginSender
  incl gpsGlobalOriginUnpack
  defStruct (Proxy :: Proxy "gps_global_origin_msg")

[ivory|
struct gps_global_origin_msg
  { latitude :: Stored Sint32
  ; longitude :: Stored Sint32
  ; altitude :: Stored Sint32
  }
|]

mkGpsGlobalOriginSender ::
  Def ('[ ConstRef s0 (Struct "gps_global_origin_msg")
        , Ref s1 (Stored Uint8) -- seqNum
        , Ref s1 (Struct "mavlinkPacket") -- tx buffer/length
        ] :-> ())
mkGpsGlobalOriginSender =
  proc "mavlink_gps_global_origin_msg_send"
  $ \msg seqNum sendStruct -> body
  $ do
  arr <- local (iarray [] :: Init (Array 12 (Stored Uint8)))
  let buf = toCArray arr
  call_ pack buf 0 =<< deref (msg ~> latitude)
  call_ pack buf 4 =<< deref (msg ~> longitude)
  call_ pack buf 8 =<< deref (msg ~> altitude)
  -- 6: header len, 2: CRC len
  let usedLen    = 6 + 12 + 2 :: Integer
  let sendArr    = sendStruct ~> mav_array
  let sendArrLen = arrayLen sendArr
  if sendArrLen < usedLen
    then error "gpsGlobalOrigin payload of length 12 is too large!"
    else do -- Copy, leaving room for the payload
            arrayCopy sendArr arr 6 (arrayLen arr)
            call_ mavlinkSendWithWriter
                    gpsGlobalOriginMsgId
                    gpsGlobalOriginCrcExtra
                    12
                    seqNum
                    sendStruct

instance MavlinkUnpackableMsg "gps_global_origin_msg" where
    unpackMsg = ( gpsGlobalOriginUnpack , gpsGlobalOriginMsgId )

gpsGlobalOriginUnpack :: Def ('[ Ref s1 (Struct "gps_global_origin_msg")
                             , ConstRef s2 (CArray (Stored Uint8))
                             ] :-> () )
gpsGlobalOriginUnpack = proc "mavlink_gps_global_origin_unpack" $ \ msg buf -> body $ do
  store (msg ~> latitude) =<< call unpack buf 0
  store (msg ~> longitude) =<< call unpack buf 4
  store (msg ~> altitude) =<< call unpack buf 8

