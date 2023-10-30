/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/aw9523b.h>
#include <wws_mcu/time.h>
#include <wws_mcu/debug.h>

wws_comp_t WWS_COMP_AW9523B = "AW9523B";

WWS_WEAK wws_evt_t WWS_EVT_READ  = "READ";
WWS_WEAK wws_evt_t WWS_EVT_WRITE = "WRITE";

WWS_WEAK wws_ret_t WWS_RET_OK           = "OK";
WWS_WEAK wws_ret_t WWS_RET_ERR_INVALID  = "ERR_INVALID";
WWS_WEAK wws_ret_t WWS_RET_ERR_NOT_INIT = "ERR_NOT_INIT ";

enum
{
  REG_INPUT    = 0,
  REG_OUTPUT   = REG_INPUT + 2,
  REG_DIR      = REG_OUTPUT + 2,
  REG_CTL      = REG_DIR + 2,
  REG_PIN_MODE = REG_CTL + 1,
  REG_DIM      = REG_PIN_MODE + 2,
};

static inline unsigned short addr(wws_aw9523b_t *dev)
{
  return 0x58 | (dev->ad1 << 1) | (dev->ad0 << 0);
}

static inline wws_ret_t
reg_write(wws_aw9523b_t *dev, unsigned char reg_addr, unsigned char reg, unsigned int size)
{
  return wws_i2c_xfer(dev->bus,
                      addr(dev),
                      (wws_i2c_xfer_t[]){
                        { .ptr = &reg_addr, .size = 1, .xfer = WWS_XFER_WRITE },
                        { .ptr = &dev->regs[reg], .size = size, .xfer = WWS_XFER_WRITE },
                        {},
                      },
                      WWS_MS(100));
}

void ___wws_aw9523b_input_service_callback(wws_phase_t on, wws_service_t *serv)
{
  if (on != WWS_ON_ROUTINE) return;

  wws_aw9523b_t   *dev = serv->inst;
  wws_countdown_t *cd  = serv->context;

  if (dev->init == 0) return;

  if (dev->interrupt) {
    /** return if no interrupt */
    if (wws_logic_read(dev->interrupt) == WWS_LOW) return;
  }
  else {
    /** polling in 100ms if no interrupt */
    WWS_SERVICE_INTERVAL(cd, WWS_MS(100));
  }

  wws_ret_t ret =
    wws_i2c_xfer(dev->bus,
                 addr(dev),
                 (wws_i2c_xfer_t[]){
                   { .cptr = (const unsigned char *){ 0x00 }, .size = 1, .xfer = WWS_XFER_WRITE },
                   { .ptr = &dev->regs[0], .size = 2, .xfer = WWS_XFER_READ },
                   {},
                 },
                 WWS_MS(100));

  if (ret != WWS_RET_OK) wws_event(WWS_COMP_AW9523B, WWS_EVT_READ, &dev, ret);
}

wws_ret_t wws_aw9523b_init(wws_aw9523b_t *dev)
{
  wws_assert(dev && dev->bus);
  if (dev->init) return WWS_RET_OK;

  wws_ret_t ret = WWS_RET_OK;

  if (dev->reset) {
    wws_logic_write(dev->reset, WWS_LOW);
    wws_delay(WWS_MS(10));
    wws_logic_write(dev->reset, WWS_HIGH);
    wws_delay(WWS_MS(50));
  }

  ret = wws_i2c_test_device(dev->bus, addr(dev), WWS_MS(100));
  if (ret != WWS_RET_OK) return ret;

  /** try to read id */
  static const unsigned char id_addr = 0x10;
  unsigned char              id      = 0;
  ret                                = wws_i2c_xfer(dev->bus,
                     addr(dev),
                     (wws_i2c_xfer_t[]){
                       { .cptr = &id_addr, .size = 1, .xfer = WWS_XFER_WRITE },
                       { .ptr = &id, .size = 1, .xfer = WWS_XFER_READ },
                       {},
                     },
                     WWS_MS(100));
  if (ret != WWS_RET_OK) return ret;
  if (id != 0x23) return WWS_RET_ERR_INVALID;

  /** set default value */
  dev->regs[REG_PIN_MODE]     = 0xFF;
  dev->regs[REG_PIN_MODE + 1] = 0xFF;

  dev->init = 1;

  return ret;
}

wws_ret_t wws_aw9523b_deinit(wws_aw9523b_t *dev)
{
  if (dev->init == 0) return WWS_RET_OK;
  if (dev->reset) wws_logic_write(dev->reset, WWS_LOW);

  dev->init = 0;
  return WWS_RET_OK;
}

wws_ret_t wws_aw9523b_set_current_limit(wws_aw9523b_t *dev, wws_conf_t conf)
{
  if (dev->init == 0) return WWS_RET_ERR_NOT_INIT;

  dev->regs[REG_CTL] &= ~0x3;
  if (conf == WWS_CONF_CURRENT_LIMIT_9mA) dev->regs[REG_CTL] |= 0x3;
  else if (conf == WWS_CONF_CURRENT_LIMIT_18mA) dev->regs[REG_CTL] |= 0x2;
  else if (conf == WWS_CONF_CURRENT_LIMIT_27mA) dev->regs[REG_CTL] |= 0x1;
  else if (conf == WWS_CONF_CURRENT_LIMIT_37mA) dev->regs[REG_CTL] |= 0x0;

  return reg_write(dev, 0x11, REG_CTL, 1);
}

