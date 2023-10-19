/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/service.h>
#include <wws_mcu/compiler.h>
#include <wws_mcu/debug.h>

wws_comp_t         WWS_COMP_SERVICE = "Service";
WWS_WEAK wws_evt_t WWS_EVT_START    = "START";
WWS_WEAK wws_evt_t WWS_EVT_STOP     = "STOP";
WWS_WEAK wws_evt_t WWS_EVT_ROUTINE  = "ROUTINE";
WWS_WEAK wws_evt_t WWS_EVT_TICK     = "TICK";


extern wws_phase_t WWS_ON_START   WWS_ALIAS(WWS_EVT_START);
extern wws_phase_t WWS_ON_STOP    WWS_ALIAS(WWS_EVT_STOP);
extern wws_phase_t WWS_ON_ROUTINE WWS_ALIAS(WWS_EVT_ROUTINE);
extern wws_phase_t WWS_ON_TICK    WWS_ALIAS(WWS_EVT_TICK);


wws_service_t *wws_services = (wws_service_t *) (wws_service_t[]){ [0] = { .callback = 0 } };

wws_service_t *wws_service_by_id(unsigned char id)
{
  for (wws_service_t *s = &wws_services[0]; s->callback != 0; s++) {
    if (s->id == id) return s;
  }
  return 0;
}

void wws_service_run(const char *event)
{
  for (wws_service_t *s = &wws_services[0]; s->callback != 0; s++) {
    /** ignore tick to main tick performance */
    if ((event == WWS_ON_TICK) && s->tick_enable) continue;

    /** when routine, auto start if config as default start but not yet */
    if ((event == WWS_ON_ROUTINE) && s->default_start && !s->_default_start) {
      wws_service_start(s);
      s->_default_start = 1;
    }

    wws_event(WWS_COMP_SERVICE, event, s);
    s->callback(event, s);
  }
}

void wws_service_start(wws_service_t *serv)
{
  wws_assert(serv && serv->callback);
  if (serv->_started) return;
  wws_event(WWS_COMP_SERVICE, WWS_EVT_START, serv);
  serv->callback(WWS_ON_START, serv);
  serv->_started = 1;
}

void wws_service_stop(wws_service_t *serv)
{
  wws_assert(serv && serv->callback);
  if (!serv->_started) return;
  wws_event(WWS_COMP_SERVICE, WWS_EVT_STOP, serv);
  serv->callback(WWS_ON_STOP, serv);
  serv->_started = 0;
}