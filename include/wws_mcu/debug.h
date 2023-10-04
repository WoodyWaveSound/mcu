/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_DEBUG_H___
#define ___WWS_DEBUG_H___

#include <stdio.h>

#include "typedef.h"
#include "time.h"

/**
 * @brief Config Debug Message length
 */
#ifndef WWS_CONFIG_DBG_MSG_LEN
#define WWS_CONFIG_DBG_MSG_LEN (128U)
#endif /** WWS_CONFIG_DBG_MSG_LEN */

/**
 * @brief Type for message output
 */
#define WWS_DEBUG_MESSAGE (1U << 0)
/**
 * @brief Type for event output
 */
#define WWS_DEBUG_EVENT (1U << 1)

/**
 * @brief Debug object
 */
typedef struct __wws_debug_t
{
  /**
   * @brief component
   */
  const char *component;
  /**
   * @brief event
   */
  const char *event;
  /**
   * @brief timestamp of deug object create
   */
  const unsigned int timestamp;
  /**
   * @brief data array or message string
   */
  const void *const *data;
  /**
   * @brief length of message or numbers of data array
   */
  const unsigned short len;
  /**
   * @brief Debug type
   */
  const unsigned char type;
} wws_debug_t;

/**
 * @brief debug callback definition
 * @param debug debug object
 */
typedef void (*wws_debug_callback_t)(const wws_debug_t *debug);


/**
 * @brief wws debug callback
 */
extern wws_debug_callback_t ___wws_debug_callback;


/**
 * @brief dispatch debug
 * @param _debug wws_debug_t
 */
#define ___wws_debug(_debug)                                                                       \
  do {                                                                                             \
    if (___wws_debug_callback) { ___wws_debug_callback(_debug); }                                  \
  } while (0)

/**
 * @brief register debug callback
 * @param callback callback (NULL to disable debug output)
 */
static inline void wws_debug_set_callback(wws_debug_callback_t callback)
{
  ___wws_debug_callback = callback;
}


/**
 * @brief Generate wws_debug_t
 * @param _comp const char * Component
 * @param _evt const char *event
 * @param _type type
 * @param _data data array or message string
 * @param _len length of string or number of data
 * @param ... override wws_debug_t field
 * @return wws_debug_t
 */
#define wws_new_debug(_comp, _evt, _type, _data, _len, ...)                                        \
  (wws_debug_t)                                                                                    \
  {                                                                                                \
    .component = _comp, .event = _evt, .timestamp = wws_get_tick(), .type = _type, .data = _data,  \
    .len = _len, ##__VA_ARGS__                                                                     \
  }


/**
 * @brief general debug
 * @param ... override wws_debug_t field
 */
