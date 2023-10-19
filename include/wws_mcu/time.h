/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_TIME_H___
#define ___WWS_TIME_H___

#include <stdbool.h>
#include "typedef.h"

/**
 * @brief Config for tick/us
 */
#ifndef WWS_CONFIG_TICK_PER_US
#define WWS_CONFIG_TICK_PER_US (100)
#endif /* WWS_CONFIG_TICK_PER_US */

/**
 * @brief global ticks
 */
extern volatile unsigned int wws_tick;

/**
 * @brief us to tick
 * @param _us us
 * @return unsigned int
 */
#define WWS_US(_us) ((unsigned int) ((_us) / WWS_CONFIG_TICK_PER_US))

/**
 * @brief ms to tick
 * @param _ms ms
 * @return unsigned int
 */
#define WWS_MS(_ms) (WWS_US(1000) * (_ms))

/**
 * @brief sec to tick
 * @param _sec sec
 * @return unsigned int
 */
#define WWS_SEC(_sec) (WWS_MS(1000) * (_sec))

/**
 * @brief get tick
 * @return unsigned int system tick
 */
static inline unsigned int wws_tick_get()
{
  return wws_tick;
}

/**
 * @brief is tick up
 * @param stamp old stamp to be compared
 * @param compare ticks to compare
 * @return bool up or not
 */
static inline bool wws_tick_isup(unsigned int stamp, unsigned int compare)
{
  return (wws_tick_get() - stamp) >= compare;
}

/**
 * @brief delay for ticks
 * @param ticks
 */
extern void wws_delay(unsigned int ticks);

/**
 * @brief system uptime calculated by tick
 */
extern struct __wws_uptime_t
{
  /**
   * @brief day
   */
  unsigned int day;
  /**
   * @brief hour (0-23)
   */
  unsigned int hour : 5;
  /**
   * @brief mins (0-59)
   */
  unsigned int min : 6;
  /**
   * @brief seconds (0-59)
   */
  unsigned int sec : 6;
  /**
   * @brief milliseconds (0-999)
   */
  unsigned int msec : 10;
} wws_uptime;


/**
 * @brief system tick increase
 */
extern void ___wws_tick_inc();

#endif /* ___WWS_TIME_H___ */
