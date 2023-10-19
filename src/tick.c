/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/tick.h>

wws_comp_t WWS_COMP_TICK = "Tick";

wws_tick_callback_t ___wws_tick_callback = 0;
