/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_AW9523B_H___
#define ___WWS_AW9523B_H___

#include "typedef.h"
#include "logic.h"
#include "i2c.h"
#include "service.h"
#include "countdown.h"

extern wws_comp_t WWS_COMP_AW9523B;

extern wws_evt_t WWS_EVT_READ;

extern wws_ret_t WWS_RET_OK;
extern wws_ret_t WWS_RET_ERR_INVALID;
extern wws_ret_t WWS_RET_ERR_NOT_INIT;

/**
 * @brief aw9523b driver
 */
typedef struct __wws_aw9523b_t
{
  /**
   * @brief address pins
   */
  wws_logic_t ad0, ad1;
  /**
   * @brief i2c bus
   */
  wws_i2c_t *const bus;
  /**
   * @brief pin of reset
   */
  wws_logic_writer_t *const reset;
  /**
   * @brief pin of interrupt
   */
  wws_logic_reader_t *const interrupt;
  /**
   * @brief registers
   */
  unsigned char regs[25];
  /**
   * @brief flags
   */
  union
  {
    unsigned int flags;
    struct
    {
      unsigned int init : 1;
      unsigned int pending_output : 1;
      unsigned int pending_dim : 1;
    };
  };
} wws_aw9523b_t;

extern void ___wws_aw9523b_input_service_callback(wws_phase_t on, wws_service_t *serv);

/**
 * @brief service of AW9523B for input read
 */
#define WWS_AW9523B_INPUT_SERVICE                                                                  \
  .callback = ___wws_aw9523b_input_service_callback, .default_start = 1,                           \
  .context = (void *) &(wws_countdown_t)                                                           \
  {                                                                                                \
  }


/**
 * @brief init aw9523b
 * @param dev
 * @return
 */
extern wws_ret_t wws_aw9523b_init(wws_aw9523b_t *dev);

/**
 * @brief deinit aw9523b
 * @param dev
 * @return
 */
extern wws_ret_t wws_aw9523b_deinit(wws_aw9523b_t *dev);

/**
 * @brief Current limits
 */
extern wws_conf_t WWS_CONF_CURRENT_LIMIT_9mA;
extern wws_conf_t WWS_CONF_CURRENT_LIMIT_18mA;
extern wws_conf_t WWS_CONF_CURRENT_LIMIT_27mA;
extern wws_conf_t WWS_CONF_CURRENT_LIMIT_37mA;

/**
 * @brief set current limits
 * @param dev
 * @param conf
 * @return
 */
extern wws_ret_t wws_aw9523b_set_current_limit(wws_aw9523b_t *dev, wws_conf_t conf);

/**
 * @brief P0 mode
 */
extern wws_conf_t WWS_CONF_OPEN_DRAIN;
extern wws_conf_t WWS_CONF_PUSH_PULL;

/**
 * @brief set p0 mode
 * @param dev
 * @param conf
 * @return
 */
extern wws_ret_t wws_aw9523b_set_p0_mode(wws_aw9523b_t *dev, wws_conf_t conf);

/**
 * @brief pending options
 */
extern wws_phase_t WWS_PENDING_OUTPUT;
extern wws_phase_t WWS_PENDING_DIM;

/**
 * @brief set pending for specific registers
 * @param dev
 * @param pending
 * @return
 */
extern wws_ret_t wws_aw9523b_set_pending(wws_aw9523b_t *dev, wws_phase_t pending);

/**
 * @brief apply pending for specific registers
 * @param dev
 * @param pending
 * @return
 */
extern wws_ret_t wws_aw9523b_apply_pending(wws_aw9523b_t *dev, wws_phase_t pending);

/**
 * @brief AW9523B pin masks
 */
