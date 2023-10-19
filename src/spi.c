/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/spi.h>
#include <wws_mcu/debug.h>

wws_comp_t         WWS_COMP_SPI  = "SPI";
WWS_WEAK wws_evt_t WWS_EVT_START = "START";
WWS_WEAK wws_evt_t WWS_EVT_XFER  = "XFER";
WWS_WEAK wws_evt_t WWS_EVT_STOP  = "STOP";

WWS_WEAK wws_ret_t WWS_RET_OK        = "OK";
WWS_WEAK wws_ret_t WWS_RET_ERR_OTHER = "ERR_OTHER";


wws_ret_t wws_spi_xfer_batch(wws_spi_dev_t *dev, wws_spi_xfer_t xfers[])
{
  wws_assert(dev && dev->spi && dev->spi->interface && xfers);

  wws_ret_t ret = WWS_RET_OK;

  wws_event(WWS_COMP_SPI, WWS_EVT_START, dev);
  ret = dev->spi->interface->start(dev->spi->inst, &dev->cfg);
  if ((ret == WWS_RET_OK) && dev->cs) {
    wws_logic_write(dev->cs, WWS_LOW);
    wws_delay(dev->cfg.delay.start);
  }

  for (int i = 0; (ret == WWS_RET_OK) && xfers[i].len; i++) {
    wws_event(WWS_COMP_SPI, WWS_EVT_XFER, dev, &xfers[i]);
    ret = dev->spi->interface->exchange(dev->spi->inst, &dev->cfg, &xfers[i]);
  }

  wws_event(WWS_COMP_SPI, WWS_EVT_STOP, dev);
  if (dev->cs) {
    wws_logic_write(dev->cs, WWS_HIGH);
    wws_delay(dev->cfg.delay.stop);
  }
  dev->spi->interface->stop(dev->spi->inst, &dev->cfg);

  return ret;
}
