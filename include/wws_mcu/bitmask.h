/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#ifndef ___WWS_BITMASK_H___
#define ___WWS_BITMASK_H___

/**
 * @brief generate bitmask by bit
 */
#define wws_bitmask(_bit) (1U << _bit)

/**
 * @brief Generate bitmask with first and last bit
 * @param _first first bit position
 * @param _last last bit position
 */
#define wws_new_bitmask(_first, _last)                                                             \
  ((0xFFFFFFFFU << (_first)) & (0xFFFFFFFFU >> (31U - (_last))))

/**
 * @brief Generate bitmask from 0 to len
 * @param _len length of bitmask, must be less or eq than 32
 */
#define wws_new_bitmask_with_len(_len) wws_new_bitmask(0, (_len - 1))

/**
 * @brief Compare bitmask is full equal as compare under masked
 * @param _mask masked to compare
 * @param _compare to compare
 */
#define wws_bitmask_eq(_bitmask, _mask, _compare) (((_bitmask) & (_mask)) == (_compare))

/**
 * @brief Compare bitmask is every bits in compare are set
 * @param _bitmask bitmask
 * @param _compare mask to compare
 */
#define wws_bitmask_every(_bitmask, _compare) wws_bitmask_eq(_bitmask, _compare, _compare)

/**
 * @brief Compare bitmask is any bit in compare is set
 * @param _bitmask bitmask
 * @param _compare mask to compare
 */
#define wws_bitmask_any(_bitmask, _compare) (((_bitmask) & (_compare)) > 0)

/**
 * @brief Compare bitmask is none of bits in compare are set
 * @param _bitmask bitmask
 * @param _compare mask to compare
 */
#define wws_bitmask_none(_bitmask, _compare) (!wws_bitmask_any(_bitmask, _compare))

/**
 * @brief mask a bitmask
 * @param _bitmask bitmask
 * @param _mask to be masked
 */
#define wws_bitmask_mask(_bitmask, _mask)                                                          \
  do {                                                                                             \
    (_bitmask) |= (_mask);                                                                         \
  } while (0)

/**
 * @brief unmask a bitmask
 * @param _bitmask bitmask
 * @param _mask to be unmasked
 */
#define wws_bitmask_unmask(_bitmask, _mask)                                                        \
  do {                                                                                             \
    (_bitmask) &= ~(_mask);                                                                        \
  } while (0)

/**
 * @brief Compare bitmask is bit set
 * @param _bitmask bitmask
 * @param _bit to compare
 */
#define wws_bitmask_isset(_bitmask, _bit) (wws_bitmask_any(_bitmask, (1U << _bit)))

/**
 * @brief set a bit on bitmask
 * @param _bitmask bitmask
 * @param _bit to be set
 */
#define wws_bitmask_set(_bitmask, _bit) wws_bitmask_mask(_bitmask, (1U << _bit))

/**
 * @brief reset a bit on bitmask
 * @param _bitmask bitmask
 * @param _bit to be reset
 */
#define wws_bitmask_reset(_bitmask, _bit) wws_bitmask_unmask(_bitmask, (1U << _bit))


#endif /* ___WWS_BITMASK_H___ */
