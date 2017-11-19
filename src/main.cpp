#include <Arduino.h>
#include <AccelStepper.h>
#include <HardwareSerial.h>
#include "Helpers.h"
#include "Constants.h"
#include "pinouts.h"
#include "enums.h"
#include "Motor.h"
#include "Client.h"

// Boolean used to determine whether live action is ongoing, e.g. timelapse.
bool isRunning = false;

// Create a new Client object that is used to communicate with the client
Client client(Serial2);

// Connection status indicates whether the Camera Slider is connected to a
// client or is it
bool isConnected = false;

// Initialize motors
Motor slideMotor(SLIDE_PUL, SLIDE_DIR, SLIDE_ENA, Microstepping::MODE1, 200,
                 Constants::SLIDER_GEAR_RATIO);

Motor panMotor(PAN_PUL, PAN_DIR, PAN_ENA, Microstepping::MODE8, 200,
               Constants::NEMA11_GEAR_RATIO);

Motor tiltMotor(TILT_PUL, TILT_DIR, TILT_ENA, Microstepping::MODE8, 200,
                Constants::NEMA11_GEAR_RATIO);

// Time when the last motor move command was received (when holding down a
// button). Relative to start time, microseconds. Will overflow roughly every
// 70 minutes.
unsigned long motorMoveCommandReceived = 0;

// Time when a connection verification message was last received. If a
// verification message is not received at least every VERIFICATION_INTERVAL
// milliseconds, the connection is terminated.
unsigned long connectionVerificationTime = 0;

bool motorsMovingManually = false;

// Initial startup position and home are set to 0. Position is used to keep
// track of the current position regardles of home position. Position is always
// relative to the initial position and if home is set in the client the home
// position is set to the current position and only relative difference between
// Position and Home is shown in the client.
struct Coordinates Position = { 0, 0, 0, 0, 0 };
struct Coordinates Home = { 0, 0, 0, 0, 0 };

/*
 * Sets the current position as home position
 */
void setHome(void) {
  Home = Position;
}

/**
 * Callback for serial messages telling the motors to turn when a button is
 * held down in the client software. Used in motor manual control.
 *
 * @param data Data part of received serial buffer
 */
void moveMotors(const char b1, const char b2) {
  unsigned short moveInstructions = b1 << 8 | b2;
  motorMoveCommandReceived = micros();
  motorsMovingManually = true;

  panMotor.move(((b1 & (1 << 4)) ? 1 : 0) * ((b1 & (1 << 5)) ? 1 : -1));
  tiltMotor.move(((b1 & (1 << 2)) ? 1 : 0) * ((b1 & (1 << 3)) ? 1 : -1));
}

/**
 * Interrupt callback, called when either of the slider limit switches is
 * activated.
 */
void slideLimitSwitchActivated(void) {
  slideMotor.stepper->stop();
}

/**
 * Save received instructions to a file.
 *
 * @param data [description]
 */
void saveInstructions(const char * data) {
  // TODO Save data to EEPROM
}

/**
 * Start action. After action is started, only a stop command can be received
 * along with status commands.
 */
void startAction(void) {
  // TODO
}

void setup() {
  // Attach slider limit switches
  pinMode(SLIDE_LIMIT_SWITCH_1, INPUT);
  pinMode(SLIDE_LIMIT_SWITCH_2, INPUT);

  attachInterrupt(digitalPinToInterrupt(SLIDE_LIMIT_SWITCH_1),
                  slideLimitSwitchActivated, RISING);
  attachInterrupt(digitalPinToInterrupt(SLIDE_LIMIT_SWITCH_2),
                  slideLimitSwitchActivated, RISING);

  Serial.begin(9600);
  Serial2.begin(115200);
}

void loop() {
  if (client.serial.available()) {
    // Read 64 bytes into buffer
    char msg[64];
    client.serial.readBytesUntil(Constants::FLAG_STOP, msg, 64);

    // The second byte in the message is the command requested
    char cmd = msg[1];

    // Get only the data bytes
    char data[61];
    memcpy(&data, &msg + 2, sizeof(msg) - 3);

    if (!isConnected && memcmp(Constants::HANDSHAKE_RESPONSE, data,
                               strlen(Constants::HANDSHAKE_RESPONSE))) {
      client.sendHandshakeGreetingMessage();
      isConnected = true;
      connectionVerificationTime = millis();
    } else {
      // Connection established, ready to receive commands.
      switch (cmd) {
        case Commands::SEND_VERIFICATION:
          connectionVerificationTime = millis();
          break;
        case Commands::SEND_STATUS:
          client.sendStatus();
          break;
        case Commands::SEND_POSITION:
          client.sendPosition();
          break;
        case Commands::SET_HOME:
          setHome();
          break;
        case Commands::MOVE_MOTORS:
          moveMotors(msg[2], msg[3]);
          break;
        case Commands::SAVE_INSTRUCTIONS:
          saveInstructions(data);
          break;
        case Commands::START_ACTION:
          break;
      }
    }
  }

  // If new motor move command isn't received within the threshold time,
  // stop moving.
  /*if (motorsMovingManually && micros() - motorMoveCommandReceived >= Constants::HOLD_MOVE_DELAY) {
    const char zeros[2] = { 0, 0 };
    moveMotors(zeros);
    motorsMovingManually = false;
  }*/

  // See connectionVerificationTime docblock
  if (isConnected && millis() - connectionVerificationTime >= Constants::VERIFICATION_INTERVAL) {
    isConnected = false;
  }

  // Do stepping
  slideMotor.stepper->run();
  panMotor.stepper->run();
  tiltMotor.stepper->run();
}
