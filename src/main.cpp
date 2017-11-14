#include <Arduino.h>
#include <AccelStepper.h>
#include <HardwareSerial.h>
#include "Helpers.h"
#include "Constants.h"
#include "pinouts.h"
#include "enums.h"
#include "Motor.h"
#include "Client.h"

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

// Create a new Client object that is used to communicate with the client
Client * client = new Client(&Serial2);

// Connection status indicates whether the Camera Slider is connected to a
// client, it is connecting or it is disconnected.
ConnectionStatus connectionStatus = ConnectionStatus::DISCONNECTED;

// Initialize motors
Motor * slideMotor = new Motor(SLIDE_PUL, SLIDE_DIR, SLIDE_ENA,
                               Microstepping::MODE1, 200,
                               Constants::SLIDER_GEAR_RATIO);

Motor * panMotor =   new Motor(PAN_PUL, PAN_DIR, PAN_ENA,
                               Microstepping::MODE16, 200,
                               Constants::NEMA11_GEAR_RATIO);

Motor * tiltMotor =  new Motor(TILT_PUL, TILT_DIR, TILT_ENA,
                               Microstepping::MODE16, 200,
                               Constants::NEMA11_GEAR_RATIO);

// Time when the last motor move command was received (when holding down a
// button). Relative to start time, microseconds. Will overflow roughly every
// 70 minutes.
unsigned long motorMoveCommandReceived = 0;

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
void moveMotors(const char * data) {
  unsigned short moveInstructions = data[0] << 8 | data[1];
  motorMoveCommandReceived = micros();

  slideMotor->move(
    (moveInstructions & MotorMoveBitmask::SLIDE_ENABLE) *
    (moveInstructions & MotorMoveBitmask::SLIDE_DIRECTION ? 1 : -1) *
    Constants::HOLD_MOVE_ANGLE
  );

  panMotor->move(
    (moveInstructions & MotorMoveBitmask::PAN_ENABLE) *
    (moveInstructions & MotorMoveBitmask::PAN_DIRECTION ? 1 : -1) *
    Constants::HOLD_MOVE_ANGLE
  );

  tiltMotor->move(
    (moveInstructions & MotorMoveBitmask::TILT_ENABLE) *
    (moveInstructions & MotorMoveBitmask::TILT_DIRECTION ? 1 : -1) *
    Constants::HOLD_MOVE_ANGLE
  );
}

/**
 * Interrupt callback, called when either of the slider limit switches is
 * activated.
 */
void slideLimitSwitchActivated(void) {
  slideMotor->stepper.stop();
}

void setup() {
  // Attach slider limit switches
  pinMode(SLIDE_LIMIT_SWITCH_1, INPUT);
  pinMode(SLIDE_LIMIT_SWITCH_2, INPUT);

  attachInterrupt(digitalPinToInterrupt(SLIDE_LIMIT_SWITCH_1),
                  slideLimitSwitchActivated, RISING);
  attachInterrupt(digitalPinToInterrupt(SLIDE_LIMIT_SWITCH_2),
                  slideLimitSwitchActivated, RISING);
}

void loop() {
  if (client->serial->available()) {
    // Read 30 bytes into buffer
    char msg[30];
    client->serial->readBytesUntil(Constants::FLAG_STOP, msg, 30);

    // The second byte in the message is the command requested
    char cmd = msg[1];

    // Get only the data bytes
    char data[27];
    memcpy(&data, &msg + 2, sizeof(msg) - 3);

    if (unlikely(connectionStatus == ConnectionStatus::DISCONNECTED)) {
      // Upon first connection, send a greeting message to verify the client
      client->sendHandshakeGreetingMessage();
      connectionStatus = ConnectionStatus::CONNECTING;
    } else if (unlikely(connectionStatus == ConnectionStatus::CONNECTING)) {
      // After the greeting message was sent, a special response is expected.
      // If received we proceed with the connection. Otherwise we refuse it.
      if (strcmp(Constants::HANDSHAKE_RESPONSE, msg) == 0) {
        connectionStatus = ConnectionStatus::CONNECTED;
      } else {
        connectionStatus = ConnectionStatus::DISCONNECTED;
      }
    } else {
      // Connection established, ready to receive commands.
      switch (cmd) {
        case Commands::SEND_STATUS:
          client->sendStatus();
          break;
        case Commands::SEND_POSITION:
          client->sendPosition();
          break;
        case Commands::SET_HOME:
          setHome();
          break;
        case Commands::MOVE_MOTORS:
          moveMotors(data);
          break;
      }
    }
  }

  // If new motor move command isn't received within the threshold time,
  // stop moving.
  if (micros() - motorMoveCommandReceived >= Constants::HOLD_MOVE_DELAY) {
    const char zeros[2] = { 0, 0 };
    moveMotors(zeros);
  }

  // Do stepping
  slideMotor->stepper.run();
  panMotor->stepper.run();
  tiltMotor->stepper.run();
}
