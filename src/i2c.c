/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#include <wws_mcu/i2c.h>
#include <wws_mcu/debug.h>
#include <wws_mcu/compiler.h>

const char          *WWS_COMP_I2C  = "I2C";
WWS_WEAK const char *WWS_EVT_WRITE = "Write";
WWS_WEAK const char *WWS_EVT_READ  = "Read";

static inline wws_i2c_err_t ll_is_ready(wws_i2c_t *i2c)
{
  wws_assert(i2c->ll->is_ready != 0);
  return i2c->ll->is_ready(i2c->instance) == 0 ? WWS_I2C_ERR_OK : WWS_I2C_ERR_BUSY;
}

static inline wws_i2c_err_t
ll_start(wws_i2c_t *i2c, unsigned short addr, wws_i2c_xfer_t xfer, unsigned int timeout)
{
  wws_assert(i2c->ll->start != 0);
  return i2c->ll->start(i2c->instance, addr, xfer, timeout);
}

static inline wws_i2c_err_t
ll_restart(wws_i2c_t *i2c, unsigned short addr, wws_i2c_xfer_t xfer, unsigned int timeout)
{
  wws_assert((i2c->ll->restart != 0) || (i2c->ll->start != 0));
  if (i2c->ll->restart) { return i2c->ll->restart(i2c->instance, addr, xfer, timeout); }
  return i2c->ll->start(i2c->instance, addr, xfer, timeout);
}

static inline wws_i2c_err_t ll_stop(wws_i2c_t *i2c, unsigned short addr, unsigned int timeout)
{
  wws_assert(i2c->ll->stop);
  return i2c->ll->stop(i2c->instance, addr, timeout);
}

static inline wws_i2c_err_t ll_put(wws_i2c_t *i2c, unsigned char byte, unsigned int timeout)
{
  wws_assert(i2c->ll->put);
  return i2c->ll->put(i2c->instance, byte, timeout);
}

static inline wws_i2c_err_t ll_get(wws_i2c_t *i2c, unsigned char *buf, unsigned int timeout)
{
  wws_assert(i2c->ll->get);
  return i2c->ll->get(i2c->instance, buf, timeout);
}


wws_i2c_err_t wws_i2c_test_device(wws_i2c_t *i2c, unsigned short addr, unsigned int timeout)
{
  wws_i2c_err_t err = WWS_I2C_ERR_OK;
  if (i2c->hal && i2c->hal->test) { err = i2c->hal->test(i2c->instance, addr, timeout); }
  else if (i2c->ll) {
    do {
      err = ll_is_ready(i2c);
      if (err != WWS_I2C_ERR_OK) break;
      err = ll_start(i2c, addr, WWS_I2C_XFER_WRITE, timeout);
      if (err != WWS_I2C_ERR_OK) break;
    } while (0);
    ll_stop(i2c, addr, timeout);
  }
  else {
    /** shouldn't here */
    err = WWS_I2C_ERR_OTHER;
  }
  return err;
}

wws_i2c_err_t wws_i2c_transfer(wws_i2c_t     *i2c,
                               unsigned short addr,
                               unsigned char *mem,
                               unsigned short mem_len,
                               unsigned char *data,
                               unsigned short data_len,
                               unsigned short buf_size,
                               unsigned char *buf,
                               unsigned int   timeout)
{
  wws_i2c_err_t err = WWS_I2C_ERR_OK;
  if (i2c->hal && i2c->hal->xfer) {
    err = i2c->hal->xfer(i2c->instance, addr, mem, mem_len, data, data_len, buf_size, buf, timeout);
  }
  else if (i2c->ll) {
    do {
      err = ll_is_ready(i2c);
      if (err != WWS_I2C_ERR_OK) break;
      if ((mem && mem_len) || (data && data_len)) {
        err = ll_start(i2c, addr, WWS_I2C_XFER_WRITE, timeout);
        if (err != WWS_I2C_ERR_OK) break;
        wws_event(WWS_COMP_I2C, WWS_EVT_WRITE, i2c, (void *) addr, mem, (void *) mem_len);
        for (unsigned short i = 0; i < mem_len; i++) {
          err = ll_put(i2c, mem[i], timeout);
          if (err != WWS_I2C_ERR_OK) break;
        }
        if (err != WWS_I2C_ERR_OK) break;
        wws_event(WWS_COMP_I2C, WWS_EVT_WRITE, i2c, (void *) addr, data, (void *) data_len);
        for (unsigned short i = 0; i < data_len; i++) {
          err = ll_put(i2c, data[i], timeout);
          if (err != WWS_I2C_ERR_OK) break;
        }
        if (err != WWS_I2C_ERR_OK) break;
      }
      if (buf && buf_size) {
        err = ll_start(i2c, addr, WWS_I2C_XFER_READ, timeout);
        if (err != WWS_I2C_ERR_OK) break;
        wws_event(WWS_COMP_I2C, WWS_EVT_READ, i2c, (void *) addr, (void *) buf_size, buf);
        for (unsigned short i = 0; i < buf_size; i++) {
          err = ll_get(i2c, buf + i, timeout);
          if (err != WWS_I2C_ERR_OK) break;
        }
        if (err != WWS_I2C_ERR_OK) break;
      }
    } while (0);
    ll_stop(i2c, addr, timeout);
  }
  else {
    /** shouldn't here */
    err = WWS_I2C_ERR_OTHER;
  }

  return err;
}

