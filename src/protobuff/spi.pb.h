/* Automatically generated nanopb header */
/* Generated by nanopb-1.0.0-dev */

#ifndef PB_SPI_PB_H_INCLUDED
#define PB_SPI_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _BitOrder {
    BitOrder_LSB = 0,
    BitOrder_MSB = 1
} BitOrder;

typedef enum _Mode {
    Mode_MODE_0 = 0,
    Mode_MODE_1 = 1,
    Mode_MODE_2 = 2,
    Mode_MODE_3 = 3
} Mode;

typedef enum _SPIError {
    SPIError_SUCCESS = 0,
    SPIError_INCORRECT_INPUT = 1,
    SPIError_FRAME_FORMAT = 2,
    SPIError_UNKNOWN = 3
} SPIError;

/* Struct definitions */
typedef PB_BYTES_ARRAY_T(4096) SPIReadWriteRequest_write_t;
typedef struct _SPIReadWriteRequest {
    SPIReadWriteRequest_write_t write;
    uint32_t rn_bytes;
    uint32_t rw_byte;
    uint32_t speed;
    BitOrder bit_order;
    Mode mode;
    bool no_answer;
} SPIReadWriteRequest;

typedef PB_BYTES_ARRAY_T(4096) SPIReadWriteResponse_read_t;
typedef struct _SPIReadWriteResponse {
    SPIError error;
    SPIReadWriteResponse_read_t read;
} SPIReadWriteResponse;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _BitOrder_MIN BitOrder_LSB
#define _BitOrder_MAX BitOrder_MSB
#define _BitOrder_ARRAYSIZE ((BitOrder)(BitOrder_MSB+1))

#define _Mode_MIN Mode_MODE_0
#define _Mode_MAX Mode_MODE_3
#define _Mode_ARRAYSIZE ((Mode)(Mode_MODE_3+1))

#define _SPIError_MIN SPIError_SUCCESS
#define _SPIError_MAX SPIError_UNKNOWN
#define _SPIError_ARRAYSIZE ((SPIError)(SPIError_UNKNOWN+1))

#define SPIReadWriteRequest_bit_order_ENUMTYPE BitOrder
#define SPIReadWriteRequest_mode_ENUMTYPE Mode

#define SPIReadWriteResponse_error_ENUMTYPE SPIError


/* Initializer values for message structs */
#define SPIReadWriteRequest_init_default         {{0, {0}}, 0, 0, 0, _BitOrder_MIN, _Mode_MIN, 0}
#define SPIReadWriteResponse_init_default        {_SPIError_MIN, {0, {0}}}
#define SPIReadWriteRequest_init_zero            {{0, {0}}, 0, 0, 0, _BitOrder_MIN, _Mode_MIN, 0}
#define SPIReadWriteResponse_init_zero           {_SPIError_MIN, {0, {0}}}

/* Field tags (for use in manual encoding/decoding) */
#define SPIReadWriteRequest_write_tag            1
#define SPIReadWriteRequest_rn_bytes_tag         2
#define SPIReadWriteRequest_rw_byte_tag          3
#define SPIReadWriteRequest_speed_tag            4
#define SPIReadWriteRequest_bit_order_tag        5
#define SPIReadWriteRequest_mode_tag             6
#define SPIReadWriteRequest_no_answer_tag        7
#define SPIReadWriteResponse_error_tag           1
#define SPIReadWriteResponse_read_tag            2

/* Struct field encoding specification for nanopb */
#define SPIReadWriteRequest_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BYTES,    write,             1) \
X(a, STATIC,   SINGULAR, UINT32,   rn_bytes,          2) \
X(a, STATIC,   SINGULAR, UINT32,   rw_byte,           3) \
X(a, STATIC,   SINGULAR, UINT32,   speed,             4) \
X(a, STATIC,   SINGULAR, UENUM,    bit_order,         5) \
X(a, STATIC,   SINGULAR, UENUM,    mode,              6) \
X(a, STATIC,   SINGULAR, BOOL,     no_answer,         7)
#define SPIReadWriteRequest_CALLBACK NULL
#define SPIReadWriteRequest_DEFAULT NULL

#define SPIReadWriteResponse_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    error,             1) \
X(a, STATIC,   SINGULAR, BYTES,    read,              2)
#define SPIReadWriteResponse_CALLBACK NULL
#define SPIReadWriteResponse_DEFAULT NULL

extern const pb_msgdesc_t SPIReadWriteRequest_msg;
extern const pb_msgdesc_t SPIReadWriteResponse_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define SPIReadWriteRequest_fields &SPIReadWriteRequest_msg
#define SPIReadWriteResponse_fields &SPIReadWriteResponse_msg

/* Maximum encoded size of messages (where known) */
#define SPIReadWriteRequest_size                 4123
#define SPIReadWriteResponse_size                4101
#define SPI_PB_H_MAX_SIZE                        SPIReadWriteRequest_size

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
