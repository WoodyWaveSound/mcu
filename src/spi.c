/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/spi.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/compiler.h>

const char          *WWS_COMP_SPI  = "SPI";
WWS_WEAK const char *WWS_EVT_START = "Start";
WWS_WEAK const char *WWS_EVT_XFER  = "Xfer";
WWS_WEAK const char *WWS_EVT_STOP  = "Stop";

wws_spi_err_t wws_spi_xfer_batch(wws_spi_dev_t *dev, wws_spi_xfer_t *xfers[])
{
  wws_assert(dev && dev->spi && dev->spi->ll && xfers);

  wws_spi_err_t err = WWS_SPI_ERR_OK;

  do {
    wws_event(WWS_COMP_SPI, WWS_EVT_START, dev);
    err = dev->spi->ll->start(dev->spi->instance, &dev->cfg);
    if (err != WWS_SPI_ERR_OK) break;

    if (dev->cfg.cs) {
      wws_logic_write(dev->cfg.cs, WWS_LOW);
      wws_delay(dev->cfg.delay.start);
    }

    for (wws_spi_xfer_t *xfer = xfers[0]; xfer != 0; xfer++) {
      wws_event(WWS_COMP_SPI, WWS_EVT_XFER, dev, xfer);
      err = dev->spi->ll->exchange(dev->spi->instance, xfer->data, xfer->len, xfer->buf);
      if (err != WWS_SPI_ERR_OK) break;
    }
  } while (0);

  wws_event(WWS_COMP_SPI, WWS_EVT_STOP, dev);
  if (dev->cfg.cs) {
    wws_logic_write(dev->cfg.cs, WWS_HIGH);
    wws_delay(dev->cfg.delay.stop);
  }
  dev->spi->ll->stop(dev->spi->instance, &dev->cfg);

  return err;
}

wws_spi_err_t wws_spi_xfer(wws_spi_dev_t *dev, wws_spi_xfer_t *const xfer)
{
  wws_assert(xfer);

  return wws_spi_xfer_batch(dev, (wws_spi_xfer_t *[]){ xfer, 0 });
}
