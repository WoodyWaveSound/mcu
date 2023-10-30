/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_TYPEDEF_H___
#define ___WWS_TYPEDEF_H___

#include "compiler.h"

/**
 * @brief compiled-time const string with length
 */
typedef struct __wws_cstr_t
{
  /**
   * @brief string
   */
  const char *const str;
  /**
   * @brief length
   */
  const unsigned int len;
} wws_cstr_t;

/**
 * @brief Generate wws_cstr_t
 * @param _str string
 */
#define wws_new_cstr(_str) wws_new_cstr_with_len(_str, sizeof(_str) - 1)

/**
 * @brief Generate wws_cstr_t
 * @param _str
 * @param _len
 */
#define wws_new_cstr_with_len(_str, _len)                                                          \
  (wws_cstr_t)                                                                                     \
  {                                                                                                \
    .str = (_str), .len = (_len)                                                                   \
  }


/**
 * @brief generate local variable by line number
 * @param ... additional name
 */
#define WWS_LOCAL_VAR(...) WWS_LINE_VAR(___wws_local, ##__VA_ARGS__)

/**
 * @brief generate variable by line number
 * @param _var prefix of line var
 * @param ... additional name
 */
#define WWS_LINE_VAR(_var, ...) ___WWS_LINE_VAR_1(_var, __LINE__, ##__VA_ARGS__)

#define ___WWS_LINE_VAR_1(_var, _line, ...) ___WWS_LINE_VAR_2(_var, _line, ##__VA_ARGS__)
#define ___WWS_LINE_VAR_2(_var, _line, ...) _var##_L##_line##_##__VA_ARGS__

/**
 * @brief get override value (last one)
 * @param _type type of value to be override
 * @param _default default value if nothing overrided
 * @param ... nums of value to be override
 * @return _type last element to override
 */
#define WWS_OVERRIDE(_type, _default, ...)                                                         \
  ({                                                                                               \
    _type override[] = { _default, ##__VA_ARGS__ };                                                \
    (override[(sizeof(override) / sizeof(override[0])) - 1]);                                      \
  })

/**
 * @brief create buffer
 * @param _type type of element
 * @param _size size of buffer
 */
#define WWS_NEW_BUFFER(_type, _size)                                                               \
  (_type[_size])                                                                                   \
  {                                                                                                \
    [(_size) -1] = 0                                                                               \
  }

/**
 * @brief component identifer
 */
typedef const char *const wws_comp_t;

/**
 * @brief event identifier
 */
typedef const char *const wws_evt_t;

/**
 * @brief error identifier
 */
typedef const char *wws_ret_t;

/**
 * @brief phase
 */
typedef const char *const wws_phase_t;

/**
 * @brief xfert
 */
typedef const char *const wws_xfer_t;

/**
 * @brief general configuration
 */
typedef const char *const wws_conf_t;

/**
 * @brief conv val into u8 array in big endian, max len = 4
 */
#define WWS_8BITS_BE(_len, _val)                                                                   \
  {                                                                                                \
    [(((_len) -4) > 0 ? ((_len) -4) : 0)] = (((_val) >> (8 * (4 - 1))) & 0xFF),                    \
                                      [(((_len) -3) > 0 ? ((_len) -3) : 0)] =                      \
                                        (((_val) >> (8 * (3 - 1))) & 0xFF),                        \
                                      [(((_len) -2) > 0 ? ((_len) -2) : 0)] =                      \
                                        (((_val) >> (8 * (2 - 1))) & 0xFF),                        \
                                      [(((_len) -1) > 0 ? ((_len) -1) : 0)] =                      \
                                        (((_val) >> (8 * (1 - 1))) & 0xFF),                        \
  }

/**
 * @brief conv val into u8 array in little endian, max len = 4
 */
#define WWS_8BITS_LE(_len, _val)                                                                   \
  {                                                                                                \
    [(_len) -1] = 0, [((_len) > 3) ? 3 : 0] = (((_val) >> 24) & 0xFF),                             \
            [((_len) > 2) ? 2 : 0] = (((_val) >> 16) & 0xFF),                                      \
            [((_len) > 1) ? 1 : 0] = (((_val) >> 8) & 0xFF), [0] = ((_val) &0xFF)                  \
  }

#include "bitmask.h"

#endif /* ___WWS_TYPEDEF_H___ */
