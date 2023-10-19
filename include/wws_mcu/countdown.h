/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_COUNTDOWN_H___
#define ___WWS_COUNTDOWN_H___

#include <stdbool.h>
#include "typedef.h"

/**
 * @brief countdown
 */
typedef struct __wws_countdown_t
{
  /**
   * @brief timestamp cache
   */
  unsigned int timestamp;
  /**
   * @brief flag for countdown
   */
  unsigned int counting : 1;
} wws_countdown_t;


/**
 * @brief recount cd
 * @param cd
 */
extern void wws_countdown_recount(wws_countdown_t *cd);

/**
 * @brief is count up to tick
 * @param cd
 * @param ticks
 * @return
 */
extern bool wws_countdown_isup(wws_countdown_t *cd, unsigned int ticks);

/**
 * @brief stop countdown
 * @param cd
 */
extern void wws_countdown_stop(wws_countdown_t *cd);

/**
 * @brief is cd counting
 * @param cd
 * @return counting?
 */
extern bool wws_countdown_iscounting(wws_countdown_t *cd);

#endif /* ___WWS_COUNTDOWN_H___ */