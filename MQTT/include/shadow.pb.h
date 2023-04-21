/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8-dev */

#ifndef PB_MAIN_PROTOS_SHADOW_PB_H_INCLUDED
#define PB_MAIN_PROTOS_SHADOW_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _state {
    state_OK = 0,
    state_FAILED = 1,
    state_OVERCURRENT = 2
} state;

/* Struct definitions */
typedef struct _Shadow {
    bool active;
    state State;
} Shadow;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _state_MIN state_OK
#define _state_MAX state_OVERCURRENT
#define _state_ARRAYSIZE ((state)(state_OVERCURRENT+1))

#define Shadow_State_ENUMTYPE state


/* Initializer values for message structs */
#define Shadow_init_default                      {0, _state_MIN}
#define Shadow_init_zero                         {0, _state_MIN}

/* Field tags (for use in manual encoding/decoding) */
#define Shadow_active_tag                        1
#define Shadow_State_tag                         2

/* Struct field encoding specification for nanopb */
#define Shadow_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     active,            1) \
X(a, STATIC,   SINGULAR, UENUM,    State,             2)
#define Shadow_CALLBACK NULL
#define Shadow_DEFAULT NULL

extern const pb_msgdesc_t Shadow_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define Shadow_fields &Shadow_msg

/* Maximum encoded size of messages (where known) */
#define Shadow_size                              4

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif