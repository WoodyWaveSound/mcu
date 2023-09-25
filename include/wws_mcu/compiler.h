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

#endif /* ___WWS_COMPILER_H___ */
