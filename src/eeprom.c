/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/eeprom.h>
#include <wws_mcu/debug.h>

#define REG_LEN_MAX (2)

/** schemas */
const wws_eeprom_schema_t wws_eeprom_AT24C128 = {
  .base_addr_7bit = 0x50, .page_size = 64, .reg_len = 2, .size = 16 * 1024
};
const wws_eeprom_schema_t wws_eeprom_M24C01_W = {
  .base_addr_7bit = 0xA0 >> 1, .page_size = 16, .reg_len = 1, .size = 128
};

/** debug */
wws_comp_t         WWS_COMP_EEPROM = "EEPROM";
WWS_WEAK wws_evt_t WWS_EVT_WRITE   = "WRITE";
WWS_WEAK wws_evt_t WWS_EVT_READ    = "READ";

/** returns */
WWS_WEAK wws_ret_t WWS_RET_OK = "OK";

static inline unsigned short _addr(wws_eeprom_t *eeprom)
{
  return eeprom->schema->base_addr_7bit | (eeprom->ad3 << 3) | (eeprom->ad2 << 2) |
         (eeprom->ad1 << 1) | (eeprom->ad0 << 0);
}

wws_ret_t wws_eeprom_write(wws_eeprom_t        *eeprom,
                           unsigned int         addr,
                           const unsigned char *data,
                           unsigned int         len,
                           unsigned int        *written)
{
  wws_assert(eeprom && eeprom->schema && eeprom->bus && ((addr + len) <= eeprom->schema->size));

  unsigned int         wlen     = 0;
  const unsigned short dev_addr = _addr(eeprom);
  wws_ret_t            ret      = WWS_RET_OK;

  if (eeprom->wc) wws_logic_write(eeprom->wc, WWS_LOW);

  while (len) {
    const unsigned char reg_addr[REG_LEN_MAX] = WWS_8BITS_BE(REG_LEN_MAX, addr + wlen);
    unsigned int        wl                    = len;
    if (wl > eeprom->schema->page_size) wl = eeprom->schema->page_size;


    /** wait eeprom writable */
    for (unsigned int ts = wws_tick_get();;) {
      if ((ret = wws_i2c_test_device(eeprom->bus, dev_addr, WWS_MS(10))) == WWS_RET_OK) break;
      if (ret != WWS_RET_ERR_TIMEOUT) return ret;
      if (wws_tick_isup(ts, WWS_MS(100))) return WWS_RET_ERR_TIMEOUT;
    }

    wws_event(WWS_COMP_EEPROM, WWS_EVT_WRITE, eeprom, reg_addr, &data[wlen], &wl);
    ret =
      wws_i2c_xfer(eeprom->bus,
                   dev_addr,
                   (wws_i2c_xfer_t[]){
                     { .cptr = reg_addr, .size = eeprom->schema->reg_len, .xfer = WWS_XFER_WRITE },
                     { .cptr = &data[wlen], .size = wl, .xfer = WWS_XFER_WRITE },
                     {},
                   },
                   WWS_MS(10));
    if (ret != WWS_RET_OK) break;
    wlen += wl;
    len -= wl;
  }

  if (eeprom->wc) wws_logic_write(eeprom->wc, WWS_HIGH);
  if (written) *written = wlen;

  return ret;
}

wws_ret_t
wws_eeprom_read(wws_eeprom_t *eeprom, unsigned int addr, unsigned int size, unsigned char *buf)
{
  wws_assert(eeprom && eeprom->schema && eeprom->bus && ((addr + size) <= eeprom->schema->size));

  const unsigned char reg_addr[REG_LEN_MAX] = WWS_8BITS_BE(REG_LEN_MAX, addr);

  return wws_i2c_xfer(
    eeprom->bus,
    _addr(eeprom),
    (wws_i2c_xfer_t[]){
      { .cptr = reg_addr, .size = eeprom->schema->reg_len, .xfer = WWS_XFER_WRITE },
      { .ptr = buf, .size = size, .xfer = WWS_XFER_READ },
      {},
    },
    WWS_MS(10));
}


static wws_ret_t mem_put8(void *inst, unsigned int addr, char data)
{
  return wws_eeprom_write(inst, addr, data, 1, 0);
}

static wws_ret_t
mem_write8(void *inst, unsigned int addr, const char *data, unsigned int len, unsigned int *written)
{
  return wws_eeprom_write(inst, addr, data, len, written);
}

static wws_ret_t mem_get8(void *inst, unsigned int addr, char *buf)
{
  return wws_eeprom_read(inst, addr, 1, buf);
}

static wws_ret_t
mem_read8(void *inst, unsigned int addr, unsigned int size, char *buf, unsigned int *buffered)
{
  return wws_eeprom_read(inst, addr, size, buf);
}

const wws_memory_inf_t wws_eeprom_memory_interface = {
  .put8   = mem_put8,
  .write8 = mem_write8,
  .get8   = mem_get8,
  .read8  = mem_read8,
};
