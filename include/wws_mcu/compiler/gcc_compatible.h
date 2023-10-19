/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_GCC_COMPATIBLE_H___
#define ___WWS_GCC_COMPATIBLE_H___

/** macros */
#define WWS_ROM(_section) __attribute__((used, section(".text.wss." _section)))
#define WWS_RAM(_section) __attribute__((used, section(".data.wss." _section)))
#define WWS_UNUSED        __attribute__((unused))
#define WWS_USED          __attribute__((used))
#define WWS_ALIGNED(_n)   __attribute__((aligned(_n)))
#define WWS_WEAK          __attribute__((weak))
#define WWS_NORETURN      __attribute__((__noreturn__))
#define WWS_ALIAS(_alias) __attribute__((alias(#_alias)))
#define WWS_PACKED        __attribute__((__packed__))

#endif /* ___WWS_GCC_COMPATIBLE_H___ */
