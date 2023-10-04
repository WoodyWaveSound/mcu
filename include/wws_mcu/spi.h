/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_SPI_H___
#define ___WWS_SPI_H___

#include "logic.h"

extern const char *WWS_COMP_SPI;
extern const char *WWS_EVT_START;
extern const char *WWS_EVT_XFER;
extern const char *WWS_EVT_STOP;

/**
 * @brief SPI device config
 */
typedef struct __wws_spi_dev_cfg_t
{
  /**
   * @brief chip select pin
   * @note remain 0 if use "module cs"
   */
  wws_logic_writer_t *const cs;
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
} wws_spi_dev_cfg_t;


/**
 * @brief SPI error
 */
typedef enum __wws_spi_err_t
{
  /**
   * @brief none error
   */
  WWS_SPI_ERR_OK,
  /**
   * @brief uncategorized error
   */
  WWS_SPI_ERR_OTHER,
} wws_spi_err_t;

/**
 * @brief Low level interface
 */
typedef struct __wws_spi_ll_t
{
  wws_spi_err_t (*start)(void *instance, wws_spi_dev_cfg_t *cfg);
  wws_spi_err_t (*exchange)(void          *instance,
                            unsigned char *data,
                            unsigned short len,
                            unsigned char *buf);
  wws_spi_err_t (*stop)(void *instance, wws_spi_dev_cfg_t *cfg);
} wws_spi_ll_t;

/**
 * @brief SPI bus
 */
typedef struct __wws_spi_t
{
  /**
   * @brief low level interface
   */
  wws_spi_ll_t *const ll;
  /**
   * @brief instance payload
   */
  void *const instance;
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
   * @brief config
   */
  wws_spi_dev_cfg_t cfg;
} wws_spi_dev_t;

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
 * @brief exchange batch of xfer message
 * @param dev
 * @param xfers array of xfer message
 * @return wws_spi_err_t
 */
extern wws_spi_err_t wws_spi_xfer_batch(wws_spi_dev_t *dev, wws_spi_xfer_t *xfers[]);

/**
 * @brief exchange xfer message
 * @param dev
 * @param xfer
 * @return wws_spi_err_t
 */
extern wws_spi_err_t wws_spi_xfer(wws_spi_dev_t *dev, wws_spi_xfer_t *const xfer);

#endif /* ___WWS_SPI_H___ */
