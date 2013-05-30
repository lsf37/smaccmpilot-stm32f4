#/usr/bin/env python
'''
parse a MAVLink protocol XML file and generate a C implementation

Copyright Andrew Tridgell 2011
Copyright Pat Hickey 2013
Released under GNU GPL version 3 or later
'''

import sys, textwrap, os, time
import mavparse, mavtemplate

t = mavtemplate.MAVTemplate()

def message_imports(ms):
    imports = [ "import qualified SMACCMPilot.Mavlink.Messages."
              + m.name_module for m in ms ]
    return ("\n").join(imports)

def generate_messages_hs(directory, ms):
    '''generate umbrella haskell SMACCMPilot.Mavlink.Messages module'''
    exports = [ "module SMACCMPilot.Mavlink.Messages."
              + m.name_module for m in ms ]
    modules = ( "mavlinkMessageModules :: [Module]\n"
              + "mavlinkMessageModules =\n  [ "
              + ("\n  , ").join([ "SMACCMPilot.Mavlink.Messages.%s.%sModule" %
                  (m.name_module , m.name_camel) for m in ms])
              + "\n  ]\n"
              )
    messageLCs = ( "messageLensCRCs :: [(Word8, (Word8, Word8))]\n"
                 + "messageLensCRCs =\n"
                 + "  [ " + ("\n  , ").join(["(%d, (%d, %d))"
                                            % (m.id, m.wire_length, m.crc_extra)
                                            for m in ms])
                 + "\n  ]\n"
                 )

    f = open(os.path.join(directory, 'Messages.hs'), mode='w')
    t.write(f, '''
{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE Rank2Types #-}

module SMACCMPilot.Mavlink.Messages
  ( mavlinkMessageModules
  , messageLensCRCs
  ) where

import Data.Word (Word8)
import Ivory.Language

${imports}

${modulesList}

${messageLCs}

''', { 'exports': ("\n  , ").join(exports)
     , 'imports': message_imports(ms)
     , 'modulesList': modules
     , 'messageLCs': messageLCs
     })
    f.close()

def generate_senders_hs(directory, ms):
    '''generate umbrella haskell SMACCMPilot.Mavlink.Senders module'''

    datadecl = ( "data MavlinkMessageSenders =\n  MavlinkMessageSenders\n    { "
                 + ("\n    , ").join([
                     "%sSender :: forall s . Def ('[ ConstRef s (Struct \"%s_msg\") ] :->())"
                      % ( m.name_camel, m.name_lower ) for m in ms ])
                 + "\n    }\n"
               )
    senderCons = ( "mavlinkMessageSenders :: MavlinkSender -> "
                 + " (MavlinkMessageSenders, Module)\n"
                 + "mavlinkMessageSenders wrappedSender = (s, m)\n  where"
                 + "\n  sender :: (forall n struct . (SingI n, MavlinkSendable struct n) => Proxy struct -> SizedMavlinkSender n)"
                 + "\n  sender _proxy = case wrappedSender of MavlinkSender unwrapped -> unwrapped"
                 + "\n  s = MavlinkMessageSenders\n    { "
                 + ("\n    , ").join([ ("%sSender = mkSender (sender ( Proxy :: " +
                     "Proxy \"%s_msg\"))")
                                    % (m.name_camel, m.name_lower)
                                    for m in ms ])
                 + "\n    }"
                 + "\n  m = package (\"mavlink_msg_senders_\" ++ "
                 +                         "(senderName (sender (Proxy :: Proxy \"heartbeat_msg\")))) $ do"
                 + "\n        mapM_ depend mavlinkMessageModules"
                 + "\n        senderDeps (sender (Proxy :: Proxy \"heartbeat_msg\"))"
                 + "\n        "
                 + ("\n        ").join([ "incl (%sSender s)"
                                         % (m.name_camel) for m in ms])

                 )
    f = open(os.path.join(directory, 'Senders.hs'), mode='w')
    t.write(f, '''
{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE Rank2Types #-}

module SMACCMPilot.Mavlink.Senders
  ( MavlinkMessageSenders(..)
  , mavlinkMessageSenders
  ) where

import Ivory.Language

import SMACCMPilot.Mavlink.Send
import SMACCMPilot.Mavlink.Messages

${datadecl}

${senderCons}

''', { 'datadecl': datadecl
     , 'senderCons': senderCons
     })
    f.close()

def ivory_name(field , m):
    f = field.name.encode('ascii')
    # catch haskell reserved words
    if (f == "type" and m.name == "HEARTBEAT"):
        return "mavtype"
    if (f in ["type", "seq"]):
       return m.name.lower() + "_" + f
    else:
      return f

