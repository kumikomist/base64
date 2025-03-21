#ifndef BASE64_H
#define BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// include
//------------------------------------------------------------------------------
#include <stdint.h>

//------------------------------------------------------------------------------
// define
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function
//------------------------------------------------------------------------------

int32_t base64_encode(const uint8_t bin[], uint32_t bin_size, uint8_t ascii[]);
int32_t base64_decode(const uint8_t ascii[], uint32_t ascii_size, uint8_t bin[]);

#ifdef __cplusplus
}
#endif

#endif
