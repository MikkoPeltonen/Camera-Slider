#include <stdint.h>
#include "enums.h"

/**
 * Turns uint32_t integers into char arrays.
 *
 * Usage: Initialize a unsigned char array and call with a pointer to that array
 * unsigned char bytes[4];
 * intToByteArray(bytes, number);
 *
 * @param bytes [description]
 * @param n     [description]
 */
void intToByteArray(unsigned char * bytes, uint32_t n);

/**
 * Turns four byte char array into a uint_32t integer. Char array must be in big
 * endian format (MSB first).
 *
 * @param  bytes [description]
 * @return       [description]
 */
uint32_t byteArrayToInt(unsigned char * bytes);

/**
 * [getMoveDirection description]
 * @param  enabled      [description]
 * @param  directionBit [description]
 * @return              [description]
 */
MoveDirection getMoveDirection(const int enabled, const int directionBit);
