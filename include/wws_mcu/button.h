/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_BUTTON_H___
#define ___WWS_BUTTON_H___

#include <stdbool.h>

#include "typedef.h"
#include "logic.h"
#include "service.h"
#include "time.h"

extern wws_comp_t WWS_COMP_BUTTON;
extern wws_comp_t WWS_COMP_BTN_CLICKS;
extern wws_comp_t WWS_COMP_BTN_REPEAT;
extern wws_evt_t  WWS_EVT_RESET;
extern wws_evt_t  WWS_EVT_LOCK;
extern wws_evt_t  WWS_EVT_UNLOCK;
extern wws_evt_t  WWS_EVT_PRESSED;
extern wws_evt_t  WWS_EVT_RELEASED;
extern wws_evt_t  WWS_EVT_START;
extern wws_evt_t  WWS_EVT_COUNT;
extern wws_evt_t  WWS_EVT_DONE;

/**
 * @brief Button
 */
typedef struct __wws_button_t
{
  /**
   * @brief active logic
   */
  const unsigned char active;
  /**
   * @brief button logic input
   */
  wws_logic_reader_t *const input;
  /**
   * @brief timestamp for recent event
   * - PRESSED
   * - RELEASED
   */
  unsigned int timestamp;
  /**
   * @brief flags
   */
  union
  {
    unsigned int flags;
    struct
    {
      unsigned int lock : 1;
      unsigned int pressed : 1;
      unsigned int released : 1;
    };
  };
} wws_button_t;

/**
 * @brief reset button
 * @param button
 */
extern void wws_button_reset(wws_button_t *button);

/**
 * @brief lock button until deactive
 * @param button
 */
extern void wws_button_lock(wws_button_t *button);

/**
 * @brief Manually unlock button
 * @param button
 */
extern void wws_button_unlock(wws_button_t *button);

/**
 * @brief Is holding for time
 * @param button
 * @param ticks
 * @return is still holding and reach ticks
 */
static inline bool wws_button_hold_for(wws_button_t *button, unsigned int ticks)
{
  return button->pressed && wws_tick_isup(button->timestamp, ticks);
}

/**
 * @brief Is just releasd after ticks
 * @param button
 * @param ticks
 * @return just released and reach ticks
 */
static inline bool wws_button_released_after(wws_button_t *button, unsigned ticks)
{
  return button->released && wws_tick_isup(button->timestamp, ticks);
}

extern void ___wws_button_service_callback(wws_phase_t on, wws_service_t *serv);

/**
 * @brief service for button
 */
#define WWS_BUTTON_SERVICE .callback = ___wws_button_service_callback, .default_start = 1

/**
 * @brief count clicks
 */
typedef struct __wws_btn_clicks_t
{
  /**
   * @brief button source
   */
  wws_button_t *const button;
  /**
   * @brief timeout of clicks
   */
  unsigned int timeout;
  /**
   * @brief count of clicks
   */
  unsigned int count : 8;
  /**
   * @brief flags
   */
  unsigned int counting : 1;
  unsigned int counted : 1;
} wws_btn_clicks_t;

/**
 * @brief get clicks
 * @param clicks
 * @return unsigned char count
 */
static inline unsigned int wws_btn_clicks_count(wws_btn_clicks_t *clicks)
{
  return clicks->counted ? clicks->count : 0;
}

extern void ___wws_btn_clicks_service_callback(wws_phase_t on, wws_service_t *serv);

#define WWS_BTN_CLICKS_SERVICE .callback = ___wws_btn_clicks_service_callback, .default_start = 1

/**
 * @brief repeat of button
 */
typedef struct __wws_btn_repeat_t
{
  /**
   * @brief button source
   */
  wws_button_t *const button;
  /**
   * @brief last count timestamp
   */
  unsigned int timestamp;
  /**
   * @brief repeat count
   */
  unsigned int count;
  /**
   * @brief Threshold table
   */
  struct wws_btn_repeat_threshold_t
  {
    /**
     * @brief End of -1 as external
     */
    int threshold;
    /**
     * @brief repeat interval ticks
     */
    unsigned int interval;
  } *const table;
} wws_btn_repeat_t;

/**
 * @brief Try to repeat
 * @param repeat
 * @return unsigned int 0: no repeat, otherwise: repeat count
 */
extern unsigned int wws_btn_repeat_try(wws_btn_repeat_t *repeat);


#endif /* ___WWS_BUTTON_H___ */
