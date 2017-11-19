#include <cstdint>

namespace Commands {
  static const unsigned char SEND_HANDSHAKE_GREETING =   0x08;
  static const unsigned char SEND_STATUS =               0x02;
  static const unsigned char SEND_POSITION =	           0x03;
  static const unsigned char SET_HOME =                  0x04;
  static const unsigned char MOVE_MOTORS =               0x05;
  static const unsigned char SAVE_INSTRUCTIONS =         0x06;
  static const unsigned char START_ACTION =              0x07;
  static const unsigned char SEND_VERIFICATION =         0x09;
}

namespace Constants {
   static const unsigned char FLAG_START =               0x01;
   static const unsigned char FLAG_STOP =                0x00;
   static const char * HANDSHAKE_GREETING =              "r6lrj37e2nkaavgz";
   static const char * HANDSHAKE_RESPONSE =              "77fpm1lyw612jhlr";
   static const unsigned int HOLD_MOVE_DELAY =           25;
   static const double HOLD_MOVE_ANGLE =                 0.1;
   static double NEMA11_GEAR_RATIO =                     13 + 212 / 289;
   static double SLIDER_GEAR_RATIO =                     1;
   static const unsigned int EEPROM_ADDRESS =            0x40;
   static const unsigned int VERIFICATION_INTERVAL =     6000;
}
