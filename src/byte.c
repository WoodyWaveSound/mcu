/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/byte.h>
#include <wws_mcu/debug.h>

unsigned int wws_byte_get(wws_byte_t *b, char *buf)
{
  wws_assert(b && b->interface && b->interface->get);
  return b->interface->get(b->instance, buf);
}

unsigned int wws_byte_read(wws_byte_t *b, unsigned int size, char *buf)
{
  wws_assert(b && b->interface);
  if (b->interface->read) { return b->interface->read(b->instance, size, buf); }
  else {
    unsigned int len = 0;
    for (; len < size; len++) {
      if (wws_byte_get(b, &buf[len]) == 0) break;
    }
    return len;
  }
}

unsigned int wws_byte_put(wws_byte_t *b, char byte)
{
  wws_assert(b && b->interface && b->interface->put);
  return b->interface->put(b->instance, byte);
}

unsigned int wws_byte_write(wws_byte_t *b, char *bytes, unsigned int len)
{
  wws_assert(b && b->interface);
  if (b->interface->write) { return b->interface->write(b->instance, bytes, len); }
  else {
    unsigned int l = 0;
    for (; l < len; l++) {
      if (wws_byte_put(b, bytes[l]) == 0) break;
    }
    return l;
  }
}

unsigned int wws_byte_put_repeat(wws_byte_t *b, char byte, unsigned int times)
{
  unsigned int len = 0;
  for (; len < times; len++) {
    if (wws_byte_put(b, byte) == 0) { break; }
  }
  return len;
}


void wws_byte_rx_reset(wws_byte_t *b)
{
  char c = 0;
  while (wws_byte_get(b, &c))
    ;
}
