/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/time.h>

volatile unsigned int wws_tick = 0;

void wws_delay(unsigned int ticks)
{
  unsigned int ts = wws_get_tick();

  while ((ticks > 0) && !wws_is_tickup(ts, ticks))
    ;
}
