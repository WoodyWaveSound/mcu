/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/button.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/compiler.h>

const char          *WWS_COMP_BUTTON     = "Button";
const char          *WWS_COMP_BTN_CLICKS = "BtnClick";
const char          *WWS_COMP_BTN_REPEAT = "BtnRepeat";
WWS_WEAK const char *WWS_EVT_RESET       = "Reset";
WWS_WEAK const char *WWS_EVT_LOCK        = "Lock";
WWS_WEAK const char *WWS_EVT_UNLOCK      = "Unlock";
WWS_WEAK const char *WWS_EVT_PRESSED     = "Pressed";
WWS_WEAK const char *WWS_EVT_RELEASED    = "Released";
WWS_WEAK const char *WWS_EVT_START       = "Start";
WWS_WEAK const char *WWS_EVT_COUNT       = "Count";
WWS_WEAK const char *WWS_EVT_DONE        = "Done";


void wws_button_reset(wws_button_t *button)
{
  wws_assert(button);
  button->flag = 0;
  wws_event(WWS_COMP_BUTTON, WWS_EVT_RESET, button);
}

void wws_button_reset_list(wws_button_t *const *const buttons)
{
  wws_assert(buttons && buttons[0]);
  for (wws_button_t *b = buttons[0]; b != 0; b++) {
    wws_button_reset(b);
  }
}

void wws_button_lock(wws_button_t *button)
{
  wws_assert(button);
  wws_bitmask_mask(button->flag, WWS_BUTTON_LOCK);
  wws_event(WWS_COMP_BUTTON, WWS_EVT_LOCK, button);
}

void wws_button_unlock(wws_button_t *button)
{
  wws_assert(button);
  wws_bitmask_unmask(button->flag, WWS_BUTTON_LOCK);
  wws_event(WWS_COMP_BUTTON, WWS_EVT_UNLOCK, button);
}


void wws_button_update(wws_button_t *button)
{
  wws_assert(button);

  const unsigned char input        = wws_logic_read(button->input);
  const bool          is_activated = (input == button->active);

  if (wws_bitmask_any(button->flag, WWS_BUTTON_LOCK)) {
    if (is_activated) return; /** still locked */
    wws_button_reset(button);
  }

  if (is_activated) {
    /** just pressed? */
    if (wws_bitmask_none(button->flag, WWS_BUTTON_PRESSED)) {
      wws_bitmask_mask(button->flag, WWS_BUTTON_PRESSED);
      button->timestamp = wws_tick_get();
      wws_event(WWS_COMP_BUTTON, WWS_EVT_PRESSED, button);
    }
  }
  else {
    /** just released? */
    if (wws_bitmask_any(button->flag, WWS_BUTTON_PRESSED)) {
      wws_bitmask_unmask(button->flag, WWS_BUTTON_PRESSED);
      wws_bitmask_mask(button->flag, WWS_BUTTON_RELEASED);
      button->timestamp = wws_tick_get();
      wws_event(WWS_COMP_BUTTON, WWS_EVT_RELEASED, button);
    }
    else if (wws_bitmask_any(button->flag, WWS_BUTTON_RELEASED)) {
      wws_button_reset(button);
    }
  }
}

void wws_button_update_list(wws_button_t *const *const buttons)
{
  wws_assert(buttons && buttons[0]);
  for (wws_button_t *b = buttons[0]; b != 0; b++) {
    wws_button_update(b);
  }
}

void wws_btn_clicks_update(wws_btn_clicks_t *clicks)
{
  if (wws_bitmask_any(clicks->flag, WWS_BTN_CLICKS_COUNTED)) {
    wws_bitmask_unmask(clicks->flag, WWS_BTN_CLICKS_COUNTING | WWS_BTN_CLICKS_COUNTED);
    wws_event(WWS_COMP_BTN_CLICKS, WWS_EVT_RESET, clicks);
  }
  else if (wws_bitmask_any(clicks->button->flag, WWS_BUTTON_RELEASED)) {
    if (wws_bitmask_any(clicks->flag, WWS_BTN_CLICKS_COUNTING)) {
      clicks->count++;
      wws_event(WWS_COMP_BTN_CLICKS, WWS_EVT_COUNT, clicks);
    }
    else {
      wws_bitmask_unmask(clicks->flag, WWS_BTN_CLICKS_COUNTED);
      wws_bitmask_mask(clicks->flag, WWS_BTN_CLICKS_COUNTING);
      clicks->count = 1;
      wws_event(WWS_COMP_BTN_CLICKS, WWS_EVT_START, clicks);
    }
  }
  else if (wws_bitmask_none(clicks->button->flag, WWS_BUTTON_PRESSED)) {
    if (wws_tick_isup(clicks->button->timestamp, clicks->timeout)) {
      wws_bitmask_mask(clicks->flag, WWS_BTN_CLICKS_COUNTED);
      wws_event(WWS_COMP_BTN_CLICKS, WWS_EVT_DONE, clicks);
    }
  }
}

void wws_btn_clicks_update_list(wws_btn_clicks_t *const *const clicks)
{
  wws_assert(clicks && clicks[0]);
  for (wws_btn_clicks_t *c = clicks[0]; c != 0; c++) {
    wws_btn_clicks_update(c);
  }
}

unsigned int wws_btn_repeat_try_count(wws_btn_repeat_t *repeat)
{
  wws_assert(repeat);

  /** not in pressed */
  if (wws_bitmask_none(repeat->button->flag, WWS_BUTTON_PRESSED)) {
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
