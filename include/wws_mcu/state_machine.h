/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_STATE_MACHINE_H___
#define ___WWS_STATE_MACHINE_H___

#include <stdbool.h>
#include "typedef.h"
#include "service.h"

extern wws_comp_t WWS_COMP_STATE_MACHINE;
extern wws_evt_t  WWS_EVT_CHANGE;
extern wws_evt_t  WWS_EVT_ENTER;
extern wws_evt_t  WWS_EVT_RUN;
extern wws_evt_t  WWS_EVT_LEAVE;

/**
 * @brief Phase on enter to state
 */
extern wws_phase_t WWS_ON_ENTER; // (WWS_EVT_ENTER)
/**
 * @brief Phase on each run of state
 */
extern wws_phase_t WWS_ON_RUN; // (WWS_EVT_RUN)
/**
 * @brief Phase on leave from a state
 */
extern wws_phase_t WWS_ON_LEAVE; // (WWS_EVT_LEAVE)

/**
 * @brief Machine state
 */
typedef void (*wws_state_machine_state_t)(const char *phase, void *prev, void *next, void *inst);

/**
 * @brief State Machine
 */
typedef struct __wws_state_machine_t
{
  /**
   * @brief Current state
   */
  wws_state_machine_state_t state;
  /**
   * @brief inst
   */
  void *const inst;
} wws_state_machine_t;

/**
 * @brief Change state machine to state
 * @param sm
 * @param state
 * @return is changed?
 */
extern bool wws_state_machine_change_state(wws_state_machine_t      *sm,
                                           wws_state_machine_state_t state);

/**
 * @brief Run state machine
 * @param sm
 */
extern void wws_state_machine_run(wws_state_machine_t *sm);

/**
 * @brief service callback
 * @param phase
 * @param serv
 */
extern void ___wws_state_machine_service_callback(wws_phase_t phase, wws_service_t *serv);

/**
 * @brief service of state machine
 */
#define WWS_STATE_MACHINE_SERVICE                                                                  \
  .callback = ___wws_state_machine_service_callback, .default_start = 1

#endif /* ___WWS_STATE_MACHINE_H___ */
