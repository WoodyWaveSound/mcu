#ifndef ___WWS_GCC_COMPATIBLE_H___
#define ___WWS_GCC_COMPATIBLE_H___

/** macros */
#define WSS_ROM(_section) __attribute__((used, section(".text.wss." _section)))
#define WSS_RAM(_section) __attribute__((used, section(".data.wss." _section)))
#define WSS_UNUSED __attribute__((unused))
#define WSS_USED __attribute__((used))
#define WSS_ALIGNED(_n) __attribute__((aligned(_n)))
#define WSS_WEAK __attribute__((weak))
#define WSS_NORETURN __attribute__((__noreturn__))
#define WSS_ALIAS(_src, _alias) extern typeof(_src) _alias __attribute__((alias(#_src)))

#endif /* ___WWS_GCC_COMPATIBLE_H___ */
