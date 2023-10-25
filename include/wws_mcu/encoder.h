/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_ENCODER_H___
#define ___WWS_ENCODER_H___

#include "typedef.h"
#include "logic.h"
#include "service.h"

/**
 * @brief encoder
 */
typedef struct __wws_encoder_t
{
  /**
   * @brief encoder inputs
   */
  wws_logic_reader_t *const cw, *const ccw;
  /**
   * @brief active logic
   */
  const wws_logic_t active;
  /**
   * @brief value of encoder (cw+, ccw-)
   */
  volatile int value;
  /**
   * @brief flags
   */
  union
  {
    unsigned int flags;
    struct
    {
      unsigned int _stx : 1;
      unsigned int _mid : 1;
      unsigned int _cw : 1;
      unsigned int _ccw : 1;
    };
  };
} wws_encoder_t;

extern void ___wws_encoder_service_callback(wws_phase_t on, wws_service_t *serv);

/**
 * @brief service for encoder
 */
#define WWS_ENCODER_SERVICE .callback = ___wws_encoder_service_callback, .default_start = 1

/**
 * @brief read encoder value
 * @param enc
 * @return
 */
static inline int wws_encoder_read(wws_encoder_t *enc)
{
  int t      = enc->value;
  enc->value = 0;
  return t;
}

#endif /* ___WWS_ENCODER_H___ */
