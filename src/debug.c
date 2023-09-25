#include <stdio.h>
#include <wws_mcu/debug.h>

wws_debug_callback_t ___wws_debug_callback = NULL;

const char *WWS_COMP_LOG      = "LOG";
const char *WWS_EVT_LOG_TRACE = "TRACE";
const char *WWS_EVT_LOG_DEBUG = "DEBUG";
const char *WWS_EVT_LOG_INFO  = "INFO";
const char *WWS_EVT_LOG_WARN  = "WARN";
const char *WWS_EVT_LOG_ERROR = "ERROR";
const char *WWS_EVT_LOG_FATAL = "FATAL";
const char *WWS_COMP_ASSERT   = "ASSERT";
