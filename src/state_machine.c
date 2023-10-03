/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/state_machine.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/compiler.h>

const char          *WWS_COMP_STATE_MACHINE  = "StateMachine";
WWS_WEAK const char *WWS_EVT_CHANGE          = "Change";
WWS_WEAK const char *WWS_EVT_ENTER           = "Enter";
WWS_WEAK const char *WWS_EVT_RUN             = "Run";
WWS_WEAK const char *WWS_EVT_LEAVE           = "Leave";
const char          *WWS_STATE_MACHINE_ENTER = "ENTER";
const char          *WWS_STATE_MACHINE_RUN   = "RUN";
const char          *WWS_STATE_MACHINE_LEAVE = "LEAVE";

bool wws_state_machine_change_state(wws_state_machine_t *sm, wws_state_machine_state_t *state)
{
  wws_assert(sm);
  if (sm->state == state) { return false; }

  wws_event(WWS_COMP_STATE_MACHINE, WWS_EVT_CHANGE, sm);

  /** leave old */
  if (sm->state) {
    wws_event(WWS_COMP_STATE_MACHINE, WWS_EVT_LEAVE, sm, sm->state);
    sm->state(WWS_STATE_MACHINE_LEAVE, sm->state, state, sm->payload);
  }

  /** start new */
  if (state) {
    wws_event(WWS_COMP_STATE_MACHINE, WWS_EVT_ENTER, sm, state);
    sm->state(WWS_STATE_MACHINE_ENTER, sm->state, state, sm->payload);
  }

  sm->state = state;
  return true;
}

void wws_state_machine_run(wws_state_machine_t *sm)
{
  wws_assert(sm);

  if (sm->state) {
    wws_event(WWS_COMP_STATE_MACHINE, WWS_EVT_RUN, sm, sm->state);
    sm->state(WWS_STATE_MACHINE_RUN, 0, 0, sm->payload);
  }
}
