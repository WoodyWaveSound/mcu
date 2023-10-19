/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_I2C_H___
#define ___WWS_I2C_H___

#include "typedef.h"

extern wws_comp_t WWS_COMP_I2C;
extern wws_evt_t  WWS_EVT_WRITE;
extern wws_evt_t  WWS_EVT_READ;

extern wws_xfer_t WWS_XFER_WRITE;
extern wws_xfer_t WWS_XFER_READ;

extern wws_ret_t WWS_RET_OK;
extern wws_ret_t WWS_RET_ERR_BUSY;
extern wws_ret_t WWS_RET_ERR_NACK;
extern wws_ret_t WWS_RET_ERR_TIMEOUT;
extern wws_ret_t WWS_RET_ERR_OTHER;

/**
 * @brief I2C low level interface
 */
typedef struct __wws_i2c_inf_t
{
  /**
   * @brief is bus ready?
   */
  wws_ret_t (*const is_ready)(void *inst);
  /**
   * @brief Send start to bus
   */
  wws_ret_t (*const start)(void *inst, unsigned short addr, wws_xfer_t xfer, unsigned int timeout);
  /**
   * @brief Send restart to bus
   * @note if NULL, will use start
   */
  wws_ret_t (*const restart)(void *inst, unsigned short addr, wws_xfer_t xfer, unsigned int timeout);
  /**
   * @brief Send stop to bus
   */
  void (*const stop)(void *inst, unsigned short addr, unsigned int timeout);
  /**
   * @brief Put data to bus
   */
  wws_ret_t (*const put)(void *inst, unsigned char byte, unsigned int timeout);
  /**
   * @brief Get data from bus
   */
  wws_ret_t (*const get)(void *inst, unsigned char *buf, unsigned int timeout);
} wws_i2c_inf_t;


/**
 * @brief i2c handler
 */
typedef struct __wws_i2c_t
{
  /**
   * @brief low level interface
   */
  wws_i2c_inf_t *const interface;
  /**
   * @brief inst data of inst
   */
  void *const inst;
} wws_i2c_t;

/**
 * @brief Test device status
 * @param i2c
 * @param addr
 * @param timeout
 * @return wws_ret_t
 */
extern wws_ret_t wws_i2c_test_device(wws_i2c_t *i2c, unsigned short addr, unsigned int timeout);

/**
 * @brief xfer definition
 */
typedef struct __wws_i2c_xfer_t
{
  unsigned char *ptr;
  unsigned short size;
  wws_xfer_t     xfer;
} wws_i2c_xfer_t;

extern wws_ret_t
wws_i2c_xfer(wws_i2c_t *i2c, unsigned short addr, wws_i2c_xfer_t xfers[], unsigned int timeout);

#endif /* ___WWS_I2C_H___ */
