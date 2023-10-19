/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/state_machine.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/compiler.h>

wws_comp_t         WWS_COMP_STATE_MACHINE = "StateMachine";
WWS_WEAK wws_evt_t WWS_EVT_CHANGE         = "CHANGE";
WWS_WEAK wws_evt_t WWS_EVT_ENTER          = "ENTER";
WWS_WEAK wws_evt_t WWS_EVT_RUN            = "RUN";
WWS_WEAK wws_evt_t WWS_EVT_LEAVE          = "LEAVE";

extern wws_phase_t WWS_ON_ENTER WWS_ALIAS(WWS_EVT_ENTER);
extern wws_phase_t WWS_ON_RUN   WWS_ALIAS(WWS_EVT_RUN);
extern wws_phase_t WWS_ON_LEAVE WWS_ALIAS(WWS_EVT_LEAVE);


bool wws_state_machine_change_state(wws_state_machine_t *sm, wws_state_machine_state_t state)
{
  wws_assert(sm);
  if (sm->state == state) { return false; }

  wws_event(WWS_COMP_STATE_MACHINE, WWS_EVT_CHANGE, sm);

  /** leave old */
  if (sm->state) {
    wws_event(WWS_COMP_STATE_MACHINE, WWS_EVT_LEAVE, sm, sm->state);
    sm->state(WWS_ON_LEAVE, sm->state, state, sm->inst);
  }

  /** start new */
  if (state) {
    wws_event(WWS_COMP_STATE_MACHINE, WWS_EVT_ENTER, sm, state);
    sm->state(WWS_ON_ENTER, sm->state, state, sm->inst);
  }

  sm->state = state;
  return true;
}

void wws_state_machine_run(wws_state_machine_t *sm)
{
  wws_assert(sm);

  if (sm->state) {
    wws_event(WWS_COMP_STATE_MACHINE, WWS_EVT_RUN, sm, sm->state);
    sm->state(WWS_ON_RUN, 0, 0, sm->inst);
  }
}

void ___wws_state_machine_service_callback(wws_phase_t phase, wws_service_t *serv)
{
  if (phase == WWS_ON_RUN) { wws_state_machine_run(serv->inst); }
}
