/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_LOGIC_FILTER_H___
#define ___WWS_LOGIC_FILTER_H___

#include "typedef.h"
#include "logic.h"
#include "service.h"

extern wws_comp_t WWS_COMP_LOGIC_FILTER;
extern wws_evt_t  WWS_EVT_CHANGE;

/**
 * @brief logic reader with filter
 */
typedef struct __wws_logic_filter_t
{
  /**
   * @brief cached logic (filtered)
   */
  wws_logic_t logic;
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
 * @param reader reader which inst is filter
 */
extern void wws_logic_filter_update(wws_logic_reader_t *reader);

extern void ___wws_logic_filter_service_callback(wws_phase_t on, wws_service_t *serv);

#define WWS_LOGIC_FILTER_SERVICE                                                                   \
  .callback = ___wws_logic_filter_service_callback, .default_start = 1

#endif /* ___WWS_LOGIC_FILTER_H___ */
