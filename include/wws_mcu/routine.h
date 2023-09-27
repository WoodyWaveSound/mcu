/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_ROUTINE_H___
#define ___WWS_ROUTINE_H___

#include <stdbool.h>

#include "typedef.h"
#include "time.h"

/**
 * @brief Routine definition
 */
typedef struct __wws_routine_t
{
  /**
   * @brief callback of routine
   */
  void (*callback)(struct __wws_routine_t *);
  /**
   * @brief callback on routine start
   */
  void (*on_start)(struct __wws_routine_t *);
  /**
   * @brief flags of routine
   */
  unsigned int flags;
  /**
   * @brief pointer to resume to work as co-routine
   */
  void *resume;
  /**
   * @brief timestamp for sleep
   */
  unsigned int timestamp;
  /**
   * @brief context cached routine work
   */
  void *context;
  /**
   * @brief payload for different instance
   */
  void *payload;
} wws_routine_t;


/**
 * @brief start routine
 * @param routine
 * @return is changed?
 */
bool wws_routine_start(wws_routine_t *routine);

/**
 * @brief stop routine
 * @param routine
 * @return is changed?
 */
bool wws_routine_stop(wws_routine_t *routine);

/**
 * @brief resume routine
 * @param routine
 * @return is changed?
 */
bool wws_routine_resume(wws_routine_t *routine);

/**
 * @brief pause routine
 * @param routine
 * @return is changed?
 */
bool wws_routine_pause(wws_routine_t *routine);

/**
 * @brief run routine
 * @param routine
 * @return is run
 */
bool wws_routine_run(wws_routine_t *routine);

/**
 * @brief Resume routine to previous yield, if never started, then start next
 * line
 * @param _routine routine pointer
 */
#define WWS_COROUTINE_RESUME(_routine)                                                             \
  do {                                                                                             \
    if ((_routine)->resume != NULL) { goto *((_routine)->resume); }                                \
  } while (0)

/**
 * @brief Yield routine as co-routine
 * @param _routine routine pointer
 */
#define WWS_COROUTINE_YIELD(_routine)                                                              \
  do {                                                                                             \
    (_routine)->resume = &&WWS_LOCAL_VAR(label);                                                   \
    return;                                                                                        \
    WWS_LOCAL_VAR(label)                                                                           \
    :;                                                                                             \
  } while (0)

/**
 * @brief let routine sleep for ticks as co-routine
 * @param _routine routine pointer
 * @param _ticks ticks to sleep
 */
#define WWS_COROUTINE_SLEEP(_routine, _ticks)                                                      \
  do {                                                                                             \
    (_routine)->timestamp = wws_get_tick();                                                        \
    WWS_COROUTINE_YIELD(_routine);                                                                 \
    if (!wws_is_tickup((_routine)->timestamp, (_ticks))) { return; }                               \
  } while (0)

/**
 * @brief restart routine as co-routine
 * @param _routine
 */
#define WWS_COROUTINE_RESTART(_routine)                                                            \
  do {                                                                                             \
    (_routine)->resume = NULL;                                                                     \
  } while (0)

#endif /* ___WWS_ROUTINE_H___ */
