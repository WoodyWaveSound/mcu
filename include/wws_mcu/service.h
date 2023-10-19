/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_SERVICE_H___
#define ___WWS_SERVICE_H___

#include "typedef.h"
#include "countdown.h"

extern wws_comp_t WWS_COMP_SERVICE;

extern wws_evt_t WWS_EVT_START;
extern wws_evt_t WWS_EVT_STOP;
extern wws_evt_t WWS_EVT_ROUTINE;
extern wws_evt_t WWS_EVT_TICK;

extern wws_phase_t WWS_ON_START; //   (WWS_EVT_START)
extern wws_phase_t WWS_ON_STOP; //    (WWS_EVT_STOP)
extern wws_phase_t WWS_ON_ROUTINE; // (WWS_EVT_ROUTINE)
extern wws_phase_t WWS_ON_TICK; //    (WWS_EVT_TICK)

/**
 * @brief coroutine
 */
typedef const void *wws_coroutine_t;

/**
 * @brief service
 */
typedef struct __wws_service_t
{
  /**
   * @brief callback
   */
  void (*const callback)(const char *phase, struct __wws_service_t *service);
  /**
   * @brief context of service
   */
  void *const context;
  /**
   * @brief inst of service
   */
  void *const inst;
  /**
   * @brief id of service for control
   */
  const unsigned int id : 8;
  /**
   * @brief enable for tick
   */
  const unsigned int tick_enable : 1;
  /**
   * @brief default start
   */
  const unsigned int default_start : 1;
  /**
   * @brief flag of default start
   */
  unsigned int _default_start : 1;
  /**
   * @brief flag of started
   */
  unsigned int _started : 1;
  /**
   * @brief name of service
   */
  const char *name;
} wws_service_t;

/**
 * @brief service
 */
extern wws_service_t *wws_services;

/**
 * @brief get service by id
 * @param id
 * @return wws_service_t*
 */
extern wws_service_t *wws_service_by_id(unsigned char id);

/**
 * @brief get service by index
 * @param index
 * @return wws_service_t*
 */
static inline wws_service_t *wws_service_by_index(unsigned char index)
{
  return &(wws_services)[index];
}

/**
 * @brief service init
 * @param services
 */
static inline void wws_service_init(wws_service_t services[])
{
  wws_services = (wws_service_t *) services;
}

/**
 * @brief run service on event
 * @param event
 */
extern void wws_service_run(const char *event);

/**
 * @brief start service
 * @param serv
 */
extern void wws_service_start(wws_service_t *serv);

/**
 * @brief stop service
 * @param serv
 */
extern void wws_service_stop(wws_service_t *serv);

/**
 * @brief run coroutine
 */
#define WWS_COROUTINE_RUN(_coroutine)                                                              \
  do {                                                                                             \
    if ((_coroutine) != 0) { goto *(_coroutine); }                                                 \
  } while (0)

/**
 * @brief yield coroutine
 */
#define WWS_COROUTINE_YIELD(_coroutine)                                                            \
  do {                                                                                             \
    (_coroutine) = &&WWS_LOCAL_VAR(label);                                                         \
    return;                                                                                        \
    WWS_LOCAL_VAR(label)                                                                           \
    :;                                                                                             \
  } while (0)

/**
 * @brief let routine sleep for ticks as co-routine
 */
#define WWS_COROUTINE_SLEEP(_coroutine, _timestamp, _ticks)                                        \
  do {                                                                                             \
    (_timestamp) = wws_tick_get();                                                                 \
    WWS_COROUTINE_YIELD(_coroutine);                                                               \
    if (!wws_tick_isup((_timestamp), (_ticks))) { return; }                                        \
  } while (0)

/**
 * @brief restart routine as co-routine
 */
#define WWS_COROUTINE_RESET(_coroutine)                                                            \
  do {                                                                                             \
    (_corouitine) = 0;                                                                             \
  } while (0)

/**
 * @brief set service as intervalable
 */
#define WWS_SERVICE_INTERVAL(_cd, _ticks)                                                          \
  do {                                                                                             \
    if (!wws_countdown_iscounting(_cd)) wws_countdown_recount(_cd);                                \
    if (!wws_countdown_isup(_cd, _ticks)) return;                                                  \
    wws_countdown_recount(_cd);                                                                    \
  } while (0)

#endif /* ___WWS_SERVICE_H___ */