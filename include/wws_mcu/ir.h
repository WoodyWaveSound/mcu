/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_IR_H___
#define ___WWS_IR_H___

#include "typedef.h"
#include "logic.h"

/**
 * @warning Need 100Khz timer to use
 */

extern wws_ret_t WWS_RET_CHANGE;
extern wws_ret_t WWS_RET_KEEP;


/**
 * @brief cache for ir update
 */
struct ___wws_ir_cache_t
{
  wws_logic_t  old;
  unsigned int code;
  unsigned int count;
};

/**
 * @brief protocol definitions
 */
typedef struct __wws_ir_protocol_t
{
  wws_ret_t (*const update)(wws_logic_t input, struct ___wws_ir_cache_t *cache);
} wws_ir_protocol_t;

/**
 * @brief NEC IR protocol
 */
extern wws_ir_protocol_t wws_ir_nec_protocol;

/**
 * @brief infrared remote definitions
 */
typedef struct __wws_ir_t
{
  /**
   * @brief input reader
   */
  wws_logic_reader_t *const reader;
  /**
   * @brief protocol of IR
   */
  wws_ir_protocol_t *const protocol;
  /**
   * @brief cache for protocol update
   */
  struct ___wws_ir_cache_t _cache;
  /**
   * @brief code of update
   */
  unsigned int code;
} wws_ir_t;

/**
 * @brief update ir code
 * @param ir
 * @warning need to be run in 100KHz (100us) timer
 */
extern void wws_ir_update(wws_ir_t *ir);

/**
 * @brief code read from IR
 * @param ir
 * @return 0: not read yet or idle
 */
static inline unsigned int wws_ir_get(wws_ir_t *ir)
{
  return ir->code;
}

#endif /* ___WWS_IR_H___ */
