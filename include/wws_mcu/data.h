/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_DATA_H___
#define ___WWS_DATA_H___

#include <stdbool.h>

#include "typedef.h"
#include "bitmask.h"
#include "debug.h"

extern const char *WWS_COMP_DATA;
extern const char *WWS_EVT_CHANGE;
extern const char *WWS_EVT_WRITE;

/**
 * @brief define data type
 * @param _name name of type ==> @p wws_data_<_name>_t
 * @param _type real type of container
 */
#define WWS_DEFINE_DATA(_name, _type)                                                              \
  typedef struct __wws_data_##_name##_t                                                            \
  {                                                                                                \
    _type *const         ref;                                                                      \
    _type                data;                                                                     \
    unsigned short       changed, written;                                                         \
    unsigned short       len;                                                                      \
    const unsigned short size;                                                                     \
    _type                max, min;                                                                 \
    struct                                                                                         \
    {                                                                                              \
      const _type *const  value;                                                                   \
      const unsigned char num;                                                                     \
    } select;                                                                                      \
  } wws_data_##_name##_t

/**
 * @brief data write error
 */
enum wws_data_write_err_t
{
  /**
   * @brief normal, no changed
   */
  WWS_DATA_WRITE_NORMAL,
  /**
   * @brief write ok, data changed
   */
  WWS_DATA_WRITE_CHANGED,
  /**
   * @brief data larger than max
   */
  WWS_DATA_WRITE_MAX_EXCEED,
  /**
   * @brief data smaller than min
   */
  WWS_DATA_WRITE_MIN_EXCEED,
  /**
   * @brief data not in select definition
   */
  WWS_DATA_WRITE_NOT_SELECTABLE,
  /**
   * @brief bulk data over size
   */
  WWS_DATA_WRITE_OVER_SIZE,
};

/**
 * @brief read data
 */
#define wws_data_read(_data) ((_data)->ref == 0 ? (_data)->data : *((_data)->ref))

/**
 * @brief write data value
 * @param _value value to write
 * @param ... access flag
 */
