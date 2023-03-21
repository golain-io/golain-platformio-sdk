#ifndef DECODING_CALLBACKS_H
#define DECODING_CALLBACKS_H

#include "../nanopb/pb.h"
#include "string.h"
#include"../nanopb/pb_common.h"
#include "../nanopb/pb_decode.h"
#include "../nanopb/pb_encode.h"
bool decode_string(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    uint8_t buffer[1024] = {0};
    
    /* We could read block-by-block to avoid the large buffer... */
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;
    
    sprintf((char*)*arg, "%s", buffer);
    return true;
}

#endif