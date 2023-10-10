/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_SERVICE_H___
#define ___WWS_SERVICE_H___

#include "compiler.h"

extern const char *WWS_COMP_SERVICE;

extern const char *WWS_EVT_START;
extern const char *WWS_EVT_STOP;
extern const char *WWS_EVT_ROUTINE;
extern const char *WWS_EVT_TICK;

extern const char *WWS_ON_START   WWS_ALIAS(WWS_EVT_START);
extern const char *WWS_ON_STOP    WWS_ALIAS(WWS_EVT_STOP);
extern const char *WWS_ON_ROUTINE WWS_ALIAS(WWS_EVT_ROTINE);
extern const char *WWS_ON_TICK    WWS_ALIAS(WWS_EVT_TICK);

/**
 * @brief routine of service
 */
typedef struct __wws_serv_routine_t
{
  /**
   * @brief resume for co-routine
   */
  const void *resume;
  /**
   * @brief timestamp for routine
   */
  unsigned int timestamp;
} wws_serv_routine_t;

/**
 * @brief new routine for service
 */
#define wws_serv_routine                                                                           \
  .routine = &(wws_serv_routine_t)                                                                 \
  {                                                                                                \
    0                                                                                              \
  }

/**
 * @brief service
 */
typedef struct __wws_service_t
{
  /**
   * @brief callback
   */
  void (*const callback)(const char *phase, struct __wws_service_t *service);
  /**
   * @brief context of service
   */
  void *const context;
  /**
   * @brief payload of service
   */
  void *const payload;
  /**
   * @brief id of service for control
   */
  const unsigned char id;
  /**
   * @brief config bits
   */
  const unsigned char config;
  /**
   * @brief runtime flag
   */
  unsigned short flag;
  /**
   * @brief name of service
   */
  const char *name;
  /**
   * @brief routine runtime
   */
  wws_serv_routine_t *const routine, a;
} wws_service_t;

/**
 * @brief service
 */
extern wws_service_t **wws_services;

#endif /* ___WWS_SERVICE_H___ */