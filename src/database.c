/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/database.h>
#include <wws_mcu/debug.h>

const char          *WWS_COMP_DATABASE = "Database";
WWS_WEAK const char *WWS_EVT_INVALID   = "Invalid";


int wws_database_valid(wws_database_t *db)
{
  wws_assert((db != 0) && (db->head != 0) && (db->tail != 0));
  do {
    if ((*(db->head) | *(db->tail)) != 0xFFFFFFFF) break;
    if (*(db->head) != db->key) break;
    if (*(db->tail) != ~db->key) break;
    return 0;
  } while (0);
  wws_event(WWS_COMP_DATABASE, WWS_EVT_INVALID, db);

  *(db->head) = db->key;
  *(db->tail) = ~db->key;

  return 1;
}
