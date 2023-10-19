/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/countdown.h>
#include <wws_mcu/time.h>
#include <wws_mcu/debug.h>

wws_comp_t         WWS_COMP_COUNTDOWN = "COUNTDOWN";
WWS_WEAK wws_evt_t WWS_EVT_START      = "START";
WWS_WEAK wws_evt_t WWS_EVT_STOP       = "STOP";
WWS_WEAK wws_evt_t WWS_EVT_DONE       = "DONE";

void wws_countdown_recount(wws_countdown_t *cd)
{
  cd->timestamp = wws_tick_get();
  cd->counting  = 1;
  wws_event(WWS_COMP_COUNTDOWN, WWS_EVT_START, cd);
}

bool wws_countdown_isup(wws_countdown_t *cd, unsigned int ticks)
{
  if (cd->counting == 0) return false;
  if (!wws_tick_isup(cd->timestamp, ticks)) return false;
  /** up */
  wws_event(WWS_COMP_COUNTDOWN, WWS_EVT_DONE, cd);
  cd->counting = 0;
  return true;
}

void wws_countdown_stop(wws_countdown_t *cd)
{
  cd->counting = 0;
  wws_event(WWS_COMP_COUNTDOWN, WWS_EVT_STOP, cd);
}

bool wws_countdown_iscounting(wws_countdown_t *cd)
{
  return cd->counting == 1;
}
