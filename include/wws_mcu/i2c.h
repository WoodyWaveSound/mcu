/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_I2C_H___
#define ___WWS_I2C_H___

/**
 * @brief i2c write/read direction
 */
typedef enum __wws_i2c_xfer_t
{
  /**
   * @brief Write
   */
  WWS_I2C_XFER_WRITE,
  /**
   * @brief Read
   */
  WWS_I2C_XFER_READ,
} wws_i2c_xfer_t;

/**
 * @brief I2C error
 */
typedef enum __wws_i2c_err_t
{
  /**
   * @brief no error
   */
  WWS_I2C_ERR_OK,
  /**
   * @brief bus busy
   */
  WWS_I2C_ERR_BUSY,
  /**
   * @brief Nack
   */
  WWS_I2C_ERR_NACK,
  /**
   * @brief timeout
   */
  WWS_I2C_ERR_TIMEOUT,
  /**
   * @brief uncategorized error
   */
  WWS_I2C_ERR_OTHER,
} wws_i2c_err_t;

/**
 * @brief I2C low level interface
 */
typedef struct __wws_i2c_low_level_t
{
  /**
   * @brief is bus ready?
   * @return 0: ready, 1: busy
   */
  int (*const is_ready)(void *payload);
  /**
   * @brief Send start to bus
   */
  wws_i2c_err_t (*const start)(void          *payload,
                               unsigned short addr,
                               wws_i2c_xfer_t xfer,
                               unsigned int   timeout);
  /**
   * @brief Send restart to bus
   * @note if NULL, will use start
   */
  wws_i2c_err_t (*const restart)(void          *payload,
                                 unsigned short addr,
                                 wws_i2c_xfer_t xfer,
                                 unsigned int   timeout);
  /**
   * @brief Send stop to bus
   */
  wws_i2c_err_t (*const stop)(void *payload, unsigned short addr, unsigned int timeout);
  /**
   * @brief Put data to bus
   */
  wws_i2c_err_t (*const put)(void *payload, unsigned char byte, unsigned int timeout);
  /**
   * @brief Get data from bus
   */
  wws_i2c_err_t (*const get)(void *payload, unsigned char *buf, unsigned int timeout);
} wws_i2c_low_level_t;

/**
 * @brief i2c HAL interface
 */
typedef struct __wws_i2c_hal_t
{
  /**
   * @brief test device
   */
  wws_i2c_err_t (*const test)(void *payload, unsigned short addr, unsigned int timeout);
  /**
   * @brief common transfer
   */
  wws_i2c_err_t (*const xfer)(void          *payload,
                              unsigned short addr,
                              unsigned char *mem,
                              unsigned short mem_len,
                              unsigned char *data,
                              unsigned short data_len,
                              unsigned short buf_size,
                              unsigned char *buf,
                              unsigned int   timeout);
  /**
   * @brief common write
   * @note if NULL, use xfer
   */
  wws_i2c_err_t (*const write)(wws_i2c_t     *i2c,
                               unsigned short addr,
                               unsigned char *data,
                               unsigned short len,
                               unsigned int   timeout);
  /**
   * @brief common mem write
   * @note if NULL, use write
   */
  wws_i2c_err_t (*const mem_write)(void          *payload,
                                   unsigned       addr,
                                   unsigned char *mem,
                                   unsigned short mem_len,
                                   unsigned      *data,
                                   unsigned short data_len,
                                   unsigned int   timeout);
  /**
   * @brief common read
   * @note if NULL, use xfer
   */
  wws_i2c_err_t (*const read)(void          *payload,
                              unsigned short addr,
                              unsigned short size,
                              unsigned char *buf,
                              unsigned int   timeout);
  /**
   * @brief common mem read
   * @note if NULL, use read
   */
  wws_i2c_err_t (*const mem_read)(void          *payload,
                                  unsigned       addr,
                                  unsigned char *mem,
                                  unsigned short mem_len,
                                  unsigned short buf_size,
                                  unsigned      *buf,
                                  unsigned int   timeout);
} wws_i2c_hal_t;

/**
 * @brief i2c handler
 */
typedef struct __wws_i2c_t
{
  /**
   * @brief low level interface
   */
  wws_i2c_low_level_t *const ll;
  /**
   * @brief HAL interface
   */
  wws_i2c_hal_t *const hal;
  /**
   * @brief payload data of instance
   */
  void *const payload;
} wws_i2c_t;

/**
 * @brief Test device status
 * @param i2c
 * @param addr
 * @param timeout
 * @return wws_i2c_err_t
 */
extern wws_i2c_err_t wws_i2c_test_device(wws_i2c_t *i2c, unsigned short addr, unsigned int timeout);

/**
 * @brief transfer data
 * @param i2c
 * @param addr
 * @param data data to write
 * @param data_len data length to write
 * @param buf_size buffer size to read
 * @param buf buf to store
 * @param timeout
 * @return wws_i2c_err_t
 */
extern wws_i2c_err_t wws_i2c_transfer(wws_i2c_t     *i2c,
                                      unsigned short addr,
                                      unsigned char *mem,
                                      unsigned short mem_len,
                                      unsigned char *data,
                                      unsigned short data_len,
                                      unsigned short buf_size,
                                      unsigned char *buf,
                                      unsigned int   timeout);

/**
 * @brief write data
 * @param i2c
 * @param addr
 * @param data
 * @param len
 * @param timeout
 * @return wws_i2c_err_t
 */
extern wws_i2c_err_t wws_i2c_write(wws_i2c_t     *i2c,
                                   unsigned short addr,
                                   unsigned char *data,
                                   unsigned short len,
                                   unsigned int   timeout);

/**
 * @brief write data to mem
 * @param i2c
 * @param addr
 * @param mem
 * @param mem_len
 * @param data
 * @param data_len
 * @param timeout
 * @return
 */
extern wws_i2c_err_t www_i2c_mem_write(wws_i2c_t     *i2c,
                                       unsigned       addr,
                                       unsigned char *mem,
                                       unsigned short mem_len,
                                       unsigned      *data,
                                       unsigned short data_len,
                                       unsigned int   timeout);


/**
 * @brief read data
 * @param i2c
 * @param addr
 * @param size
 * @param buf
 * @param timeout
 * @return wws_i2c_err_t
 */
extern wws_i2c_err_t wws_i2c_read(wws_i2c_t     *i2c,
                                  unsigned short addr,
                                  unsigned short size,
                                  unsigned char *buf,
                                  unsigned int   timeout);

/**
 * @brief read data from mem
 * @param i2c
 * @param addr
 * @param mem
 * @param mem_len
 * @param buf_size
 * @param buf
 * @param timeout
 * @return wws_i2c_err_t
 */
extern wws_i2c_err_t www_i2c_mem_read(wws_i2c_t     *i2c,
                                      unsigned       addr,
                                      unsigned char *mem,
                                      unsigned short mem_len,
                                      unsigned short buf_size,
                                      unsigned      *buf,
                                      unsigned int   timeout);

#endif /* ___WWS_I2C_H___ */