#define __wws_debug(_comp, _evt, _type, _data, _len, ...)                                          \
  ___wws_debug(&(wws_new_debug(_comp, _evt, _type, _data, _len, ##__VA_ARGS__)))

/**
 * @brief debug event only
 * @note no data version to reduce work
 */
#define wws_event_only(_comp, _evt) __wws_debug(_comp, _evt, WWS_DEBUG_EVENT, 0, 0);

/**
 * @brief debug event
 * @param ... data pointers
 */
#define wws_event(_comp, _evt, ...)                                                                \
  do {                                                                                             \
    const void *const   *WWS_LOCAL_VAR(data) = (const void *const[]){ NULL, ##__VA_ARGS__ };       \
    const unsigned short WWS_LOCAL_VAR(len)  = (sizeof(WWS_LOCAL_VAR(data)) / sizeof(void *)) - 1; \
    __wws_debug(_comp, _evt, WWS_DEBUG_EVENT, WWS_LOCAL_VAR(data) + 1, WWS_LOCAL_VAR(len));        \
  } while (0)

/**
 * @brief debug message string
 * @param _str string
 * @param _len length of string
 */
#define wws_msg_str(_comp, _evt, _str, _len)                                                       \
  __wws_debug(_comp, _evt, WWS_DEBUG_MESSAGE, (const void *const *) _str, _len)


/**
 * @brief debug for raw string
 */
#define wws_msg_str_raw(_comp, _evt, _str) wws_msg_str(_comp, _evt, _str, sizeof(_str) - 1)

/**
 * @brief debug message as printf
 */
#define wws_msg(_comp, _evt, _format, ...)                                                         \
  do {                                                                                             \
    char WWS_LOCAL_VAR(buf)[WWS_CONFIG_DBG_MSG_LEN] = { 0 };                                       \
    int  WWS_LOCAL_VAR(len) =                                                                      \
      snprintf(WWS_LOCAL_VAR(buf), WWS_CONFIG_DBG_MSG_LEN, _format, ##__VA_ARGS__);                \
    wws_msg_str(                                                                                   \
      _comp, _evt, WWS_LOCAL_VAR(buf), (WWS_LOCAL_VAR(len) > 0 ? WWS_LOCAL_VAR(len) : 0));         \
  } while (0)

/**
 * @brief General log component
 */
extern const char *WWS_COMP_LOG;
/**
 * @brief Trace level
 */
extern const char *WWS_EVT_LOG_TRACE;
/**
 * @brief Debug level
 */
extern const char *WWS_EVT_LOG_DEBUG;
/**
 * @brief Info level
 */
extern const char *WWS_EVT_LOG_INFO;
/**
 * @brief Warn level
 */
extern const char *WWS_EVT_LOG_WARN;
/**
 * @brief Error level
 */
extern const char *WWS_EVT_LOG_ERROR;
/**
 * @brief Fatal level
 */
extern const char *WWS_EVT_LOG_FATAL;

/**
 * @brief general log
 */
#define wws_log(_lv, _format, ...)   wws_msg(WWS_COMP_LOG, _lv, _format, ##__VA_ARGS__)
#define wws_logln(_lv, _format, ...) wws_log(_lv, _format "\r\n", ##__VA_ARGS__)

/**
 * @brief Trace log
 */
#define wws_trace(_format, ...)   wws_log(WWS_EVT_LOG_TRACE, _format, ##__VA_ARGS__)
#define wws_traceln(_format, ...) wws_trace(_format "\r\n", ##__VA_ARGS__)

/**
 * @brief Debug log
 */
#define wws_debug(_format, ...)   wws_log(WWS_EVT_LOG_DEBUG, _format, ##__VA_ARGS__)
#define wws_debugln(_format, ...) wws_debug(_format "\r\n", ##__VA_ARGS__)

/**
 * @brief Info log
 */
#define wws_info(_format, ...)   wws_log(WWS_EVT_LOG_INFO, _format, ##__VA_ARGS__)
#define wws_infoln(_format, ...) wws_info(_format "\r\n", ##__VA_ARGS__)

/**
 * @brief Warn log
 */
#define wws_warn(_format, ...)   wws_log(WWS_EVT_LOG_WARN, _format, ##__VA_ARGS__)
#define wws_warnln(_format, ...) wws_warn(_format "\r\n", ##__VA_ARGS__)

/**
 * @brief Error log
 */
#define wws_error(_format, ...)   wws_log(WWS_EVT_LOG_ERROR, _format, ##__VA_ARGS__)
#define wws_errorln(_format, ...) wws_error(_format "\r\n", ##__VA_ARGS__)

/**
 * @brief Fatal log
 */
#define wws_fatal(_format, ...)   wws_log(WWS_EVT_LOG_FATAL, _format, ##__VA_ARGS__)
#define wws_fatalln(_format, ...) wws_fatal(_format "\r\n", ##__VA_ARGS__)

/**
 * @brief Assert component
 */
extern const char *WWS_COMP_ASSERT;

/**
 * @brief Assert
 *
 * - @p data[0]: wws_cstr_t
 * - @p data[1]: filename
 * - @p data[2]: line
 */
#define wws_assert(_cond)                                                                          \
  ({                                                                                               \
    bool res = _cond;                                                                              \
    if (!res) {                                                                                    \
      wws_event(WWS_COMP_ASSERT, NULL, &wws_new_cstr(#_cond), __FILE__, (void *) __LINE__);        \
      for (;;)                                                                                     \
        ;                                                                                          \
    }                                                                                              \
    (!res);                                                                                        \
  })

#endif /* ___WWS_DEBUG_H___ */
