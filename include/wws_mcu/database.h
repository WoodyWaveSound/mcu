/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_DATABASE_H___
#define ___WWS_DATABASE_H___

#include "typedef.h"
#include "memory.h"

extern wws_comp_t WWS_COMP_DATABASE;
extern wws_evt_t  WWS_EVT_INVALID;
extern wws_ret_t  WWS_RET_OK;

/**
 * @brief load failed, re-init
 */
extern wws_ret_t WWS_RET_REINIT;

/**
 * @brief put data in database section named @p _name
 */
#define WWS_DB(_name) WWS_RAM("db." #_name)

/**
 * @brief database definition
 */
typedef struct __wws_database_t
{
  /**
   * @brief key as magic to validate
   */
  const unsigned int key;
  /**
   * @brief head variable
   */
  unsigned int *const head;
  /**
   * @brief tail variable
   */
  unsigned int *const tail;
  /**
   * @brief size of database
   */
  unsigned int size;
  /**
   * @brief memory interface
   */
  wws_memory_t memory;
} wws_database_t;

/**
 * @brief load database from memory region
 * @param db
 * @return
 */
extern wws_ret_t wws_database_load(wws_database_t *db);

/**
 * @brief save database to memory region
 * @param db
 * @return
 */
extern wws_ret_t wws_database_save(wws_database_t *db);

#endif /* ___WWS_DATABASE_H___ */