#define WWS_AW9523B_P0_0 (1U << (0 + 8 * 0))
#define WWS_AW9523B_P0_1 (1U << (1 + 8 * 0))
#define WWS_AW9523B_P0_2 (1U << (2 + 8 * 0))
#define WWS_AW9523B_P0_3 (1U << (3 + 8 * 0))
#define WWS_AW9523B_P0_4 (1U << (4 + 8 * 0))
#define WWS_AW9523B_P0_5 (1U << (5 + 8 * 0))
#define WWS_AW9523B_P0_6 (1U << (6 + 8 * 0))
#define WWS_AW9523B_P0_7 (1U << (7 + 8 * 0))
#define WWS_AW9523B_P1_0 (1U << (0 + 8 * 1))
#define WWS_AW9523B_P1_1 (1U << (1 + 8 * 1))
#define WWS_AW9523B_P1_2 (1U << (2 + 8 * 1))
#define WWS_AW9523B_P1_3 (1U << (3 + 8 * 1))
#define WWS_AW9523B_P1_4 (1U << (4 + 8 * 1))
#define WWS_AW9523B_P1_5 (1U << (5 + 8 * 1))
#define WWS_AW9523B_P1_6 (1U << (6 + 8 * 1))
#define WWS_AW9523B_P1_7 (1U << (7 + 8 * 1))

/**
 * @brief config for pin mode
 */
extern wws_conf_t WWS_CONF_OUTPUT;
extern wws_conf_t WWS_CONF_INPUT;
extern wws_conf_t WWS_CONF_LED;

/**
 * @brief set pins
 * @param dev
 * @param mask mask of pins
 * @param conf
 * @return
 */
extern wws_ret_t wws_aw9523b_set_pins(wws_aw9523b_t *dev, unsigned int mask, wws_conf_t conf);

/**
 * @brief read logic from pin
 * @param dev
 * @param pin
 * @return
 */
extern wws_logic_t wws_aw9523b_read(wws_aw9523b_t *dev, unsigned int pin);

/**
 * @brief read dim from pin
 * @param dev
 * @param pin
 * @return
 */
extern unsigned char wws_aw9523b_read_dim(wws_aw9523b_t *dev, unsigned int pin);

/**
 * @brief write logic to pins
 * @param dev
 * @param pins
 * @param logic
 * @return
 */
extern wws_ret_t wws_aw9523b_write(wws_aw9523b_t *dev, unsigned int pins, wws_logic_t logic);

/**
 * @brief write dim to pins
 * @param dev
 * @param pins
 * @param dim
 * @return
 */
extern wws_ret_t wws_aw9523b_write_dim(wws_aw9523b_t *dev, unsigned int pins, unsigned char dim);


/**
 * @brief pin of aw9523b
 */
typedef struct __wws_aw9523b_pin_t
{
  wws_aw9523b_t *const dev;
  const unsigned int   pin;
} wws_aw9523b_pin_t;

/**
 * @brief read for pin
 * @param p
 * @return
 */
static inline wws_logic_t wws_aw9523b_pin_read(wws_aw9523b_pin_t *p)
{
  return wws_aw9523b_read(p->dev, p->pin);
}

/**
 * @brief read dim for pin
 * @param p
 * @return
 */
static inline unsigned char wws_aw9523b_pin_read_dim(wws_aw9523b_pin_t *p)
{
  return wws_aw9523b_read_dim(p->dev, p->pin);
}

/**
 * @brief write for pin
 * @param p
 * @param logic
 * @return
 */
static inline wws_ret_t wws_aw9523b_pin_write(wws_aw9523b_pin_t *p, wws_logic_t logic)
{
  return wws_aw9523b_write(p->dev, p->pin, logic);
}

/**
 * @brief write dim for pin
 * @param p
 * @param dim
 * @return
 */
static inline wws_ret_t wws_aw9523b_pin_write_dim(wws_aw9523b_pin_t *p, unsigned char dim)
{
  return wws_aw9523b_write_dim(p->dev, p->pin, dim);
}


/**
 * @brief logic reader for aw9523b pin
 * @param inst
 * @return
 */
extern wws_logic_t *wws_aw9523b_logic_reader(void *inst);

/**
 * @brief logic writer for aw9523b pin
 * @param inst
 * @param logic
 * @return
 */
extern void *wws_aw9523b_logic_writer(void *inst, wws_logic_t logic);

#endif /* ___WWS_AW9523B_H___ */
