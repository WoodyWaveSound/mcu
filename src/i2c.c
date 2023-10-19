/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#include <wws_mcu/i2c.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/compiler.h>

extern wws_comp_t  WWS_COMP_I2C;
WWS_WEAK wws_evt_t WWS_EVT_WRITE = "WRITE";
WWS_WEAK wws_evt_t WWS_EVT_READ  = "READ";

extern wws_xfer_t WWS_XFER_WRITE WWS_ALIAS(WWS_EVT_WRITE);
extern wws_xfer_t WWS_XFER_READ  WWS_ALIAS(WWS_EVT_READ);

WWS_WEAK wws_ret_t WWS_RET_OK          = "OK";
WWS_WEAK wws_ret_t WWS_RET_ERR_BUSY    = "ERR_BUSY";
WWS_WEAK wws_ret_t WWS_RET_ERR_NACK    = "ERR_NACK";
WWS_WEAK wws_ret_t WWS_RET_ERR_TIMEOUT = "ERR_TIMEOUT";
WWS_WEAK wws_ret_t WWS_RET_ERR_OTHER   = "ERR_OTHER";

wws_ret_t wws_i2c_test_device(wws_i2c_t *i2c, unsigned short addr, unsigned int timeout)
{
  wws_assert(i2c && i2c->interface);
  wws_ret_t ret = WWS_RET_OK;
  do {
    if ((ret = i2c->interface->is_ready(i2c->inst)) != WWS_RET_OK) break;
    if ((ret = i2c->interface->start(i2c->inst, addr, WWS_XFER_WRITE, timeout)) != WWS_RET_OK)
      break;
  } while (0);
  i2c->interface->stop(i2c->inst, addr, timeout);
  return ret;
}

wws_ret_t
wws_i2c_xfer(wws_i2c_t *i2c, unsigned short addr, wws_i2c_xfer_t xfers[], unsigned int timeout)
{
  wws_assert(i2c && i2c->interface);
  wws_ret_t ret = WWS_RET_OK;

  ret = i2c->interface->is_ready(i2c->inst);

  for (int i = 0; (ret == WWS_RET_OK) && (xfers[i].xfer != 0); i++) {
    wws_event(WWS_COMP_I2C, xfers[i].xfer, &xfers[i]);

    if (i && i2c->interface->restart) {
      if ((ret = i2c->interface->restart(i2c->inst, addr, xfers[i].xfer, timeout)) != WWS_RET_OK)
        break;
    }
    else {
      if ((ret = i2c->interface->start(i2c->inst, addr, xfers[i].xfer, timeout)) != WWS_RET_OK)
        break;
    }

    if (xfers[i].xfer == WWS_XFER_WRITE) {
      for (int d = 0; d < xfers[i].size; d++) {
        if ((ret = i2c->interface->put(i2c->inst, xfers[i].ptr[d], timeout)) != WWS_RET_OK) break;
      }
    }
    else if (xfers[i].xfer == WWS_XFER_READ) {
      for (int d = 0; d < xfers[i].size; d++) {
        if ((ret = i2c->interface->get(i2c->inst, &xfers[i].ptr[d], timeout)) != WWS_RET_OK) break;
      }
    }
  }

  i2c->interface->stop(i2c->inst, addr, timeout);
  return ret;
}
