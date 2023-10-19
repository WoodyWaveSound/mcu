/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_MEMORY_H___
#define ___WWS_MEMORY_H___

#include "typedef.h"

extern wws_comp_t WWS_COMP_MEMORY;

extern wws_ret_t WWS_RET_OK;
extern wws_ret_t WWS_RET_ERR_OVERSIZE;
extern wws_ret_t WWS_RET_ERR_NO_DATA;

/**
 * @brief Memory interface
 */
typedef struct __wws_memory_inf_t
{
  /**
   * @brief put 1 byte
   */
  wws_ret_t (*put8)(void *inst, unsigned int addr, char data);
  /**
   * @brief put 2 byte
   */
  wws_ret_t (*put16)(void *inst, unsigned int addr, short data);
  /**
   * @brief put 4 byte
   */
  wws_ret_t (*put32)(void *inst, unsigned int addr, int data);
  /**
   * @brief write in 1 bytes
   */
  wws_ret_t (*write8)(
    void *inst, unsigned int addr, const char *data, unsigned int len, unsigned int *written);
  /**
   * @brief write in 2 bytes
   */
  wws_ret_t (*write16)(
    void *inst, unsigned int addr, const short *data, unsigned int len, unsigned int *written);
  /**
   * @brief write in 4 bytes
   */
  wws_ret_t (*write32)(
    void *inst, unsigned int addr, const int *data, unsigned int len, unsigned int *written);
  /**
   * @brief get 1 byte
   */
  wws_ret_t (*get8)(void *inst, unsigned int addr, char *buf);
  /**
   * @brief get 2 byte
   */
  wws_ret_t (*get16)(void *inst, unsigned int addr, short *buf);
  /**
   * @brief get 3 byte
   */
  wws_ret_t (*get32)(void *inst, unsigned int addr, int *buf);
  /**
   * @brief read in 1 byte
   */
  wws_ret_t (*read8)(
    void *inst, unsigned int addr, unsigned int size, char *buf, unsigned int *buffered);
  /**
   * @brief read in 2 bytes
   */
  wws_ret_t (*read16)(
    void *inst, unsigned int addr, unsigned int size, short *buf, unsigned int *buffered);
  /**
   * @brief read in 4 bytes
   */
  wws_ret_t (*read32)(
    void *inst, unsigned int addr, unsigned int size, int *buf, unsigned int *buffered);
} wws_memory_inf_t;

/**
 * @brief memory
 */
typedef struct __wws_memory_t
{
  /**
   * @brief interface
   */
  wws_memory_inf_t *const interface;
  /**
   * @brief instance
   */
  void *const inst;
  /**
   * @brief address base
   */
  const unsigned int base;
  /**
   * @brief size of memory region
   */
  const unsigned int size;
} wws_memory_t;

/**
 * @brief put 1 byte
 */
extern wws_ret_t wws_memory_put8(wws_memory_t *m, unsigned int offset, char data);
/**
 * @brief put 2 byte
 */
extern wws_ret_t wws_memory_put16(wws_memory_t *m, unsigned int offset, short data);
/**
 * @brief put 4 byte
 */
extern wws_ret_t wws_memory_put32(wws_memory_t *m, unsigned int offset, int data);
/**
 * @brief write in 1 bytes
 */
extern wws_ret_t wws_memory_write8(
  wws_memory_t *m, unsigned int offset, const char *data, unsigned int len, unsigned int *written);
/**
 * @brief write in 2 bytes
 */
extern wws_ret_t wws_memory_write16(
  wws_memory_t *m, unsigned int offset, const short *data, unsigned int len, unsigned int *written);
/**
 * @brief write in 4 bytes
 */
extern wws_ret_t wws_memory_write32(
  wws_memory_t *m, unsigned int offset, const int *data, unsigned int len, unsigned int *written);
/**
 * @brief get 1 byte
 */
extern wws_ret_t wws_memory_get8(wws_memory_t *m, unsigned int offset, char *buf);
/**
 * @brief get 2 byte
 */
extern wws_ret_t wws_memory_get16(wws_memory_t *m, unsigned int offset, short *buf);
/**
 * @brief get 3 byte
 */
extern wws_ret_t wws_memory_get32(wws_memory_t *m, unsigned int offset, int *buf);
/**
 * @brief read in 1 byte
 */
extern wws_ret_t wws_memory_read8(
  wws_memory_t *m, unsigned int offset, unsigned int size, char *buf, unsigned int *buffered);
/**
 * @brief read in 2 bytes
 */
extern wws_ret_t wws_memory_read16(
  wws_memory_t *m, unsigned int offset, unsigned int size, short *buf, unsigned int *buffered);
/**
 * @brief read in 4 bytes
 */
extern wws_ret_t wws_memory_read32(
  wws_memory_t *m, unsigned int offset, unsigned int size, int *buf, unsigned int *buffered);

#endif /* ___WWS_MEMORY_H___ */