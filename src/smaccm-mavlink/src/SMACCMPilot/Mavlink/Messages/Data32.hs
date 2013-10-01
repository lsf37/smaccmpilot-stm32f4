{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE MultiParamTypeClasses #-}

-- Autogenerated Mavlink v1.0 implementation: see smavgen_ivory.py

module SMACCMPilot.Mavlink.Messages.Data32 where

import SMACCMPilot.Mavlink.Pack
import SMACCMPilot.Mavlink.Unpack
import SMACCMPilot.Mavlink.Send
import qualified SMACCMPilot.Shared as S

import Ivory.Language
import Ivory.Stdlib

data32MsgId :: Uint8
data32MsgId = 170

data32CrcExtra :: Uint8
data32CrcExtra = 240

data32Module :: Module
data32Module = package "mavlink_data32_msg" $ do
  depend packModule
  depend mavlinkSendModule
  incl mkData32Sender
  incl data32Unpack
  defStruct (Proxy :: Proxy "data32_msg")

[ivory|
struct data32_msg
  { data32_type :: Stored Uint8
  ; len :: Stored Uint8
  ; data32 :: Array 32 (Stored Uint8)
  }
|]

mkData32Sender ::
  Def ('[ ConstRef s0 (Struct "data32_msg")
        , Ref s1 (Stored Uint8) -- seqNum
        , Ref s1 S.MavLinkArray -- tx buffer
        ] :-> ())
mkData32Sender =
  proc "mavlink_data32_msg_send"
  $ \msg seqNum sendArr -> body
  $ do
  arr <- local (iarray [] :: Init (Array 34 (Stored Uint8)))
  let buf = toCArray arr
  call_ pack buf 0 =<< deref (msg ~> data32_type)
  call_ pack buf 1 =<< deref (msg ~> len)
  arrayPack buf 2 (msg ~> data32)
  -- 6: header len, 2: CRC len
  let usedLen = 6 + 34 + 2 :: Integer
  let sendArrLen = arrayLen sendArr
  if sendArrLen < usedLen
    then error "data32 payload of length 34 is too large!"
    else do -- Copy, leaving room for the payload
            arrCopy sendArr arr 6
            call_ mavlinkSendWithWriter
                    data32MsgId
                    data32CrcExtra
                    34
                    seqNum
                    sendArr
            let usedLenIx = fromInteger usedLen
            -- Zero out the unused portion of the array.
            for (fromInteger sendArrLen - usedLenIx) $ \ix ->
              store (sendArr ! (ix + usedLenIx)) 0
            retVoid

instance MavlinkUnpackableMsg "data32_msg" where
    unpackMsg = ( data32Unpack , data32MsgId )

data32Unpack :: Def ('[ Ref s1 (Struct "data32_msg")
                             , ConstRef s2 (CArray (Stored Uint8))
                             ] :-> () )
data32Unpack = proc "mavlink_data32_unpack" $ \ msg buf -> body $ do
  store (msg ~> data32_type) =<< call unpack buf 0
  store (msg ~> len) =<< call unpack buf 1
  arrayUnpack buf 2 (msg ~> data32)

