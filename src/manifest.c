/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/manifest.h>
#include <wws_mcu/debug.h>

const wws_manifest_t wws_manifest = {
  .name         = "WWS_MCU",
  .major        = 1,
  .minor        = 0,
  .patch        = 0,
  .modification = "alpha",
};

void wws_manifest_print(const wws_manifest_t *manifest)
{
  wws_logln(0,
            "[MANIFEST] %s : %d.%d.%d%c%s @ " __DATE__ " " __TIME__,
            manifest->name,
            manifest->major,
            manifest->minor,
            manifest->patch,
            manifest->modification ? '-' : 0,
            manifest->modification);
}
