#ifndef ___WWS_TYPEDEF_H___
#define ___WWS_TYPEDEF_H___

/**
 * @brief Logic = high
 */
#define WWS_HIGH (1)
/**
 * @brief Logic = low
 */
#define WWS_LOW (0)

/**
 * @brief compiled-time const string with length
 */
typedef struct __wws_cstr_t
{
  /**
   * @brief string
   */
  const char *const str;
  /**
   * @brief length
   */
  const unsigned int len;
} wws_cstr_t;

/**
 * @brief Generate wws_cstr_t
 * @param _str string
 */
#define wws_new_cstr(_str) wws_new_cstr_with_len(_str, sizeof(_str) - 1)

/**
 * @brief Generate wws_cstr_t
 * @param _str
 * @param _len
 */
#define wws_new_cstr_with_len(_str, _len)                                      \
  (wws_cstr_t)                                                                 \
  {                                                                            \
    .str = (_str), .len = (_len)                                               \
  }


/**
 * @brief generate local variable by line number
 * @param ... additional name
 */
#define WWS_LOCAL_VAR(...) WWS_LINE_VAR(___wws_local, ##__VA_ARGS__)

/**
 * @brief generate variable by line number
 * @param _var prefix of line var
 * @param ... additional name
 */
#define WWS_LINE_VAR(_var, ...) ___WWS_LINE_VAR_1(_var, __LINE__, ##__VA_ARGS__)

#define ___WWS_LINE_VAR_1(_var, _line, ...)                                    \
  ___WWS_LINE_VAR_2(_var, _line, ##__VA_ARGS__)
#define ___WWS_LINE_VAR_2(_var, _line, ...) _var##_L##_line##_##__VA_ARGS__


#endif /* ___WWS_TYPEDEF_H___ */
