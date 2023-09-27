/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_MANIFEST_H___
#define ___WWS_MANIFEST_H___

/**
 * @brief Manifest definition
 */
typedef struct __wws_manifest_t
{
  /**
   * @brief name
   */
  const char *name;
  /**
   * @brief version: major
   */
  const unsigned char major;
  /**
   * @brief version: minor
   */
  const unsigned char minor;
  /**
   * @brief version: patch
   */
  const unsigned char patch;
  /**
   * @brief version: suffix for modification
   */
  const char *modification;
} wws_manifest_t;

/**
 * @brief library manifest
 */
extern const wws_manifest_t wws_manifest;


/**
 * @brief
 * @param manifest
 */
extern void wws_manifest_print(const wws_manifest_t *manifest);

#endif /* ___WWS_MANIFEST_H___ */
