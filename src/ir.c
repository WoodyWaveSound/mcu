/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/ir.h>

WWS_WEAK wws_ret_t WWS_RET_CHANGE = "CHANGE";
WWS_WEAK wws_ret_t WWS_RET_KEEP   = "KEEP";

void wws_ir_update(wws_ir_t *ir)
{
  if (ir->protocol->update(wws_logic_read(ir->reader), &ir->_cache) == WWS_RET_CHANGE) {
    ir->code = ir->_cache.code;
  }
}


/**
 * NEC Protocol
 */
static wws_ret_t nec_update(wws_logic_t input, struct ___wws_ir_cache_t *cache)
{
#define NEC_IDLE WWS_LOW
  if (input == cache->old) /** not change */ {
    cache->count++;
    /** idle for 15ms? */
    if ((cache->code != 0) && (input == NEC_IDLE) && (cache->count >= 1100)) {
      /** reset */
      cache->code = 0;
      return WWS_RET_CHANGE;
    }
    return WWS_RET_KEEP;
  }

  wws_ret_t ret = WWS_RET_KEEP;
  if (input != NEC_IDLE) /** to active */ {
    if ((cache->count >= 40) && (cache->count <= 50)) /** leader */
      cache->code = 0;
    else if ((cache->count >= 13) && (cache->count <= 18)) /** high */
      cache->code = (cache->code << 1) | 1;
    else if ((cache->count >= 2) && (cache->count <= 7)) /** low */ cache->code = cache->code << 1;
    else if ((cache->count >= 250) && (cache->count <= 530)) /** stop */ ret = WWS_RET_CHANGE;
  }
  cache->count = 0;
  cache->old   = input;
  return ret;
}
wws_ir_protocol_t wws_ir_nec_protocol = { .update = nec_update };
