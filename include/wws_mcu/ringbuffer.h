/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_RINGBUFFER_H___
#define ___WWS_RINGBUFFER_H___

#include "typedef.h"

/**
 * @brief create ringbuffer
 * @param _length must be powered of 2
 */
#define WWS_CREATE_RINGBUFFER(_name, _type, _length)                                               \
  struct                                                                                           \
  {                                                                                                \
    _type                   buffer[((_length) & ((_length) -1)) == 0 ? _length : -1];              \
    volatile unsigned short read_cur, write_cur;                                                   \
    const unsigned short    mask;                                                                  \
  } _name = { .buffer = { 0 }, .mask = (_length) -1 }

/**
 * @brief get size of ringbuffer
 * @return unsigned short
 */
#define wws_ringbuffer_size(_rb) (_rb)->mask

/**
 * @brief reset ringbuffer
 */
#define wws_ringbuffer_reset(_rb)                                                                  \
  do {                                                                                             \
    (_rb)->read_cur  = 0;                                                                          \
    (_rb)->write_cur = 0;                                                                          \
  } while (0)

/**
 * @brief get buffered length
 * @return unsigned short
 */
#define wws_ringbuffer_get_buffered(_rb) (((_rb)->write_cur - (_rb)->read_cur) & (_rb)->mask)

/**
 * @brief get available space
 * @return unsigned short
 */
#define wws_ringbuffer_get_available(_rb) ((_rb)->mask - wws_ringbuffer_get_buffered(_rb))

/**
 * @brief is ringbuffer full?
 */
#define wws_ringbuffer_is_full(_rb) (wws_ringbuffer_get_buffered(_rb) == (_rb)->mask)

/**
 * @brief is ringbuffer empty?
 */
#define wws_ringbuffer_is_empty(_rb) (wws_ringbuffer_get_buffered(_rb) == 0)

/**
 * @brief push data to ringbuffer
 *
 * @warning if buffer full, will override and destory buffer
 */
#define wws_ringbuffer_push(_rb, _data)                                                            \
  do {                                                                                             \
    (_rb)->buffer[(_rb)->write_cur++ & (_rb)->mask] = _data;                                       \
  } while (0)

/**
 * @brief push data to ringbuffer safe
 * @return 1: ok, 0: buffer full
 */
#define wws_ringbuffer_push_safe(_rb, _data)                                                       \
  ({                                                                                               \
    int ok = 0;                                                                                    \
    if (!wws_ringbuffer_is_full(_rb)) {                                                            \
      wws_ringbuffer_push(_rb, _data);                                                             \
      ok = 1;                                                                                      \
    }                                                                                              \
    (ok);                                                                                          \
  })

/**
 * @brief pop data from ringbuffer
 * @return data
 *
 * @warning if buffer empty, will destory buffer
 */
#define wws_ringbuffer_pop(_rb) ((_rb)->buffer[(_rb)->read_cur++ & (_rb)->mask])

/**
 * @brief pop data from ringbuffer to _buf in safer
 * @return 1: ok, 0: buffer empty
 */
#define wws_ringbuffer_pop_safe(_rb, _buf)                                                         \
  ({                                                                                               \
    int ok = 0;                                                                                    \
    if (!wws_ringbuffer_is_empty(_rb)) {                                                           \
      *(_buf) = wws_ringbuffer_pop(_rb);                                                           \
      ok      = 1;                                                                                 \
    }                                                                                              \
    (ok);                                                                                          \
  })

#endif /* ___WWS_RINGBUFFER_H___ */
