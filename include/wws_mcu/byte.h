/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_BYTE_H___
#define ___WWS_BYTE_H___

#include "typedef.h"

/**
 * @brief Interface of byte
 */
typedef struct __wws_byte_interface_t
{
  /**
   * @brief get byte
   * @note 1: ok, 0: failed
   */
  unsigned int (*const get)(void *instance, char *buf);
  /**
   * @brief read bytes
   * @return length of read
   */
  unsigned int (*const read)(void *instance, unsigned int size, char *buf);
  /**
   * @brief put byte
   * @return 1: Ok, 0: fail
   */
  unsigned int (*const put)(void *instance, char byte);
  /**
   * @brief write bulk of bytes
   * @return length of written
   */
  unsigned int (*const write)(void *instance, const char *bytes, unsigned int len);
} wws_byte_interface_t;


/**
 * @brief byte instance
 */
typedef struct __wws_byte_t
{
  wws_byte_interface_t *const interface;
  /**
   * @brief instance
   */
  void *const instance;
} wws_byte_t;

/**
 * @brief get byte in safe
 * @param b
 * @param buf
 * @return 1: get, 0: empty
 */
extern unsigned int wws_byte_get(wws_byte_t *b, char *buf);

/**
 * @brief read bytes
 * @param b
 * @param size
 * @param buf
 * @return length of read
 */
extern unsigned int wws_byte_read(wws_byte_t *b, unsigned int size, char *buf);

/**
 * @brief put byte in safe
 * @param b
 * @param byte
 * @return 1: put, 0: failed
 */
extern unsigned int wws_byte_put(wws_byte_t *b, char byte);

/**
 * @brief write bytes
 * @param b
 * @param bytes
 * @param len
 * @return length of written
 */
extern unsigned int wws_byte_write(wws_byte_t *b, const char *bytes, unsigned int len);

/**
 * @brief write cstr
 * @param b
 * @param cstr
 * @return lenght of written
 */
static inline unsigned int wws_byte_write_cstr(wws_byte_t *b, const wws_cstr_t *cstr)
{
  return wws_byte_write(b, cstr->str, cstr->len);
}

/**
 * @brief write string
 * @param b
 * @param str
 * @return length of written
 */
extern unsigned int wws_byte_write_str(wws_byte_t *b, const char *str);

/**
 * @brief repeat put byte for times
 * @param b
 * @param byte
 * @param times
 * @return length of put
 */
extern unsigned int wws_byte_put_repeat(wws_byte_t *b, char byte, unsigned int times);

/**
 * @brief reset rx buffer
 * @param b
 */
extern void wws_byte_rx_reset(wws_byte_t *b);

#endif /* ___WWS_BYTE_H___ */
