/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_LOGIC_H___
#define ___WWS_LOGIC_H___

#include "typedef.h"

/**
 * @brief logic
 */
typedef enum WWS_PACKED __wws_logic_t
{
  /**
   * @brief Logic = low
   */
  WWS_LOW,
  /**
   * @brief Logic = high
   */
  WWS_HIGH,
} wws_logic_t;


/**
 * @brief logic reader
 */
typedef struct __wws_logic_reader_t
{
  /**
   * @brief read function
   */
  wws_logic_t (*read)(void *inst);
  /**
   * @brief inst for inst or cached
   */
  void *const inst;
} wws_logic_reader_t;

/**
 * @brief logic writer
 */
typedef struct __wws_logic_writer_t
{
  /**
   * @brief write function
   */
  void (*write)(wws_logic_t logic, void *inst);
  /**
   * @brief inst for inst or cached
   */
  void *const inst;
} wws_logic_writer_t;

/**
 * @brief read logic from reader
 * @param reader
 * @return wws_logic_t as logic
 */
static inline wws_logic_t wws_logic_read(wws_logic_reader_t *reader)
{
  return reader->read == 0 ? ((struct { wws_logic_t logic; } *) (reader->inst))->logic :
                             reader->read(reader->inst);
}

/**
 * @brief write logic to writer
 * @param writer
 * @param logic logic to write
 */
static inline void wws_logic_write(wws_logic_writer_t *writer, wws_logic_t logic)
{
  writer->write(logic, writer->inst);
}

#endif /* ___WWS_LOGIC_H___ */
