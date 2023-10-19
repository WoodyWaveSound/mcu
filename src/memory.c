/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/memory.h>
#include <wws_mcu/debug.h>

wws_ret_t wws_memory_put8(wws_memory_t *m, unsigned int offset, char data)
{
  wws_assert(m && m->interface && m->interface->put8);
  if ((offset + 1) > m->size) return WWS_RET_ERR_OVERSIZE;
  return m->interface->put8(m->inst, m->base + offset, data);
}

wws_ret_t wws_memory_put16(wws_memory_t *m, unsigned int offset, short data)
{
  wws_assert(m && m->interface);
  if (m->interface->put16 == 0)
    return wws_memory_write8(m,
                             offset,
                             (union {
                               short d;
                               char  b[2];
                             }){ .d = data }
                               .b,
                             2,
                             0);
  if ((offset + 2) > m->size) return WWS_RET_ERR_OVERSIZE;
  return m->interface->put16(m->inst, m->base + offset, data);
}

wws_ret_t wws_memory_put32(wws_memory_t *m, unsigned int offset, int data)
{
  wws_assert(m && m->interface);
  if (m->interface->put32 == 0)
    return wws_memory_write8(m,
                             offset,
                             (union {
                               int  d;
                               char b[4];
                             }){ .d = data }
                               .b,
                             4,
                             0);
  if ((offset + 4) > m->size) return WWS_RET_ERR_OVERSIZE;
  return m->interface->put32(m->inst, m->base + offset, data);
}

wws_ret_t wws_memory_write8(
  wws_memory_t *m, unsigned int offset, const char *data, unsigned int len, unsigned int *written)
{
  wws_assert(m && m->interface);
  if (m->interface->write8) {
    if ((offset + (len + 1) * 1) > m->size) return WWS_RET_ERR_OVERSIZE;
    return m->interface->write8(m->inst, m->base + offset, data, len, written);
  }
  unsigned int l   = 0;
  wws_ret_t    ret = WWS_RET_OK;
  for (; l < len; l++) {
    if ((ret = wws_memory_put8(m, offset + l * 1, data[l])) != WWS_RET_OK) break;
  }
  (*written) = l;
  return ret;
}

wws_ret_t wws_memory_write16(
  wws_memory_t *m, unsigned int offset, const short *data, unsigned int len, unsigned int *written)
{
  wws_assert(m && m->interface);
  if (m->interface->write16) {
    if ((offset + (len + 1) * 2) > m->size) return WWS_RET_ERR_OVERSIZE;
    return m->interface->write16(m->inst, m->base + offset, data, len, written);
  }
  unsigned int l   = 0;
  wws_ret_t    ret = WWS_RET_OK;
  for (; l < len; l++) {
    if ((ret = wws_memory_put16(m, offset + l * 2, data[l])) != WWS_RET_OK) break;
  }
  (*written) = l;
  return ret;
}

wws_ret_t wws_memory_write32(
  wws_memory_t *m, unsigned int offset, const int *data, unsigned int len, unsigned int *written)
{
  wws_assert(m && m->interface);
  if (m->interface->write32) {
    if ((offset + (len + 1) * 4) > m->size) return WWS_RET_ERR_OVERSIZE;
    return m->interface->write32(m->inst, m->base + offset, data, len, written);
  }
  unsigned int l   = 0;
  wws_ret_t    ret = WWS_RET_OK;
  for (; l < len; l++) {
    if ((ret = wws_memory_put32(m, offset + l * 4, data[l])) != WWS_RET_OK) break;
  }
  (*written) = l;
  return ret;
}

wws_ret_t wws_memory_get8(wws_memory_t *m, unsigned int offset, char *buf)
{
  wws_assert(m && m->interface && m->interface->get8);
  if ((offset + 1) > m->size) return WWS_RET_ERR_OVERSIZE;
  return m->interface->get8(m->inst, m->base + offset, buf);
}

wws_ret_t wws_memory_get16(wws_memory_t *m, unsigned int offset, short *buf)
{
  wws_assert(m && m->interface);
  if (m->interface->get16 == 0) return wws_memory_read8(m, offset, 2, (char *) buf, 0);
  if ((offset + 2) > m->size) return WWS_RET_ERR_OVERSIZE;
  return m->interface->get16(m->inst, m->base + offset, buf);
}

wws_ret_t wws_memory_get32(wws_memory_t *m, unsigned int offset, int *buf)
{
  wws_assert(m && m->interface);
  if (m->interface->get32 == 0) return wws_memory_read8(m, offset, 4, (char *) buf, 0);
  if ((offset + 4) > m->size) return WWS_RET_ERR_OVERSIZE;
  return m->interface->get32(m->inst, m->base + offset, buf);
}

wws_ret_t wws_memory_read8(
  wws_memory_t *m, unsigned int offset, unsigned int size, char *buf, unsigned int *buffered)
{
  wws_assert(m && m->interface);
  if (m->interface->read8) {
    if ((offset + (size + 1) * 1) > m->size) return WWS_RET_ERR_OVERSIZE;
    return m->interface->read8(m->inst, m->base + offset, size, buf, buffered);
  }
  unsigned int l   = 0;
  wws_ret_t    ret = WWS_RET_OK;
  for (; l < size; l++) {
    if ((ret = wws_memory_get8(m, offset + l * 1, &buf[l])) != WWS_RET_OK) break;
  }
  (*buffered) = l;
  return ret;
}

wws_ret_t wws_memory_read16(
  wws_memory_t *m, unsigned int offset, unsigned int size, short *buf, unsigned int *buffered)
{
  wws_assert(m && m->interface);
  if (m->interface->read16) {
    if ((offset + (size + 1) * 2) > m->size) return WWS_RET_ERR_OVERSIZE;
    return m->interface->read16(m->inst, m->base + offset, size, buf, buffered);
  }
  unsigned int l   = 0;
  wws_ret_t    ret = WWS_RET_OK;
  for (; l < size; l++) {
    if ((ret = wws_memory_get16(m, offset + l * 2, &buf[l])) != WWS_RET_OK) break;
  }
  (*buffered) = l;
  return ret;
}

wws_ret_t wws_memory_read32(
  wws_memory_t *m, unsigned int offset, unsigned int size, int *buf, unsigned int *buffered)
{
  wws_assert(m && m->interface);
  if (m->interface->read32) {
    if ((offset + (size + 1) * 4) > m->size) return WWS_RET_ERR_OVERSIZE;
    return m->interface->read32(m->inst, m->base + offset, size, buf, buffered);
  }
  unsigned int l   = 0;
  wws_ret_t    ret = WWS_RET_OK;
  for (; l < size; l++) {
    if ((ret = wws_memory_get32(m, offset + l * 4, &buf[l])) != WWS_RET_OK) break;
  }
  (*buffered) = l;
  return ret;
}
