#include <cstdint>

namespace Commands {
  /**
   * Connection commands
   */

  static const unsigned char SEND_HANDSHAKE_GREETING =   10;
  static const unsigned char SEND_VERIFICATION =         11;


  /**
   * Status commands
   */
  static const unsigned char SEND_STATUS =               20;
  static const unsigned char SEND_POSITION =	           21;
  static const unsigned char HOMING_DONE =               22;


  /**
   * Action instructions
   */
  static const unsigned char SET_HOME =                  30;
  static const unsigned char GO_HOME =                   31;
  static const unsigned char MOVE_MOTORS =               32;
  static const unsigned char SAVE_INSTRUCTIONS =         33;
  static const unsigned char START_ACTION =              40;

}

namespace Constants {
  // FLAG_START and FLAG_STOP are used to indicate where a command starts and
  // stops. These bytes should not appear in the payload.
  static const unsigned char FLAG_START =                0b11110000;
  static const unsigned char FLAG_STOP =                 0b00001111;

  // When initiating the connecting, the client sends a SEND_HANDSHAKE_GREETING
  // message with CLIENT_GREETING payload. The Camera Slider responds with
  // the same command WELCOME_MESSAGE in the payload.
  static const char * WELCOME_MESSAGE =                  "r6lrj37e2nkaavgz";
  static const char * CLIENT_GREETING =                  "77fpm1lyw612jhlr";

  // Specifies the minimum interval at which a SEND_VERIFICTION message must be
  // received or otherwise the connection is terminated.
  static const unsigned int VERIFICATION_INTERVAL =      6000;

  // Specify the gear ratios for the motors.
  static double PAN_GEAR_RATIO =                         13 + 212 / 289;
  static double TILT_GEAR_RATIO =                        13 + 212 / 289;
  static double SLIDER_GEAR_RATIO =                      1;

  // When manually moving the motors from the client, each "step" occur every
  // HOLD_MOVE_DELAY when continuosly moving. Each step is HOLD_MOVE_ANGLE
  // degrees.
  static const unsigned int HOLD_MOVE_DELAY =            25;
  static const double HOLD_MOVE_ANGLE =                  0.1;
}
