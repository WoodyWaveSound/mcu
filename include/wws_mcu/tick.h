/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_TICK_H___
#define ___WWS_TICK_H___

#include "typedef.h"
#include "time.h"
#include "debug.h"
#include "service.h"

extern wws_comp_t WWS_COMP_TICK;

/**
 * @brief tick callback for user space
 */
typedef void (*wws_tick_callback_t)(void);

/**
 * @brief global tick callback
 */
extern wws_tick_callback_t ___wws_tick_callback;

/**
 * @brief set callback for tick run
 * @param callback
 */
static inline void wws_tick_set_callback(wws_tick_callback_t callback)
{
  ___wws_tick_callback = callback;
}

/**
 * @brief wws tick to be runned in tick handler of platform
 */
#define wws_platform_tick()                                                                        \
  do {                                                                                             \
    ___wws_tick_inc();                                                                             \
    if (___wws_tick_callback) { ___wws_tick_callback(); }                                          \
    wws_service_run(WWS_ON_TICK);                                                                  \
    wws_event_only(WWS_COMP_TICK, 0);                                                              \
  } while (0)

#endif /* ___WWS_TICK_H___ */
