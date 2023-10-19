/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/byte.h>
#include <wws_mcu/debug.h>

WWS_WEAK wws_ret_t WWS_RET_OK        = "OK";
WWS_WEAK wws_ret_t WWS_RET_ERR_OTHER = "OTHER";

wws_ret_t wws_byte_get(wws_byte_t *b, char *buf)
{
  wws_assert(b && b->interface && b->interface->get);
  return b->interface->get(b->inst, buf);
}

wws_ret_t wws_byte_read(wws_byte_t *b, unsigned int size, char *buf, unsigned int *buffered)
{
  wws_assert(b && b->interface);
  if (b->interface->read) { return b->interface->read(b->inst, size, buf, buffered); }
  else {
    wws_ret_t    ret = WWS_RET_OK;
    unsigned int len = 0;
    for (; len < size; len++) {
      if ((ret = wws_byte_get(b, &buf[len])) != WWS_RET_OK) break;
    }
    if (buffered) *buffered = len;
    return ret;
  }
}

wws_ret_t wws_byte_put(wws_byte_t *b, char byte)
{
  wws_assert(b && b->interface && b->interface->put);
  return b->interface->put(b->inst, byte);
}

wws_ret_t wws_byte_write(wws_byte_t *b, const char *bytes, unsigned int len, unsigned int *written)
{
  wws_assert(b && b->interface);
  if (b->interface->write) { return b->interface->write(b->inst, bytes, len, written); }
  else {
    wws_ret_t    ret = WWS_RET_OK;
    unsigned int l   = 0;
    for (; l < len; l++) {
      if ((ret = wws_byte_put(b, bytes[l])) != WWS_RET_OK) break;
    }
    if (written) *written = l;
    return ret;
  }
}

wws_ret_t wws_byte_write_str(wws_byte_t *b, const char *str)
{
  wws_ret_t ret = WWS_RET_OK;
  for (; *str != 0; str++) {
    if ((ret = wws_byte_put(b, *str)) != WWS_RET_OK) break;
  }
  return ret;
}


wws_ret_t wws_byte_put_repeat(wws_byte_t *b, char byte, unsigned int times, unsigned int *written)
{
  wws_ret_t    ret = WWS_RET_OK;
  unsigned int len = 0;
  for (; len < times; len++) {
    if ((ret = wws_byte_put(b, byte)) != WWS_RET_OK) { break; }
  }
  if (written) *written = len;
  return ret;
}


void wws_byte_rx_reset(wws_byte_t *b)
{
  char c = 0;
  while (wws_byte_get(b, &c) == WWS_RET_OK)
    ;
}
