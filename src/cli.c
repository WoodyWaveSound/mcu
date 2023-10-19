/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <string.h>

#include <wws_mcu/cli.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/bitmask.h>

WWS_WEAK wws_ret_t WWS_RET_OK             = "OK";
WWS_WEAK wws_ret_t WWS_RET_ERR_ABORT      = "ERR_ABORT";
WWS_WEAK wws_ret_t WWS_RET_ERR_ARGS       = "ERR_ARGS";
WWS_WEAK wws_ret_t WWS_RET_ERR_NO_MATCHED = "ERR_NO_MATCHED";

wws_comp_t         WWS_COMP_CLI  = "Cli";
WWS_WEAK wws_evt_t WWS_EVT_MATCH = "MATCH";
WWS_WEAK wws_evt_t WWS_EVT_RUN   = "RUN";
WWS_WEAK wws_evt_t WWS_EVT_RESET = "RESET";

extern wws_phase_t WWS_ON_MATCH WWS_ALIAS(WWS_EVT_MATCH);
extern wws_phase_t WWS_ON_RUN   WWS_ALIAS(WWS_EVT_RUN);
extern wws_phase_t WWS_ON_RESET WWS_ALIAS(WWS_EVT_RESET);


int wws_cli_get_token(const char *ptr, unsigned int len, unsigned char skip)
{
  int skip_len = 0;

  /** skip white space */
  while ((*ptr == ' ') && (skip_len < len)) {
    ptr++;
    skip_len++;
  }
  if ((*ptr == 0) || (skip_len == len)) return -1;

  while (skip) {
    while ((*ptr != ' ') && (skip_len < len)) {
      ptr++;
      skip_len++;
    }

    /** skip white space */
    while ((*ptr == ' ') && (skip_len < len)) {
      ptr++;
      skip_len++;
    }
    if ((*ptr == 0) || (skip_len == len)) return -1;
    skip--;
  }

  return skip_len;
}

static void prompt(wws_cli_t *cli)
{
  static wws_cstr_t default_prompt = wws_new_cstr("#");

  wws_cstr_t *prompt = &default_prompt;
  if (cli->prompt.str) { prompt = &cli->prompt; }

  wws_byte_write_cstr(cli->io, prompt);
  wws_byte_put(cli->io, ' ');
}

static inline void echo(wws_cli_t *cli, char c)
{
  if (cli->echo) { wws_byte_put(cli->io, c); }
}

enum input_t
{
  INPUT_COMPLETE,
  INPUT_DATA,
  INPUT_BACKSPACE,
  INPUT_DROP,
};

static enum input_t check_input(char c)
{
  switch (c) {
  case '\r':
  case '\n':
  case '\0': return INPUT_COMPLETE;
  case '\b': return INPUT_BACKSPACE;
  default: break;
  }
  if ((c >= 32) && (c < 127)) return INPUT_DATA;
  return INPUT_DROP;
}

static int fetch(wws_cli_t *cli)
{
  char c = 0;
  while (wws_byte_get(cli->io, &c)) {
    switch (check_input(c)) {
    case INPUT_COMPLETE: {
      cli->buffer[cli->buf_len] = 0;
      echo(cli, c);
      return 1;
    } break;
    case INPUT_BACKSPACE: {
      if (cli->buf_len) {
        cli->buf_len--;
        wws_byte_write_str(cli->io, "\b \b");
      }
    } break;
    case INPUT_DATA: {
      if (cli->buf_len < (WWS_CONFIG_CLI_BUF_SIZE - 1)) {
        cli->buffer[cli->buf_len++] = c;
        echo(cli, c);
      }
    } break;
    case INPUT_DROP: break;
    }
  }
  return 0;
}

static bool is_match(const char *ptr, wws_cstr_t *cmd)
{
  do {
    if (ptr[cmd->len] == ' ') break;
    if (ptr[cmd->len] == '\0') break;
    return false;
  } while (0);

  return memcmp(ptr, cmd->str, cmd->len) == 0;
}