wws_i2c_err_t wws_i2c_write(wws_i2c_t     *i2c,
                            unsigned short addr,
                            unsigned char *data,
                            unsigned short len,
                            unsigned int   timeout)
{
  wws_i2c_err_t err = WWS_I2C_ERR_OK;
  if (i2c->hal && i2c->hal->write) {
    wws_event(WWS_COMP_I2C, WWS_EVT_WRITE, i2c, (void *) addr, data, (void *) len);
    err = i2c->hal->write(i2c->instance, addr, data, len, timeout);
  }
  else if (i2c->ll) {
    err = wws_i2c_transfer(i2c, addr, 0, 0, data, len, 0, 0, timeout);
  }
  else {
    /** shouldn't here */
    err = WWS_I2C_ERR_OTHER;
  }

  return err;
}

wws_i2c_err_t www_i2c_mem_write(wws_i2c_t     *i2c,
                                unsigned       addr,
                                unsigned char *mem,
                                unsigned short mem_len,
                                unsigned char *data,
                                unsigned short data_len,
                                unsigned int   timeout)
{
  wws_i2c_err_t err = WWS_I2C_ERR_OK;
  if (i2c->hal && i2c->hal->mem_write) {
    wws_event(WWS_COMP_I2C, WWS_EVT_WRITE, i2c, (void *) addr, mem, (void *) mem_len);
    wws_event(WWS_COMP_I2C, WWS_EVT_WRITE, i2c, (void *) addr, data, (void *) data_len);
    err = i2c->hal->mem_write(i2c->instance, addr, mem, mem_len, data, data_len, timeout);
  }
  else if (i2c->ll) {
    err = wws_i2c_transfer(i2c, addr, mem, mem_len, data, data_len, 0, 0, timeout);
  }
  else {
    /** shouldn't here */
    err = WWS_I2C_ERR_OTHER;
  }

  return err;
}

wws_i2c_err_t wws_i2c_read(wws_i2c_t     *i2c,
                           unsigned short addr,
                           unsigned short size,
                           unsigned char *buf,
                           unsigned int   timeout)
{
  wws_i2c_err_t err = WWS_I2C_ERR_OK;
  if (i2c->hal && i2c->hal->read) {
    wws_event(WWS_COMP_I2C, WWS_EVT_READ, i2c, (void *) addr, (void *) size, buf);
    err = i2c->hal->read(i2c->instance, addr, size, buf, timeout);
  }
  else if (i2c->ll) {
    err = wws_i2c_transfer(i2c, addr, 0, 0, 0, 0, size, buf, timeout);
  }
  else {
    /** shouldn't here */
    err = WWS_I2C_ERR_OTHER;
  }

  return err;
}

wws_i2c_err_t www_i2c_mem_read(wws_i2c_t     *i2c,
                               unsigned       addr,
                               unsigned char *mem,
                               unsigned short mem_len,
                               unsigned short buf_size,
                               unsigned char *buf,
                               unsigned int   timeout)
{
  wws_i2c_err_t err = WWS_I2C_ERR_OK;
  if (i2c->hal && i2c->hal->mem_read) {
    wws_event(WWS_COMP_I2C, WWS_EVT_WRITE, i2c, (void *) addr, mem, (void *) mem_len);
    wws_event(WWS_COMP_I2C, WWS_EVT_READ, i2c, (void *) addr, (void *) buf_size, buf);
    err = i2c->hal->mem_read(i2c->instance, addr, mem, mem_len, buf_size, buf, timeout);
  }
  else if (i2c->ll) {
    err = wws_i2c_transfer(i2c, addr, mem, mem_len, 0, 0, buf_size, buf, timeout);
  }
  else {
    /** shouldn't here */
    err = WWS_I2C_ERR_OTHER;
  }

  return err;
}
