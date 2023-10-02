/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_LOGIC_FILTER_H___
#define ___WWS_LOGIC_FILTER_H___

#include "logic.h"

extern const char *WWS_COMP_LOGIC_FILTER;
extern const char *WWS_EVT_CHANGE;

/**
 * @brief logic reader with filter
 */
typedef struct __wws_logic_filter_t
{
  /**
   * @brief cached logic (filtered)
   */
  unsigned char logic;
  /**
   * @brief reader for raw logic
   */
  wws_logic_reader_t *const raw;
  /**
   * @brief ticks to high
   */
  const unsigned int rising;
  /**
   * @brief ticks to low
   */
  const unsigned int falling;
  /**
   * @brief timestamp to compare
   */
  unsigned int timestamp;
} wws_logic_filter_t;

/**
 * @brief update filter in reader
 * @param reader reader which payload is filter
 */
extern void wws_logic_filter_update(wws_logic_reader_t *reader);

/**
 * @brief update list of filter in reader
 * @param list list of reader which payload is filter, end of 0
 */
extern void wws_logic_filter_update_list(wws_logic_reader_t *const *const list);

#endif /* ___WWS_LOGIC_FILTER_H___ */
