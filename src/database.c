/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/database.h>
#include <wws_mcu/debug.h>

wws_comp_t         WWS_COMP_DATABASE = "Database";
WWS_WEAK wws_evt_t WWS_EVT_INVALID   = "Invalid";
WWS_WEAK wws_ret_t WWS_RET_OK        = "OK";
WWS_WEAK wws_ret_t WWS_RET_REINIT    = "REINIT";


wws_ret_t wws_database_load(wws_database_t *db)
{
  wws_assert((db != 0) && (db->head != 0) && (db->tail != 0));

  /**
   * layout:
   * 0 - 4: head key
   * 5 - size: data
   * (4 + size) - (size + 8): tail key
   */
  db->size = (unsigned int) ((const char *) (db->tail) - (const char *) (db->head + 1));

  wws_ret_t ret = WWS_RET_OK;

  do {
    if ((ret = wws_memory_get32(&db->memory, 0, (int *) db->head)) != WWS_RET_OK) break;
    if ((ret = wws_memory_get32(&db->memory, 4 + db->size, (int *) db->tail)) != WWS_RET_OK) break;
    if (*(db->head) != db->key) break;
    if (*(db->tail) != ~db->key) break;
    return WWS_RET_OK;
  } while (0);

  // invalid
  wws_event(WWS_COMP_DATABASE, WWS_EVT_INVALID, db);

  if ((ret = wws_memory_put32(&db->memory, 0, (int) *db->head)) != WWS_RET_OK) return ret;
  if ((ret = wws_memory_put32(&db->memory, 4 + db->size, (int) *db->tail)) != WWS_RET_OK)
    return ret;
  if ((ret = wws_database_save(db)) != WWS_RET_OK) return ret;

  return WWS_RET_REINIT;
}

wws_ret_t wws_database_save(wws_database_t *db)
{
  wws_assert((db != 0) && (db->size > 0));

  return wws_memory_write8(&db->memory, 4, (char *) (db->head + 1), db->size, 0);
}
