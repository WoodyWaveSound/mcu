/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_LOGIC_H___
#define ___WWS_LOGIC_H___

/**
 * @brief Logic = high
 */
#define WWS_HIGH (1)
/**
 * @brief Logic = low
 */
#define WWS_LOW (0)

/**
 * @brief logic reader
 */
typedef struct __wws_logic_reader_t
{
  /**
   * @brief read function
   */
  unsigned char (*read)(void *payload);
  /**
   * @brief payload for instance or cached
   */
  void *payload;
} wws_logic_reader_t;

/**
 * @brief logic writer
 */
typedef struct __wws_logic_writer_t
{
  /**
   * @brief write function
   */
  void (*write)(unsigned char logic, void *payload);
  /**
   * @brief payload for instance or cached
   */
  void *payload;
} wws_logic_writer_t;

/**
 * @brief read logic from reader
 * @param reader
 * @return unsigned char as logic
 */
static inline unsigned char wws_logic_read(wws_logic_reader_t *reader)
{
  return reader->read == 0 ? ((struct { unsigned char logic; } *) (reader->payload))->logic :
                             reader->read(reader->payload);
}

/**
 * @brief write logic to writer
 * @param writer
 * @param logic logic to write
 */
static inline void wws_logic_write(wws_logic_writer_t *writer, unsigned char logic)
{
  writer->write(logic, writer->payload);
}

#endif /* ___WWS_LOGIC_H___ */
