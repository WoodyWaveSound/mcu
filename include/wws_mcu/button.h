/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_BUTTON_H___
#define ___WWS_BUTTON_H___

#include <stdbool.h>

#include "logic.h"
#include "bitmask.h"
#include "time.h"

extern const char *WWS_COMP_BUTTON;
extern const char *WWS_COMP_BTN_CLICKS;
extern const char *WWS_COMP_BTN_REPEAT;
extern const char *WWS_EVT_RESET;
extern const char *WWS_EVT_LOCK;
extern const char *WWS_EVT_UNLOCK;
extern const char *WWS_EVT_PRESSED;
extern const char *WWS_EVT_RELEASED;
extern const char *WWS_EVT_START;
extern const char *WWS_EVT_COUNT;
extern const char *WWS_EVT_DONE;

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
   * @brief flag
   */
  unsigned char flag;
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
} wws_button_t;

/**
 * @brief button lock flag to avoid strange trigger
 */
#define WWS_BUTTON_LOCK (1U << 0)
/**
 * @brief button is pressed
 */
#define WWS_BUTTON_PRESSED (1U << 1)
/**
 * @brief button is just released
 * @warning only keep for 1 cycle
 */
#define WWS_BUTTON_RELEASED (1U << 2)

/**
 * @brief reset button
 * @param button
 */
extern void wws_button_reset(wws_button_t *button);

/**
 * @brief reset list of buttons
 * @param buttons
 */
extern void wws_button_reset_list(wws_button_t *const *const buttons);

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
  return (wws_bitmask_any(button->flag, WWS_BUTTON_PRESSED) &&
          wws_bitmask_none(button->flag, WWS_BUTTON_RELEASED) &&
          wws_is_tickup(button->timestamp, ticks));
}

/**
 * @brief Is just releasd after ticks
 * @param button
 * @param ticks
 * @return just released and reach ticks
 */
static inline bool wws_button_released_after(wws_button_t *button, unsigned ticks)
{
  return (wws_bitmask_any(button->flag, WWS_BUTTON_RELEASED) &&
          wws_is_tickup(button->timestamp, ticks));
}

/**
 * @brief update button for change
 * @param button
 */
extern void wws_button_update(wws_button_t *button);

/**
 * @brief update list of buttons
 * @param buttons list end of 0
 */
extern void wws_button_update_list(wws_button_t *const *const buttons);

/**
 * @brief flag for clicks counting
 */
#define WWS_BTN_CLICKS_COUNTING (1U << 0)
/**
 * @brief flag for clicks counted
 */
#define WWS_BTN_CLICKS_COUNTED (1U << 1)

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
  unsigned char count;
  /**
   * @brief flag
   */
  unsigned char flag;
} wws_btn_clicks_t;

/**
 * @brief get clicks
 * @param clicks
 * @return unsigned char count
 */
static inline unsigned int wws_btn_clicks_count(wws_btn_clicks_t *clicks)
{
  return wws_bitmask_any(clicks->flag, WWS_BTN_CLICKS_COUNTED) ? clicks->count : 0;
}

/**
 * @brief update clicks
 * @param clicks
 */
extern void wws_btn_clicks_update(wws_btn_clicks_t *clicks);

/**
 * @brief update list of clicks
 * @param clicks end of 0
 */
extern void wws_btn_clicks_update_list(wws_btn_clicks_t *const *const clicks);

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
extern unsigned int wws_btn_repeat_try_count(wws_btn_repeat_t *repeat);


#endif /* ___WWS_BUTTON_H___ */
