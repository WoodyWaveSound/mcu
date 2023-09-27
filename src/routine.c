/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/routine.h>
#include <wws_mcu/bitmask.h>
#include <wws_mcu/debug.h>

#define F_ENABLE  (1U << 0)
#define F_RUNNING (1U << 1)

bool wws_routine_start(wws_routine_t *routine)
{
  wws_assert(routine != 0);
  if (wws_bitmask_any(routine->flags, F_ENABLE)) return false;

  wws_bitmask_mask(routine->flags, F_ENABLE | F_RUNNING);
  if (routine->on_start) routine->on_start(routine);

  return true;
}

bool wws_routine_stop(wws_routine_t *routine)
{
  wws_assert(routine != 0);
  if (wws_bitmask_none(routine->flags, F_ENABLE)) return false;

  wws_bitmask_unmask(routine->flags, F_ENABLE | F_RUNNING);

  return true;
}

bool wws_routine_resume(wws_routine_t *routine)
{
  wws_assert(routine != 0);
  if (wws_bitmask_any(routine->flags, F_RUNNING)) return false;

  wws_bitmask_mask(routine->flags, F_RUNNING);

  return true;
}

bool wws_routine_pause(wws_routine_t *routine)
{
  wws_assert(routine != 0);
  if (wws_bitmask_none(routine->flags, F_RUNNING)) return false;

  wws_bitmask_unmask(routine->flags, F_RUNNING);

  return true;
}

bool wws_routine_run(wws_routine_t *routine)
{
  wws_assert(routine != 0);
  if (wws_bitmask_none(routine->flags, F_RUNNING | F_ENABLE)) return false;
  if (routine->callback == 0) return false;
  routine->callback(routine);
  return true;
}
