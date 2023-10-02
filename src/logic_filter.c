/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/logic_filter.h>
#include <wws_mcu/time.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/compiler.h>

const char          *WWS_COMP_LOGIC_FILTER = "LogicFilter";
WWS_WEAK const char *WWS_EVT_CHANGE        = "Change";

void wws_logic_filter_update(wws_logic_reader_t *reader)
{
  wws_assert((reader != 0) && (reader->payload != 0));

  wws_logic_filter_t *const filter = reader->payload;

  const unsigned char cur = wws_logic_read(filter->raw);

  if (cur == filter->logic) {
    filter->timestamp = wws_get_tick();
    return;
  }

  do {
    if ((cur == WWS_HIGH) && wws_is_tickup(filter->timestamp, filter->rising)) { break; }
    if ((cur == WWS_LOW) && wws_is_tickup(filter->timestamp, filter->falling)) { break; }
    return;
  } while (0);

  wws_event(WWS_COMP_LOGIC_FILTER, WWS_EVT_CHANGE, reader, (void *) cur);

  filter->logic = cur;
}


void wws_logic_filter_update_list(wws_logic_reader_t *const *const list)
{
  wws_assert(list != 0);
  for (wws_logic_reader_t *reader = list[0]; reader != 0; reader++) {
    wws_logic_filter_update(reader);
  }
}
