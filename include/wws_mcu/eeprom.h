/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_EEPROM_H___
#define ___WWS_EEPROM_H___

#include "typedef.h"
#include "logic.h"
#include "i2c.h"
#include "memory.h"

/**
 * @brief eeprom schema
 */
typedef struct __wws_eeprom_schema_t
{
  /**
   * @brief base address when all ADx are low
   */
  const unsigned short base_addr_7bit;
  /**
   * @brief page write size
   */
  const unsigned short page_size;
  /**
   * @brief length of register (generally 1 or 2)
   */
  const unsigned char reg_len;
  /**
   * @brief eeprom size in bytes
   */
  const unsigned int size;
} wws_eeprom_schema_t;

/**
 * @brief eeprom
 */
typedef struct __wws_eeprom_t
{
  /**
   * @brief schema of eeprom
   */
  const wws_eeprom_schema_t *const schema;
  /**
   * @brief i2c bus
   */
  wws_i2c_t *const bus;
  /**
   * @brief writer of write protect pin
   */
  wws_logic_writer_t *const wc;
  /**
   * @brief flags
   */
  union
  {
    unsigned int flags;
    struct
    {
      /**
       * @brief address select pins
       */
      unsigned int ad0 : 1;
      unsigned int ad1 : 1;
      unsigned int ad2 : 1;
      unsigned int ad3 : 1;
    };
  };
} wws_eeprom_t;

/** predefined schemas */
extern const wws_eeprom_schema_t wws_eeprom_AT24C128;
extern const wws_eeprom_schema_t wws_eeprom_M24C01_W;

/** debug */
extern wws_comp_t WWS_COMP_EEPROM;
extern wws_evt_t  WWS_EVT_WRITE;
extern wws_evt_t  WWS_EVT_READ;

/** return */
extern wws_ret_t WWS_RET_OK;

/**
 * @brief write into eeprom
 * @param eeprom
 * @param addr
 * @param data
 * @param len
 * @param written
 * @return
 */
extern wws_ret_t wws_eeprom_write(wws_eeprom_t        *eeprom,
                                  unsigned int         addr,
                                  const unsigned char *data,
                                  unsigned int         len,
                                  unsigned int        *written);


/**
 * @brief read from eeprom
 * @param eeprom
 * @param addr
 * @param size
 * @param buf
 * @param buffered
 * @return
 */
extern wws_ret_t
wws_eeprom_read(wws_eeprom_t *eeprom, unsigned int addr, unsigned int size, unsigned char *buf);

/**
 * @brief memory interface
 */
extern const wws_memory_inf_t wws_eeprom_memory_interface;

#endif /* ___WWS_EEPROM_H___ */
