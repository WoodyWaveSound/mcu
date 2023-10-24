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

WWS_WEAK wws_evt_t WWS_EVT_READ = "READ";

WWS_WEAK wws_ret_t WWS_RET_OK = "OK";

static inline unsigned short addr(wws_aw9523b_t *aw)
{
  return 0x58 | (aw->ad1 << 1) | (aw->ad0 << 0);
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
                   { .ptr = (unsigned char *){ 0x00 }, .size = 1, .xfer = WWS_XFER_WRITE },
                   { .ptr = &dev->regs[0], .size = 2, .xfer = WWS_XFER_READ },
                   { 0 },
                 },
                 WWS_MS(100));

  if (ret != WWS_RET_OK) wws_event(WWS_COMP_AW9523B, WWS_EVT_READ, &dev, ret);
}
