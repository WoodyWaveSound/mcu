/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/button.h>
#include <wws_mcu/debug.h>

wws_comp_t         WWS_COMP_BUTTON     = "Button";
wws_comp_t         WWS_COMP_BTN_CLICKS = "BtnClick";
wws_comp_t         WWS_COMP_BTN_REPEAT = "BtnRepeat";
WWS_WEAK wws_evt_t WWS_EVT_RESET       = "Reset";
WWS_WEAK wws_evt_t WWS_EVT_LOCK        = "Lock";
WWS_WEAK wws_evt_t WWS_EVT_UNLOCK      = "Unlock";
WWS_WEAK wws_evt_t WWS_EVT_PRESSED     = "Pressed";
WWS_WEAK wws_evt_t WWS_EVT_RELEASED    = "Released";
WWS_WEAK wws_evt_t WWS_EVT_START       = "Start";
WWS_WEAK wws_evt_t WWS_EVT_COUNT       = "Count";
WWS_WEAK wws_evt_t WWS_EVT_DONE        = "Done";


void wws_button_reset(wws_button_t *button)
{
  wws_assert(button);
  button->flags = 0;
  wws_event(WWS_COMP_BUTTON, WWS_EVT_RESET, button);
}

void wws_button_lock(wws_button_t *button)
{
  wws_assert(button);
  button->lock = 1;
  wws_event(WWS_COMP_BUTTON, WWS_EVT_LOCK, button);
}

void wws_button_unlock(wws_button_t *button)
{
  wws_assert(button);
  button->lock = 0;
  wws_event(WWS_COMP_BUTTON, WWS_EVT_UNLOCK, button);
}

void ___wws_button_service_callback(wws_phase_t on, wws_service_t *serv)
{
  wws_button_t *button = serv->inst;
  if (on == WWS_ON_START) { wws_button_lock(button); }
  else if (on == WWS_ON_ROUTINE) {
    const unsigned char input        = wws_logic_read(button->input);
    const bool          is_activated = (input == button->active);

    if (button->lock) {
      if (is_activated) return; /** still locked */
      wws_button_reset(button);
    }

    if (is_activated) {
      /** just pressed? */
      if (!button->pressed) {
        button->pressed   = 1;
        button->timestamp = wws_tick_get();
        wws_event(WWS_COMP_BUTTON, WWS_EVT_PRESSED, button);
      }
    }
    else {
      /** just released? */
      if (!button->released) {
        button->pressed   = 0;
        button->released  = 1;
        button->timestamp = wws_tick_get();
        wws_event(WWS_COMP_BUTTON, WWS_EVT_RELEASED, button);
      }
      else if (button->released) {
        wws_button_reset(button);
      }
    }
  }
}

void ___wws_btn_clicks_service_callback(wws_phase_t on, wws_service_t *serv)
{
  if (on == WWS_ON_ROUTINE) {
    wws_btn_clicks_t *clicks = serv->inst;

    if (clicks->counted) {
      clicks->counted  = 0;
      clicks->counting = 0;
      wws_event(WWS_COMP_BTN_CLICKS, WWS_EVT_RESET, clicks);
    }
    else if (clicks->button->released) {
      if (clicks->counting) {
        clicks->count++;
        wws_event(WWS_COMP_BTN_CLICKS, WWS_EVT_COUNT, clicks);
      }
      else {
        clicks->counted  = 0;
        clicks->counting = 1;
        clicks->count    = 1;
        wws_event(WWS_COMP_BTN_CLICKS, WWS_EVT_START, clicks);
      }
    }
    else if (!clicks->button->pressed) {
      if (wws_tick_isup(clicks->button->timestamp, clicks->timeout)) {
        clicks->counted = 1;
        wws_event(WWS_COMP_BTN_CLICKS, WWS_EVT_DONE, clicks);
      }
    }
  }
}

unsigned int wws_btn_repeat_try(wws_btn_repeat_t *repeat)
{
  wws_assert(repeat);

  /** not in pressed */
  if (!repeat->button->pressed) {
    repeat->count = 0;
    return 0;
  }

  unsigned int timestamp = repeat->timestamp;
  if (repeat->count == 0) { timestamp = wws_tick_get(); }
  unsigned int hold_for = timestamp - repeat->button->timestamp;

  struct wws_btn_repeat_threshold_t *threshold = &(repeat->table[0]);
  while ((threshold->threshold > 0) && (repeat->count >= threshold->threshold)) {
    threshold++;
  }


  if (hold_for >= threshold->interval) {
    repeat->count++;
    wws_event(WWS_COMP_BTN_REPEAT, WWS_EVT_COUNT, repeat, threshold);
    return repeat->count;
  }

  return 0;
}
