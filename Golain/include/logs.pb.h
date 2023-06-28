/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.7-dev */

#ifndef PB_LOGS_PB_H_INCLUDED
#define PB_LOGS_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _Level {
    Level_NONE = 0,
    Level_ERROR = 1,
    Level_WARNING = 2,
    Level_INFO = 3,
    Level_DEBUG = 4,
    Level_VERBOSE = 5
} Level;

/* Struct definitions */
typedef struct _PLog {
    Level level;
    pb_callback_t tag;
    pb_callback_t function;
    pb_callback_t message;
    uint32_t time_ms;
} PLog;


/* Helper constants for enums */
#define _Level_MIN Level_NONE
#define _Level_MAX Level_VERBOSE
#define _Level_ARRAYSIZE ((Level)(Level_VERBOSE+1))

#define PLog_level_ENUMTYPE Level


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define PLog_init_default                        {_Level_MIN, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, 0}
#define PLog_init_zero                           {_Level_MIN, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, 0}

/* Field tags (for use in manual encoding/decoding) */
#define PLog_level_tag                           1
#define PLog_tag_tag                             2
#define PLog_function_tag                        3
#define PLog_message_tag                         4
#define PLog_time_ms_tag                         5

/* Struct field encoding specification for nanopb */
#define PLog_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    level,             1) \
X(a, CALLBACK, SINGULAR, STRING,   tag,               2) \
X(a, CALLBACK, SINGULAR, STRING,   function,          3) \
X(a, CALLBACK, SINGULAR, STRING,   message,           4) \
X(a, STATIC,   SINGULAR, UINT32,   time_ms,           5)
#define PLog_CALLBACK pb_default_field_callback
#define PLog_DEFAULT NULL

extern const pb_msgdesc_t PLog_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define PLog_fields &PLog_msg

/* Maximum encoded size of messages (where known) */
/* PLog_size depends on runtime parameters */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif