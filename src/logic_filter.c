/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/logic_filter.h>
#include <wws_mcu/time.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/compiler.h>

wws_comp_t         WWS_COMP_LOGIC_FILTER = "LogicFilter";
WWS_WEAK wws_evt_t WWS_EVT_CHANGE        = "CHANGE";

void ___wws_logic_filter_service_callback(wws_phase_t on, wws_service_t *serv)
{
  wws_logic_filter_t *lf = serv->inst;
  if (on == WWS_ON_START) { lf->logic = wws_logic_read(lf->raw); }
  else if (on == WWS_ON_ROUTINE) {
    const unsigned char cur = wws_logic_read(lf->raw);

    if (cur == lf->logic) {
      lf->timestamp = wws_tick_get();
      return;
    }

    do {
      if ((cur == WWS_HIGH) && wws_tick_isup(lf->timestamp, lf->rising)) { break; }
      if ((cur == WWS_LOW) && wws_tick_isup(lf->timestamp, lf->falling)) { break; }
      return;
    } while (0);

    wws_event(WWS_COMP_LOGIC_FILTER, WWS_EVT_CHANGE, lf, (void *) cur);

    lf->logic = cur;
  }
}
