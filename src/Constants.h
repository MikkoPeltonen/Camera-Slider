#include <cstdint>

namespace Commands {
  static const unsigned char SEND_HANDSHAKE_GREETING =   0x08;
  static const unsigned char SEND_STATUS =               0x02;
  static const unsigned char SEND_POSITION =	           0x03;
  static const unsigned char SET_HOME =                  0x04;
}

namespace Constants {
   static const unsigned char FLAG_START =               0x01;
   static const unsigned char FLAG_STOP =                0x00;
   static const char * HANDSHAKE_GREETING =              "r6lrj37e2nkaavgz";
   static const char * HANDSHAKE_RESPONSE =              "77fpm1lyw612jhlr";
}