ivory_type = {
        'float'    : 'IFloat',
        'double'   : 'IDouble',
        'char'     : 'Uint8',
        'int8_t'   : 'Sint8',
        'uint8_t'  : 'Uint8',
        'uint8_t_mavlink_version' : 'Uint8',
        'int16_t'  : 'Sint16',
        'uint16_t' : 'Uint16',
        'int32_t'  : 'Sint32',
        'uint32_t' : 'Uint32',
        'int64_t'  : 'Sint64',
        'uint64_t' : 'Uint64'}
sizeof_type = {
        'float'    : 4,
        'double'   : 8,
        'char'     : 1,
        'int8_t'   : 1,
        'uint8_t'  : 1,
        'uint8_t_mavlink_version'  : 1,
        'int16_t'  : 2,
        'uint16_t' : 2,
        'int32_t'  : 4,
        'uint32_t' : 4,
        'int64_t'  : 8,
        'uint64_t' : 8,
        }


def generate_message_ivory(directory, m):
    '''generate haskell file containing ivory module'''
    scalar_defs = [ ivory_name(s,m) + " :: Stored " + ivory_type[s.type]
                    for s in m.scalar_fields ]
    array_defs = [ ivory_name(s,m) + " :: Array %d (Stored %s)" %
                    (s.array_length, ivory_type[s.type])
                    for s in m.array_fields ]
    scalar_pack_calls = [ "call_ pack buf %d =<< deref (msg ~> %s)" %
                    (s.wire_offset, ivory_name(s,m))
                    for s in m.scalar_fields]
    array_pack_calls = [ "arrayPack buf %d (msg ~> %s)" %
                    (s.wire_offset, ivory_name(s,m))
                    for s in m.array_fields]
    scalar_unpack_calls = [ "store (msg ~> %s) =<< call unpack buf %d" %
                            (ivory_name(s, m), s.wire_offset)
                            for s in m.scalar_fields]
    array_unpack_calls = [ "arrayUnpack buf %d (msg ~> %s)" %
                           (s.wire_offset, ivory_name(s, m))
                           for s in m.array_fields]
    mdict = m.__dict__
    mdict.update({'struct_fields': ("\n  ; ").join(scalar_defs + array_defs)
                 ,'packing': ("\n  ").join(scalar_pack_calls + array_pack_calls)
                 ,'unpacking': "\n  ".join(scalar_unpack_calls + array_unpack_calls)})
    f = open(os.path.join(directory, 'Messages/%s.hs' % m.name_module), mode='w')
    t.write(f, '''
{-# LANGUAGE DataKinds #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE MultiParamTypeClasses #-}

-- Autogenerated Mavlink v1.0 implementation: see smavlink_ivory.py

module SMACCMPilot.Mavlink.Messages.${name_module} where

import SMACCMPilot.Mavlink.Pack
import SMACCMPilot.Mavlink.Unpack
import SMACCMPilot.Mavlink.Send

import Ivory.Language

${name_camel}MsgId :: Uint8
${name_camel}MsgId = ${id}

${name_camel}CrcExtra :: Uint8
${name_camel}CrcExtra = ${crc_extra}

${name_camel}Module :: Module
${name_camel}Module = package "mavlink_${name_lower}_msg" $ do
  depend packModule
  incl ${name_camel}Unpack
  defStruct (Proxy :: Proxy "${name_lower}_msg")

[ivory|
struct ${name_lower}_msg
  { ${struct_fields}
  }
|]

mk${name_module}Sender :: SizedMavlinkSender ${wire_length}
                       -> Def ('[ ConstRef s (Struct "${name_lower}_msg") ] :-> ())
mk${name_module}Sender sender =
  proc ("mavlink_${name_lower}_msg_send" ++ (senderName sender)) $ \msg -> body $ do
    ${name_camel}Pack (senderMacro sender) msg

instance MavlinkSendable "${name_lower}_msg" ${wire_length} where
  mkSender = mk${name_module}Sender

${name_camel}Pack :: (eff `AllocsIn` s, eff `Returns` ())
                  => SenderMacro eff s ${wire_length}
                  -> ConstRef s1 (Struct "${name_lower}_msg")
                  -> Ivory eff ()
${name_camel}Pack sender msg = do
  arr <- local (iarray [] :: Init (Array ${wire_length} (Stored Uint8)))
  let buf = toCArray arr
  ${packing}
  sender ${name_camel}MsgId (constRef arr) ${name_camel}CrcExtra
  retVoid

instance MavlinkUnpackableMsg "${name_lower}_msg" where
    unpackMsg = ( ${name_camel}Unpack , ${name_camel}MsgId )

${name_camel}Unpack :: Def ('[ Ref s1 (Struct "${name_lower}_msg")
                             , ConstRef s2 (CArray (Stored Uint8))
                             ] :-> () )
${name_camel}Unpack = proc "mavlink_${name_lower}_unpack" $ \ msg buf -> body $ do
  ${unpacking}

''', mdict)
    f.close()