static void parse(wws_cli_t *cli)
{
  static wws_cstr_t help = wws_new_cstr("?");

  wws_cli_cmd_t *cur = &cli->root, **child = 0;
  const char    *ptr      = cli->buffer;
  unsigned int   len      = cli->buf_len;
  int            skip_len = 0, skip_cnt = 0;
  wws_ret_t      ret = WWS_RET_OK;

  /** remove lead space */
  skip_len = wws_cli_get_token(ptr, len, 0);
  if (skip_len == -1) return;

  cur->parse.ptr = ptr;
  cur->parse.len = len;

  do {
    ptr += skip_len;
    len -= skip_len;

    if (is_match(ptr, &help)) {
      /** help */

      wws_byte_write_str(cli->io, "Command List:");
      wws_byte_write_str(cli->io, "\r\n");
      for (child = (wws_cli_cmd_t **) cur->children; *child != 0; child++) {
        if (!wws_bitmask_every(cli->lock, (*child)->lock)) continue;

        wws_byte_write_str(cli->io, "  ");
        wws_byte_write_cstr(cli->io, &(*child)->cmd);
        wws_byte_put_repeat(cli->io, ' ', 10 - (*child)->cmd.len, 0);
        wws_byte_write_str(cli->io, "#arg=");
        wws_byte_put(cli->io, (*child)->arg_num + '0');
        wws_byte_write_str(cli->io, "  ");
        if ((*child)->batch) { wws_byte_write_str(cli->io, "BATCH"); }
        wws_byte_write_str(cli->io, "\r\n");
      }

      ret = WWS_RET_ERR_ABORT;
      break;
    }

    if (skip_cnt) {
      skip_cnt--;
      continue;
    }

    for (child = (wws_cli_cmd_t **) cur->children; child != 0 && *child != 0; child++) {
      /** lock check */
      if (!wws_bitmask_every(cli->lock, (*child)->lock)) continue;
      if (!is_match(ptr, &(*child)->cmd)) continue;
      break;
    }

    if (child && *child) { /** match */
      (*child)->parse.ptr = ptr + (*child)->cmd.len;
      (*child)->parse.len = len - (*child)->cmd.len;
      int skip            = wws_cli_get_token((*child)->parse.ptr, (*child)->parse.len, 0);
      if (skip >= 0) {
        (*child)->parse.ptr += skip;
        (*child)->parse.len -= skip;
      }


      if ((*child)->callback) {
        ret =
          (*child)->callback(WWS_ON_MATCH, (*child)->parse.ptr, (*child)->parse.len, (*child), cli);
        if (ret != WWS_RET_OK) break;
      }

      skip_cnt = (*child)->arg_num;

      if ((*child)->batch == 0) {
        cur->parse.next      = (*child);
        (*child)->parse.prev = cur;
        cur                  = *child;
        child                = 0;
      }
    }
    else if (cur->children == 0) { /** no child */ ret = WWS_RET_ERR_ARGS;
      break;
    }
    else { /** not match */ ret = WWS_RET_ERR_NO_MATCHED;
      break;
    }
  } while ((skip_len = wws_cli_get_token(ptr, len, 1)) >= 0);

  if (ret == WWS_RET_OK) {
    for (wws_cli_cmd_t *p = &cli->root; p != 0; p = p->parse.next) {
      if (p->callback) { p->callback(WWS_ON_RUN, p->parse.ptr, p->parse.len, p, cli); }
    }
  }
  else if (ret != WWS_RET_ERR_ABORT) {
    /** error happened */
    wws_byte_write_str(cli->io, "\r\n");
    wws_byte_write_str(cli->io, "Error: \r\n");
    unsigned int skip = cli->buf_len - cur->parse.len +
                        wws_cli_get_token(cur->parse.ptr, cur->parse.len, cur->arg_num);
    wws_byte_write_str(cli->io, "\r\n");
    wws_byte_write(cli->io, cli->buffer, cli->buf_len, 0);
    wws_byte_write_str(cli->io, "\r\n");
    wws_byte_put_repeat(cli->io, ' ', skip, 0);
    wws_byte_put(cli->io, '^');
    wws_byte_write_str(cli->io, "\r\n");
    wws_byte_put_repeat(cli->io, ' ', skip, 0);
    wws_byte_write_str(cli->io, ret);
    wws_byte_write_str(cli->io, "\r\n");
  }

  for (wws_cli_cmd_t *p = cur; p != 0; p = p->parse.prev) {
    if (p->callback) { p->callback(WWS_ON_RESET, p->parse.ptr, p->parse.len, p, cli); }
    p->parse.next = 0;
    p->parse.ptr  = 0;
    p->parse.len  = 0;
  }
}

void wws_cli_parse(wws_cli_t *cli)
{
  wws_assert(cli && cli->io);

  /** first time */
  if (cli->_first == 0) {
    cli->buf_len = 0;
    prompt(cli);
    cli->_first = 1;
    return;
  }

  /** fetch */
  if (fetch(cli) == 0) return;

  if (cli->buf_len) { parse(cli); }

  cli->buf_len = 0;
  prompt(cli);
  if (cli->no_reset_rx == 0) { wws_byte_rx_reset(cli->io); }
}

void wws_cli_put(wws_cli_t *cli, char byte)
{
  wws_byte_put(cli->io, byte);
}

void wws_cli_write(wws_cli_t *cli, char *bytes, unsigned int len)
{
  wws_byte_write(cli->io, bytes, len, 0);
}
