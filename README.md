# Common MCU Framework

- Fully abstract for logic code.
- Implement low-level code per platform, separated from logic.
- Designed **without** thread.


# Designed for

- ARM-based MCU

# Compiler

- GCC
- Clang
- ArmCC
- ArmClang

# Term

- `WWS_NEW_`: new struct or value for right-side of assign
- `WWS_CREATE_`: create variable without assign
- `WWS_DEFINE_`: define a specific type or struct
- `wws_platform_`: API to be runned in platform