wws_ret_t wws_aw9523b_set_p0_mode(wws_aw9523b_t *dev, wws_conf_t conf)
{
  if (dev->init == 0) return WWS_RET_ERR_NOT_INIT;

  dev->regs[REG_CTL] &= ~(1U << 4);
  if (conf == WWS_CONF_PUSH_PULL) dev->regs[REG_CTL] |= (1U << 4);

  return reg_write(dev, 0x11, REG_CTL, 1);
}

wws_ret_t wws_aw9523b_set_pending(wws_aw9523b_t *dev, wws_phase_t pending)
{
  if (dev->init == 0) return WWS_RET_ERR_NOT_INIT;

  if (pending == WWS_PENDING_OUTPUT) dev->pending_output = 1;
  else if (pending == WWS_PENDING_DIM) dev->pending_dim = 1;

  return WWS_RET_OK;
}

wws_ret_t wws_aw9523b_apply_pending(wws_aw9523b_t *dev, wws_phase_t pending)
{
  if (dev->init == 0) return WWS_RET_ERR_NOT_INIT;

  wws_ret_t ret = WWS_RET_OK;

  if (dev->pending_output) {
    ret = reg_write(dev, 0x02, REG_OUTPUT, 2);
    if (ret != WWS_RET_OK) return ret;
    dev->pending_output = 0;
  }

  if (dev->pending_dim) {
    ret = reg_write(dev, 0x20, REG_DIM, 16);
    if (ret != WWS_RET_OK) return ret;
    dev->pending_dim = 0;
  }

  return ret;
}

wws_ret_t wws_aw9523b_set_pins(wws_aw9523b_t *dev, unsigned int mask, wws_conf_t conf)
{
  if (dev->init == 0) return WWS_RET_ERR_NOT_INIT;

  if (conf == WWS_CONF_INPUT) {
    dev->regs[REG_DIR] |= (mask & 0xFF);
    dev->regs[REG_DIR + 1] |= ((mask >> 8) & 0xFF);
    dev->regs[REG_PIN_MODE] |= (mask & 0xFF);
    dev->regs[REG_PIN_MODE + 1] |= ((mask >> 8) & 0xFF);
  }
  else if (conf == WWS_CONF_OUTPUT) {
    dev->regs[REG_DIR] &= ~(mask & 0xFF);
    dev->regs[REG_DIR + 1] &= ~((mask >> 8) & 0xFF);
    dev->regs[REG_PIN_MODE] |= (mask & 0xFF);
    dev->regs[REG_PIN_MODE + 1] |= ((mask >> 8) & 0xFF);
  }
  else if (conf == WWS_CONF_LED) {
    dev->regs[REG_PIN_MODE] &= ~(mask & 0xFF);
    dev->regs[REG_PIN_MODE + 1] &= ~((mask >> 8) & 0xFF);
  }

  wws_ret_t ret = reg_write(dev, 0x04, REG_DIR, 2);
  if (ret != WWS_RET_OK) return ret;
  ret = reg_write(dev, 0x12, REG_PIN_MODE, 2);
  return ret;
}

wws_logic_t wws_aw9523b_read(wws_aw9523b_t *dev, unsigned int pin)
{
  return ((dev->regs[REG_INPUT] & (pin & 0xFF)) |
          (dev->regs[REG_INPUT + 1] & ((pin >> 8) & 0xFF))) > 0 ?
           WWS_HIGH :
           WWS_LOW;
}

static const unsigned char pin_dim_map[] = {
  REG_DIM + 4,  REG_DIM + 5,  REG_DIM + 6,  REG_DIM + 7,  REG_DIM + 8, REG_DIM + 9,
  REG_DIM + 10, REG_DIM + 11, REG_DIM + 0,  REG_DIM + 1,  REG_DIM + 2, REG_DIM + 3,
  REG_DIM + 12, REG_DIM + 13, REG_DIM + 14, REG_DIM + 15,
};

unsigned char wws_aw9523b_read_dim(wws_aw9523b_t *dev, unsigned int pin)
{
  for (unsigned char i = 0; i < 16; i++) {
    if (pin & (1U << i)) return dev->regs[pin_dim_map[i]];
  }
  return 0;
}

wws_ret_t wws_aw9523b_write(wws_aw9523b_t *dev, unsigned int pins, wws_logic_t logic)
{
  if (dev->init == 0) return WWS_RET_ERR_NOT_INIT;

  if (logic == WWS_HIGH) {
    dev->regs[REG_OUTPUT] |= (pins & 0xFF);
    dev->regs[REG_OUTPUT + 1] |= ((pins >> 8) & 0xFF);
  }
  else {
    dev->regs[REG_OUTPUT] &= ~(pins & 0xFF);
    dev->regs[REG_OUTPUT + 1] &= ~((pins >> 8) & 0xFF);
  }

  if (dev->pending_output) return WWS_RET_OK;

  return reg_write(dev, 0x02, REG_OUTPUT, 2);
}

wws_ret_t wws_aw9523b_write_dim(wws_aw9523b_t *dev, unsigned int pins, unsigned char dim)
{
  if (dev->init == 0) return WWS_RET_ERR_NOT_INIT;
  for (unsigned char i = 0; i < 16; i++) {
    if (pins & (1U << i)) dev->regs[pin_dim_map[i]] = dim;
  }

  if (dev->pending_dim) return WWS_RET_OK;

  return reg_write(dev, 0x20, REG_DIM, 16);
}

wws_logic_t wws_aw9523b_logic_reader(void *inst)
{
  return wws_aw9523b_pin_read(inst);
}

void wws_aw9523b_logic_writer(void *inst, wws_logic_t logic)
{
  wws_ret_t ret = wws_aw9523b_pin_write(inst, logic);
  if (ret != WWS_RET_OK) wws_event(WWS_COMP_AW9523B, WWS_EVT_WRITE, inst, &logic, ret);
}
