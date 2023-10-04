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

/**
 * @brief Command line buffer size
 */
#ifndef WWS_CONFIG_CLI_BUF_SIZE
#define WWS_CONFIG_CLI_BUF_SIZE (128U)
#endif /** WWS_CONFIG_CLI_BUF_SIZE */

/**
 * @brief Enable echo function
 */
#define WWS_CLI_ECHO (1U << 0)
/**
 * @brief reset rx after process done
 */
#define WWS_CLI_RESET_RX (1U << 1)

/**
 * @brief marked cmd is batch/option
 */
#define WWS_CLI_CMD_OPTION_BATCH (1U << 0)


/** forward */
typedef struct __wws_cli_t     wws_cli_t;
typedef struct __wws_cli_cmd_t wws_cli_cmd_t;

/**
 * @brief error of cli
 */
typedef enum __wws_cli_err_t
{
  /**
   * @brief Ok, no error
   */
  WWS_CLI_ERR_OK,
  /**
   * @brief parser abort without error
   */
  WWS_CLI_ERR_ABORT,
  /**
   * @brief Args error
   */
  WWS_CLI_ERR_ARGS,
  /**
   * @brief Error when no matched
   */
  WWS_CLI_ERR_NO_MATCH,
  /**
   * @brief uncategorized error
   */
  WWS_CLI_ERR_OTHER,
} wws_cli_err_t;

/**
 * @brief error string
 */
extern const char *const wws_cli_err_str[];

/**
 * @brief Cmd run type
 */
typedef enum __wws_cli_cmd_type_t
{
  /**
   * @brief when cmd just matched
   */
  WWS_CLI_CMD_MATCH,
  /**
   * @brief when cmd run
   */
  WWS_CLI_CMD_RUN,
  /**
   * @brief after cmd, reset cache
   */
  WWS_CLI_CMD_RESET,
} wws_cli_cmd_type_t;

/**
 * @brief type str
 */
extern const char *const wws_cli_cmd_type_str[];

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
  const unsigned char lock;
  /**
   * @brief num of arguments
   */
  const unsigned char arg_num;
  /**
   * @brief flags
   */
  const unsigned char flag;
  /**
   * @brief children commands
   */
  struct __wws_cli_cmd_t *const *const children;
  /**
   * @brief callback
   */
  wws_cli_err_t (*callback)(wws_cli_cmd_type_t      type,
                            const char             *ptr,
                            unsigned int            len,
                            struct __wws_cli_cmd_t *cmd,
                            wws_cli_t              *cli);
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
 * @brief Command Line Instance
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
  unsigned short buf_len;
  /**
   * @brief lock flags
   */
  unsigned char lock;
  /**
   * @brief flag
   */
  unsigned char flag;
} wws_cli_t;


/**
 * @brief get token
 * @param ptr
 * @param len
 * @param skip skip token
 * @return unsigned int length of byte to skip to get token ptr, -1: no token until \0
 */
extern int wws_cli_get_token(const char *ptr, unsigned int len, unsigned char skip);

/**
 * @brief Parse command line
 * @param cli
 */
extern void wws_cli_parse(wws_cli_t *cli);

#endif /* ___WWS_CLI_H___ */
