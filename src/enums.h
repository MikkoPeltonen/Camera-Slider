#include <stdint.h>

#ifndef ENUMS
#define ENUMS

struct LineData {
  uint32_t startTime;
  uint32_t duration;
  uint32_t slide;
  uint32_t pan;
  uint32_t tilt;
  uint32_t focus;
  uint32_t zoom;
};

struct Coordinates {
  int slide;
  int pan;
  int tilt;
  int focus;
  int zoom;
};

// Not to pollute global namespace with enum values, enum class would be ideal to
// use. However, enum classes don't support bitwise operators (such as
// char & MyEnumClass:value). Wrapping a plain enum in a struct (or a namespace)
// circumvents this problem.
struct MotorMoveBitmask {
  enum value {
    SLIDE_DIRECTION = 1 << 16,
    SLIDE_ENABLE = 1 << 15,
    PAN_DIRECTION = 1 << 14,
    PAN_ENABLE = 1 << 13,
    TILT_DIRECTION = 1 << 12,
    TILT_ENABLE = 1 << 11,
    FOCUS_DIRECTION = 1 << 8,
    FOCUS_ENABLE = 1 << 7,
    ZOOM_DIRECTION = 1 << 6,
    ZOOM_ENABLE = 1 << 5
  };
};

#endif
