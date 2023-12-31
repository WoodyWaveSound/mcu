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
  unsigned int ts = wws_tick_get();

  while ((ticks > 0) && !wws_tick_isup(ts, ticks))
    ;
}

void ___wws_tick_inc()
{
  wws_tick++;

  wws_uptime.msec++;

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
