/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_SPI_H___
#define ___WWS_SPI_H___

#include "typedef.h"
#include "logic.h"

extern wws_comp_t WWS_COMP_SPI;
extern wws_evt_t  WWS_EVT_START;
extern wws_evt_t  WWS_EVT_XFER;
extern wws_evt_t  WWS_EVT_STOP;

extern wws_ret_t WWS_RET_OK;
extern wws_ret_t WWS_RET_ERR_OTHER;

/**
 * @brief SPI device config
 */
typedef struct __wws_spi_cfg_t
{

  /**
   * @brief delay between cs change and data transmit
   */
  struct
  {
    /**
     * @brief after cs low and data transfer
     */
    unsigned int start;
    /**
     * @brief after data transfer and cs high
     */
    unsigned int stop;
  } delay;
  /**
   * @brief clock speed in hz
   */
  const unsigned int speed;
  /**
   * @brief Clock polarity (idle = ?)
   */
  const unsigned char cpol;
  /**
   * @brief Clock phase, edge to read value
   * - 0
   *    - CPOL = 0: Rising
   *    - CPOL = 1: Falling
   * - 1
   *    - CPOL = 0: Falling
   *    - CPOL = 1: Rising
   */
  const unsigned char cpha;
  /**
   * @brief dummy byte for write
   */
  const unsigned char dummy;
} wws_spi_cfg_t;


/**
 * @brief SPI message for batch
 */
typedef struct __wws_spi_xfer_t
{
  /**
   * @brief data to write
   */
  unsigned char *data;
  /**
   * @brief len to transfer
   */
  unsigned short len;
  /**
   * @brief buffer to read
   */
  unsigned char *buf;
} wws_spi_xfer_t;

/**
 * @brief Low level interface
 */
typedef struct __wws_spi_inf_t
{
  wws_ret_t (*start)(void *inst, wws_spi_cfg_t *cfg);
  wws_ret_t (*exchange)(void *inst, wws_spi_cfg_t *cfg, wws_spi_xfer_t *xfer);
  wws_ret_t (*stop)(void *inst, wws_spi_cfg_t *cfg);
} wws_spi_inf_t;

/**
 * @brief SPI bus
 */
typedef struct __wws_spi_t
{
  /**
   * @brief low level interface
   */
  wws_spi_inf_t *const interface;
  /**
   * @brief inst inst
   */
  void *const inst;
} wws_spi_t;

/**
 * @brief SPI device
 */
typedef struct __wws_spi_dev_t
{
  /**
   * @brief SPI bus
   */
  wws_spi_t *const spi;
  /**
   * @brief chip select pin
   * @note remain 0 if use "module cs"
   */
  wws_logic_writer_t *const cs;
  /**
   * @brief config
   */
  wws_spi_cfg_t cfg;
} wws_spi_dev_t;


/**
 * @brief exchange batch of xfer message
 * @param dev
 * @param xfers array of xfer message
 * @return wws_ret_t
 */
extern wws_ret_t wws_spi_xfer(wws_spi_dev_t *dev, wws_spi_xfer_t xfers[]);


#endif /* ___WWS_SPI_H___ */