#define wws_data_write(_data, _value, ...)                                                         \
  ({                                                                                               \
    enum wws_data_write_err_t ok = WWS_DATA_WRITE_NORMAL;                                          \
    do {                                                                                           \
      if ((_data)->max > (_data)->min) {                                                           \
        if ((_value) > (_data)->max) {                                                             \
          ok = WWS_DATA_WRITE_MAX_EXCEED;                                                          \
          break;                                                                                   \
        }                                                                                          \
        if ((_value) < (_data)->min) {                                                             \
          ok = WWS_DATA_WRITE_MIN_EXCEED;                                                          \
          break;                                                                                   \
        }                                                                                          \
      }                                                                                            \
      if (((_data)->select.value != 0) && (_data)->select.num != 0) {                              \
        ok = WWS_DATA_WRITE_NOT_SELECTABLE;                                                        \
        for (int i = 0; i < (_data)->select.num; i++) {                                            \
          if ((_value) == (_data)->select.value[i]) {                                              \
            ok = WWS_DATA_WRITE_NORMAL;                                                            \
            break;                                                                                 \
          }                                                                                        \
        }                                                                                          \
        if (ok != WWS_DATA_WRITE_NORMAL) { break; }                                                \
      }                                                                                            \
      unsigned short access = (WWS_OVERRIDE(unsigned short, 0x0, ##__VA_ARGS__));                  \
      if ((_value) != wws_data_read(_data)) {                                                      \
        if ((_data)->ref) { *((_data)->ref) = (_value); }                                          \
        else {                                                                                     \
          (_data)->data = (_value);                                                                \
        }                                                                                          \
        (_data)->changed |= ~access;                                                               \
        wws_event(WWS_COMP_DATA, WWS_EVT_CHANGE, (_data), (void *) access);                        \
        ok = WWS_DATA_WRITE_CHANGED;                                                               \
      }                                                                                            \
      (_data)->written |= ~access;                                                                 \
      wws_event(WWS_COMP_DATA, WWS_EVT_WRITE, (_data), (void *) access);                           \
    } while (0);                                                                                   \
    (ok);                                                                                          \
  })

/**
 * @brief write data bulk with length
 * @param _bulk bulk data
 * @param _len length of data
 * @param ... access flag
 */
#define wws_data_write_bulk(_data, _bulk, _len, ...)                                               \
  ({                                                                                               \
    enum wws_data_write_err_t ok = WWS_DATA_WRITE_NORMAL;                                          \
    do {                                                                                           \
      if ((_len) > (_data)->size) {                                                                \
        ok = WWS_DATA_WRITE_OVER_SIZE;                                                             \
        break;                                                                                     \
      }                                                                                            \
      unsigned short access = (WWS_OVERRIDE(unsigned short, 0x0, ##__VA_ARGS__));                  \
      if (((_len) != (_data)->len) || (memcmp(wws_data_read(_data), _bulk, _len) != 0)) {          \
        memset(wws_data_read(_data), 0, (_data)->size);                                            \
        memcpy(wws_data_read(_data), _bulk, _len);                                                 \
        (_data)->changed |= ~access;                                                               \
        wws_event(WWS_COMP_DATA, WWS_EVT_CHANGE, (_data), (void *) access);                        \
        ok = WWS_DATA_WRITE_CHANGED;                                                               \
      }                                                                                            \
      (_data)->written |= ~access;                                                                 \
      wws_event(WWS_COMP_DATA, WWS_EVT_WRITE, (_data), (void *) access);                           \
    } while (0);                                                                                   \
    (ok);                                                                                          \
  })

/**
 * @brief is data changed
 * @param ... access flags
 */
#define wws_data_is_changed(_data, ...)                                                            \
  (wws_bitmask_any((_data)->changed, WWS_OVERRIDE(unsigned short, 0xFFFF, ##__VA_ARGS__)))
/**
 * @brief is data written
 * @param ... access flags
 */
#define wws_data_is_written(_data, ...)                                                            \
  (wws_bitmask_any((_data)->changed, WWS_OVERRIDE(unsigned short, 0xFFFF, ##__VA_ARGS__)))
/**
 * @brief clear data change
 * @param ... access flags
 */
#define wws_data_clear_changed(_data, ...)                                                         \
  (wws_bitmask_unmask((_data)->changed, WWS_OVERRIDE(unsigned short, 0xFFFF, ##__VA_ARGS__)))
/**
 * @brief clear data written
 * @param ... access flags
 */
#define wws_data_clear_written(_data, ...)                                                         \
  (wws_bitmask_unmask((_data)->changed, WWS_OVERRIDE(unsigned short, 0xFFFF, ##__VA_ARGS__)))

/**
 * @brief write value up for 1
 */
#define wws_data_up(_data, ...) wws_data_write(_data, wws_data_read(_data) + 1, ##__VA_ARGS__)

/**
 * @brief write value down for 1
 */
#define wws_data_down(_data, ...) wws_data_write(_data, wws_data_read(_data) - 1, ##__VA_ARGS__)

/**
 * @brief predefined data type for char
 */
WWS_DEFINE_DATA(i8, char);
/**
 * @brief predefined data type for short
 */
WWS_DEFINE_DATA(i16, short);
/**
 * @brief predefined data type for int
 */
WWS_DEFINE_DATA(i32, int);
/**
 * @brief predefined data type for unsigned char
 */
WWS_DEFINE_DATA(u8, unsigned char);
/**
 * @brief predefined data type for unsigned short
 */
WWS_DEFINE_DATA(u16, unsigned short);
/**
 * @brief predefined data type for unsigned int
 */
WWS_DEFINE_DATA(u32, unsigned int);
/**
 * @brief predefined data type for float
 */
WWS_DEFINE_DATA(f32, float);
/**
 * @brief predefined data type for bool
 */
WWS_DEFINE_DATA(bool, bool);
/**
 * @brief predefined data type for char *
 */
WWS_DEFINE_DATA(string, char *);


#endif /* ___WWS_DATA_H___ */
