/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_CLI_H___
#define ___WWS_CLI_H___

#include "typedef.h"
#include "byte.h"
#include "service.h"

/**
 * @brief Command line buffer size
 */
#ifndef WWS_CONFIG_CLI_BUF_SIZE
#define WWS_CONFIG_CLI_BUF_SIZE (128U)
#endif /** WWS_CONFIG_CLI_BUF_SIZE */

/** forward */
typedef struct __wws_cli_t     wws_cli_t;
typedef struct __wws_cli_cmd_t wws_cli_cmd_t;

extern wws_ret_t WWS_RET_OK;
extern wws_ret_t WWS_RET_ERR_ABORT;
extern wws_ret_t WWS_RET_ERR_ARGS;
extern wws_ret_t WWS_RET_ERR_NO_MATCHED;


// /**
//  * @brief error of cli
//  */
// typedef enum __wws_cli_err_t
// {
//   /**
//    * @brief Ok, no error
//    */
//   WWS_CLI_ERR_OK,
//   /**
//    * @brief parser abort without error
//    */
//   WWS_CLI_ERR_ABORT,
//   /**
//    * @brief Args error
//    */
//   WWS_CLI_ERR_ARGS,
//   /**
//    * @brief Error when no matched
//    */
//   WWS_CLI_ERR_NO_MATCH,
//   /**
//    * @brief uncategorized error
//    */
//   WWS_CLI_ERR_OTHER,
// } wws_cli_err_t;

extern wws_comp_t WWS_COMP_CLI;
extern wws_evt_t  WWS_EVT_MATCH;
extern wws_evt_t  WWS_EVT_RUN;
extern wws_evt_t  WWS_EVT_RESET;

extern wws_phase_t WWS_ON_MATCH;
extern wws_phase_t WWS_ON_RUN;
extern wws_phase_t WWS_ON_RESET;

/**
 * @brief command desc
 */
typedef struct __wws_cli_cmd_t
{
  /**
   * @brief command
   */
  wws_cstr_t cmd;
  /**
   * @brief locks
   */
  const unsigned int lock : 8;
  /**
   * @brief num of arguments
   */
  const unsigned int arg_num : 8;
  /**
   * @brief flags
   */
  const unsigned int batch : 1;
  /**
   * @brief children commands
   */
  struct __wws_cli_cmd_t *const *const children;
  /**
   * @brief callback
   */
  wws_ret_t (*callback)(
    wws_phase_t on, const char *ptr, unsigned int len, struct __wws_cli_cmd_t *cmd, wws_cli_t *cli);
  /**
   * @brief parse cache
   */
  struct
  {
    /**
     * @brief prev cmd in parsing
     */
    struct __wws_cli_cmd_t *prev;
    /**
     * @brief next cmd in parsing
     */
    struct __wws_cli_cmd_t *next;
    /**
     * @brief parsing ptr
     */
    const char *ptr;
    /**
     * @brief length of ptr
     */
    unsigned int len;
  } parse;
} wws_cli_cmd_t;

/**
 * @brief Command Line inst
 */
typedef struct __wws_cli_t
{
  /**
   * @brief byte interface as IO
   */
  wws_byte_t *const io;
  /**
   * @brief buffer
   */
  char buffer[WWS_CONFIG_CLI_BUF_SIZE];
  /**
   * @brief context for running context
   */
  void *const context;
  /**
   * @brief prompt;
   */
  wws_cstr_t prompt;
  /**
   * @brief root command
   */
  wws_cli_cmd_t root;
  /**
   * @brief length of buffer
   */
  unsigned int buf_len : 16;
  /**
   * @brief locks
   */
  unsigned int lock : 8;
  /**
   * @brief without reset rx
   */
  unsigned int no_reset_rx : 1;
  /**
   * @brief echo
   */
  unsigned int echo : 1;
} wws_cli_t;


/**
 * @brief get token
 * @param ptr
 * @param len
 * @param skip skip token
 * @return unsigned int length of byte to skip to get token ptr, -1: no token until \0
 */
extern int wws_cli_get_token(const char *ptr, unsigned int len, unsigned char skip);

extern void ___wws_cli_parse_service_callback(wws_phase_t on, wws_service_t *serv);

/**
 * @brief config for CLI service
 */
#define WWS_CLI_SERVICE .callback = ___wws_cli_parse_service_callback, .default_start = 1

#endif /* ___WWS_CLI_H___ */