class mav_include(object):
    def __init__(self, base):
        self.base = base

def process_xml(basename, xml):
    '''generate headers for one XML file'''

    directory = basename

    if xml.little_endian:
        xml.mavlink_endian = "MAVLINK_LITTLE_ENDIAN"
    else:
        xml.mavlink_endian = "MAVLINK_BIG_ENDIAN"

    if xml.crc_extra:
        xml.crc_extra_define = "1"
    else:
        xml.crc_extra_define = "0"

    if xml.sort_fields:
        xml.aligned_fields_define = "1"
    else:
        xml.aligned_fields_define = "0"

    # work out the included headers
    xml.include_list = []
    for i in xml.include:
        base = i[:-4]
        xml.include_list.append(mav_include(base))

    # form message lengths array
    xml.message_lengths_array = ''
    for mlen in xml.message_lengths:
        xml.message_lengths_array += '%u, ' % mlen
    xml.message_lengths_array = xml.message_lengths_array[:-2]

    # and message CRCs array
    xml.message_crcs_array = ''
    for crc in xml.message_crcs:
        xml.message_crcs_array += '%u, ' % crc
    xml.message_crcs_array = xml.message_crcs_array[:-2]

    # form message info array
    xml.message_info_array = ''
    for name in xml.message_names:
        if name is not None:
            xml.message_info_array += 'MAVLINK_MESSAGE_INFO_%s, ' % name
        else:
            # Several C compilers don't accept {NULL} for
            # multi-dimensional arrays and structs
            # feed the compiler a "filled" empty message
            xml.message_info_array += '{"EMPTY",0,{{"","",MAVLINK_TYPE_CHAR,0,0,0}}}, '
    xml.message_info_array = xml.message_info_array[:-2]

    # add some extra field attributes for convenience with arrays
    for m in xml.message:
        m.msg_name = m.name
        if xml.crc_extra:
            m.crc_extra_arg = ", %s" % m.crc_extra
        else:
            m.crc_extra_arg = ""
        for f in m.fields:
            if f.print_format is None:
                f.c_print_format = 'NULL'
            else:
                f.c_print_format = '"%s"' % f.print_format
            if f.array_length != 0:
                f.array_suffix = '[%u]' % f.array_length
                f.array_prefix = '*'
                f.array_tag = '_array'
                f.array_arg = ', %u' % f.array_length
                f.array_return_arg = '%s, %u, ' % (f.name, f.array_length)
                f.array_const = 'const '
                f.decode_left = ''
                f.decode_right = ', %s->%s' % (m.name_lower, f.name)
                f.return_type = 'uint16_t'
                f.get_arg = ', %s *%s' % (f.type, f.name)
                if f.type == 'char':
                    f.c_test_value = '"%s"' % f.test_value
                else:
                    test_strings = []
                    for v in f.test_value:           test_strings.append(str(v))
                    f.c_test_value = '{ %s }' % ', '.join(test_strings)
            else:
                f.array_suffix = ''
                f.array_prefix = ''
                f.array_tag = ''
                f.array_arg = ''
                f.array_return_arg = ''
                f.array_const = ''
                f.decode_left = "%s->%s = " % (m.name_lower, f.name)
                f.decode_right = ''
                f.get_arg = ''
                f.return_type = f.type
                if f.type == 'char':
                    f.c_test_value = "'%s'" % f.test_value
                elif f.type == 'uint64_t':
                    f.c_test_value = "%sULL" % f.test_value                    
                elif f.type == 'int64_t':
                    f.c_test_value = "%sLL" % f.test_value                    
                else:
                    f.c_test_value = f.test_value

    # cope with uint8_t_mavlink_version
    for m in xml.message:
        m.arg_fields = []
        m.array_fields = []
        m.scalar_fields = []
        for f in m.ordered_fields:
            if f.array_length != 0:
                m.array_fields.append(f)
            else:
                m.scalar_fields.append(f)
        for f in m.fields:
            if not f.omit_arg:
                m.arg_fields.append(f)
                f.putname = f.name
            else:
                f.putname = f.const_value
    return xml


def generate_messages(basename, xml_list):
    for xml in xml_list:
        x = process_xml(basename, xml);
        print("Generating SMACCMPilot.Mavlink.Messages modules in directory %s" %
                        basename)
        mavparse.mkdir_p(basename)
        generate_messages_hs(basename, xml.message)
        generate_senders_hs(basename, xml.message)
        for m in xml.message:
            generate_message_ivory(basename, m)
