/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_COMPILER_H___
#define ___WWS_COMPILER_H___

#if /** armcc */ defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
#include "compiler/armcc.h"
#elif /** armclang */ defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#include "compiler/armclang.h"
#elif /** clang */ defined(__clang__)
#include "compiler/clang.h"
#elif /** gcc */ defined(__GNUC__)
#include "compiler/gcc.h"
#else /** non-support compiler */
#error Compiled with unsupported compiler
#endif /** compiler */


#ifndef WWS_RAM
#error feature RAM section is necessary
#define WWS_RAM(...)
#endif /** WWS_RAM */

#ifndef WWS_WEAK
#error feature weak is necessary
#define WWS_WEAK
#endif /** WWS_WEAK */

#ifndef WWS_PACKED
#warning no packed supported
#define WWS_PACKED
#endif /** WWS_PACKED */

#ifndef WWS_ALIAS
#error WWS_ALIAS is necessary
#define WWS_ALIAS(...)
#endif /** WWS_ALIAS */

#endif /* ___WWS_COMPILER_H___ */
