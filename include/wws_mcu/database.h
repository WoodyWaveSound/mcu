/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_DATABASE_H___
#define ___WWS_DATABASE_H___

#include "compiler.h"

extern const char *WWS_COMP_DATABASE;
extern const char *WWS_EVT_INVALID;

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
} wws_database_t;


/**
 * @brief get database ptr operate as bulk data
 * @param db
 * @return void * pointer of database begin
 */
static inline void *wws_database_ptr(wws_database_t *db)
{
  return (void *) db->head;
}

/**
 * @brief get database size in bytes
 * @param db
 * @return unsigned int in bytes
 */
static inline unsigned int wws_database_size(wws_database_t *db)
{
  return (unsigned int) ((const char *) (db->tail + 1) - (const char *) (db->head));
}

/**
 * @brief valid database in memory
 * @param db
 * @return 0: valid, 1: invalid
 */
extern int wws_database_valid(wws_database_t *db);

#endif /* ___WWS_DATABASE_H___ */
