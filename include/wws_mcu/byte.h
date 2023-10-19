/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_BYTE_H___
#define ___WWS_BYTE_H___

#include "typedef.h"

/** rets */
extern wws_ret_t WWS_RET_OK;
extern wws_ret_t WWS_RET_ERR_OTHER;

/**
 * @brief Interface of byte
 */
typedef struct __wws_byte_inf_t
{
  /**
   * @brief get byte
   */
  wws_ret_t (*const get)(void *inst, char *buf);
  /**
   * @brief read bytes
   */
  wws_ret_t (*const read)(void *inst, unsigned int size, char *buf, unsigned int *buffered);
  /**
   * @brief put byte
   */
  wws_ret_t (*const put)(void *inst, char byte);
  /**
   * @brief write bulk of bytes
   */
  wws_ret_t (*const write)(void *inst, const char *bytes, unsigned int len, unsigned int *written);
} wws_byte_inf_t;


/**
 * @brief byte inst
 */
typedef struct __wws_byte_t
{
  wws_byte_inf_t *const interface;
  /**
   * @brief inst
   */
  void *const inst;
} wws_byte_t;

/**
 * @brief get byte in safe
 * @param b
 * @param buf
 */
extern wws_ret_t wws_byte_get(wws_byte_t *b, char *buf);

/**
 * @brief read bytes
 * @param b
 * @param size
 * @param buf
 */
extern wws_ret_t wws_byte_read(wws_byte_t *b, unsigned int size, char *buf, unsigned int *buffered);

/**
 * @brief put byte in safe
 * @param b
 * @param byte
 */
extern wws_ret_t wws_byte_put(wws_byte_t *b, char byte);

/**
 * @brief write bytes
 * @param b
 * @param bytes
 * @param len
 */
extern wws_ret_t
wws_byte_write(wws_byte_t *b, const char *bytes, unsigned int len, unsigned int *written);

/**
 * @brief write cstr
 * @param b
 * @param cstr
 */
static inline wws_ret_t wws_byte_write_cstr(wws_byte_t *b, const wws_cstr_t *cstr)
{
  return wws_byte_write(b, cstr->str, cstr->len, 0);
}

/**
 * @brief write string
 * @param b
 * @param str
 * @return length of written
 */
extern wws_ret_t wws_byte_write_str(wws_byte_t *b, const char *str);

/**
 * @brief repeat put byte for times
 * @param b
 * @param byte
 * @param times
 * @return length of put
 */
extern wws_ret_t
wws_byte_put_repeat(wws_byte_t *b, char byte, unsigned int times, unsigned int *written);

/**
 * @brief reset rx buffer
 * @param b
 */
extern void wws_byte_rx_reset(wws_byte_t *b);

#endif /* ___WWS_BYTE_H___ */
