/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_STATE_MACHINE_H___
#define ___WWS_STATE_MACHINE_H___

#include <stdbool.h>

/**
 * @brief Phase on enter to state
 */
extern const char *WWS_STATE_MACHINE_ENTER;
/**
 * @brief Phase on each run of state
 */
extern const char *WWS_STATE_MACHINE_RUN;
/**
 * @brief Phase on leave from a state
 */
extern const char *WWS_STATE_MACHINE_LEAVE;

/**
 * @brief Machine state
 */
typedef void (*wws_state_machine_state_t)(const char *phase, void *prev, void *next, void *payload);

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
   * @brief payload
   */
  void *const payload;
} wws_state_machine_t;

/**
 * @brief Change state machine to state
 * @param sm
 * @param state
 * @return is changed?
 */
extern bool wws_state_machine_change_state(wws_state_machine_t       *sm,
                                           wws_state_machine_state_t *state);

/**
 * @brief Run state machine
 * @param sm
 */
extern void wws_state_machine_run(wws_state_machine_t *sm);

#endif /* ___WWS_STATE_MACHINE_H___ */
