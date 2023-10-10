/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/time.h>

volatile unsigned int wws_tick = 0;

struct __wws_uptime_t wws_uptime = { 0 };

void wws_delay(unsigned int ticks)
{
  unsigned int ts = wws_get_tick();

  while ((ticks > 0) && !wws_is_tickup(ts, ticks))
    ;
}

void ___wws_tick_inc()
{
  wws_tick++;

#if WWS_CONFIG_TICK_PER_US == 1000
  wws_uptime.msec++;
#else
  static unsigned int cnt = 0;
  cnt++;
  if (cnt == (1000 / WWS_CONFIG_TICK_PER_US)) {
    cnt = 0;
    wws_uptime.msec++;
  }
#endif

  if (wws_uptime.msec == 1000) {
    wws_uptime.msec = 0;
    wws_uptime.sec++;

    if (wws_uptime.sec == 60) {
      wws_uptime.sec = 0;
      wws_uptime.min++;

      if (wws_uptime.min == 60) {
        wws_uptime.min = 0;
        wws_uptime.hour++;

        if (wws_uptime.hour == 24) {
          wws_uptime.hour = 0;
          wws_uptime.day++;
        }
      }
    }
  }
}