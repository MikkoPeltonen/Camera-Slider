#include <stdint.h>
#include "enums.h"

void intToByteArray(unsigned char * bytes, uint32_t n) {
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;
}

uint32_t byteArrayToInt(unsigned char * bytes) {
    return bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
}

MoveDirection getMoveDirection(const int enabled, const int directionBit) {
  if (!enabled) {
    return MoveDirection::None;
  }

  return directionBit ? MoveDirection::CW : MoveDirection::CCW;
}
