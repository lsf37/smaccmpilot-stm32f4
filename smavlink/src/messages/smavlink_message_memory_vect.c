/* This file has been autogenerated by Ivory
 * Compiler version  Version {versionBranch = [0,1,0,0], versionTags = []}
 */
#include <smavlink/pack.h>
#include "smavlink_message_memory_vect.h"
void smavlink_send_memory_vect(struct memory_vect_msg* n_var0,
                               struct smavlink_out_channel* n_var1,
                               struct smavlink_system* n_var2)
{
    uint8_t n_local0[36U] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0};
    uint8_t(* n_ref1)[36U] = &n_local0;
    uint16_t n_deref2 = *&n_var0->address;
    
    smavlink_pack_uint16_t(n_ref1, 0U, n_deref2);
    
    uint8_t n_deref3 = *&n_var0->ver;
    
    smavlink_pack_uint8_t(n_ref1, 2U, n_deref3);
    
    uint8_t n_deref4 = *&n_var0->memory_vect_type;
    
    smavlink_pack_uint8_t(n_ref1, 3U, n_deref4);
    
    int8_t(* n_let5)[32U] = &n_var0->value;
    
    for (uint8_t n_ix6 = 0U % 32U; n_ix6 < 31U % 32U; n_ix6 = n_ix6 + 1U %
         32U) {
        int8_t n_deref7 = *&(*n_let5)[n_ix6];
        
        smavlink_pack_int8_t(n_ref1, 4U + n_ix6, n_deref7);
    }
    smavlink_send_ivory(n_var1, n_var2, 249U, n_ref1, 36U, 204U);
    return;
}
void smavlink_unpack_memory_vect(struct memory_vect_msg* n_var0, const
                                 uint8_t** n_var1)
{
    uint16_t n_r0 = smavlink_unpack_uint16_t(n_var1, 0U);
    
    *&n_var0->address = n_r0;
    
    uint8_t n_r1 = smavlink_unpack_uint8_t(n_var1, 2U);
    
    *&n_var0->ver = n_r1;
    
    uint8_t n_r2 = smavlink_unpack_uint8_t(n_var1, 3U);
    
    *&n_var0->memory_vect_type = n_r2;
    for (uint8_t n_ix3 = 0U % 32U; n_ix3 < 31U % 32U; n_ix3 = n_ix3 + 1U %
         32U) {
        int8_t n_r4 = smavlink_unpack_int8_t(n_var1, 4U + n_ix3);
        
        *&(*&n_var0->value)[n_ix3] = n_r4;
    }
}
