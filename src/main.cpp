#include <Arduino.h>
#include <AccelStepper.h>
#include <HardwareSerial.h>
#include "Constants.h"
#include "pinouts.h"
#include "enums.h"

// Used to determine the Serial connection which is used to send data to the
// client. Assigning the Serial port to a different pointer makes it easier to
// change it later.
HardwareSerial * Client = &Serial2;

// Connection status indicates whether the Camera Slider is connected to a
// client, it is connecting or it is disconnected.
ConnectionStatus connectionStatus = ConnectionStatus::DISCONNECTED;

// Initialize motors
AccelStepper panMotor(AccelStepper::DRIVER, PAN_PUL, PAN_DIR);
AccelStepper tiltMotor(AccelStepper::DRIVER, TILT_PUL, TILT_DIR);
AccelStepper slideMotor(AccelStepper::DRIVER, SLIDE_PUL, SLIDE_DIR);

struct Coordinates {
  uint32_t slide;
  uint32_t pan;
  uint32_t tilt;
  uint32_t focus;
  uint32_t zoom;
};

// Initial startup position and home are set to 0. Position is used to keep
// track of the current position regardles of home position. Position is always
// relative to the initial position and if home is set in the client the home
// position is set to the current position and only relative difference between
// Position and Home is shown in the client.
struct Coordinates Position = { 0, 0, 0, 0, 0 };
struct Coordinates Home = { 0, 0, 0, 0, 0 };

/**
 * Send a message via Serial to the client.
 */
void sendMessage(const unsigned char command,
                 const unsigned char * payload,
                 const uint32_t payloadLength) {

  uint32_t size = sizeof(char) * (payloadLength + 2);
  unsigned char msg[size];

  msg[0] = Constants::FLAG_START;
  msg[1] = command;
  memcpy(msg + 2, payload, payloadLength);
  memcpy(msg + payloadLength + 1, "\0", 1);

  Client->write(msg, sizeof(msg));
}

/**
 * Turns uint32_t integers into char arrays.
 *
 * Usage: Initialize a unsigned char array and call with a pointer to that array
 * unsigned char bytes[4];
 * intToByteArray(bytes, number);
 */
void intToByteArray(unsigned char * bytes, uint32_t n) {
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;
}

/**
 * Turns four byte char array into a uint_32t integer. Char array must be in big
 * endian format (MSB first).
 */
uint32_t byteArrayToInt(unsigned char * bytes) {
    return bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
}

/**
 * Convert the given angle (in degrees) to motor steps.
 */
uint32_t angleToSteps(const uint16_t angle,
                      unsigned const uint16_t microsteps,
                      unsigned const uint16_t stepsPerRevolution) {
  return angle / 360.0 * stepsPerRevolution * microsteps;
}

/**
 * Sends a handshake greeting message to the client
 */
void sendHandshakeGreetingMessage(void) {
  unsigned char data[20];
  memcpy(data, Constants::HANDSHAKE_GREETING,
         strlen(Constants::HANDSHAKE_GREETING));
  sendMessage(Commands::SEND_HANDSHAKE_GREETING, data, sizeof(data));
}

/**
 *
 */
void sendStatus(void) {
  unsigned char data[20];
  sendMessage(Commands::SEND_STATUS, data, sizeof(data));
}


/*
 * Sets the current position as home position
 */
void setHome(void) {
  Home = Position;
}

/**
 * Send a position message (slide, pan, tilt, focus, zoom) via Serial.
 */
void sendPosition(void) {
  unsigned char data[20];

  // Convert position integers into char arrays
  unsigned char slide[4], pan[4], tilt[4], focus[4], zoom[4];
  intToByteArray(slide, 52300);
  intToByteArray(pan, 7250);
  intToByteArray(tilt, -2000);
  intToByteArray(focus, 0);
  intToByteArray(zoom, 5671);

  // Copy individual byte sets into data
  memcpy(data, slide, 4);
  memcpy(data + 4, pan, 4);
  memcpy(data + 8, tilt, 4);
  memcpy(data + 12, focus, 4);
  memcpy(data + 16, zoom, 4);

  sendMessage(Commands::SEND_POSITION, data, sizeof(data));
}

void setup() {
  Client->begin(115200);
  Serial.begin(9600);

  // Setup motors
  panMotor.setEnablePin(PAN_ENA);
  panMotor.setPinsInverted(false, false, true);
  panMotor.setMaxSpeed(2000);
  panMotor.setAcceleration(100000);
  panMotor.enableOutputs();
  panMotor.setCurrentPosition(0);
  panMotor.setSpeed(800);

  tiltMotor.setEnablePin(TILT_ENA);
  tiltMotor.setPinsInverted(false, false, true);
  tiltMotor.setMaxSpeed(2000);
  tiltMotor.setAcceleration(100000);
  tiltMotor.enableOutputs();
  tiltMotor.setCurrentPosition(0);
  tiltMotor.setSpeed(800);

  slideMotor.setEnablePin(SLIDE_ENA);
  slideMotor.setPinsInverted(false, false, true);
  slideMotor.setMaxSpeed(2000);
  slideMotor.setAcceleration(100000);
  slideMotor.enableOutputs();
  slideMotor.setCurrentPosition(0);
  slideMotor.setSpeed(800);

  Client->setTimeout(5);
}

void loop() {
  if (Client->available()) {
    // Read 30 bytes into buffer
    char msg[30];
    Client->readBytesUntil(Constants::FLAG_STOP, msg, 30);
    Serial.write(msg);

    // The second byte in the message is the command requested
    char cmd = msg[1];

    if (connectionStatus == ConnectionStatus::DISCONNECTED) {
      // Upon first connection, send a greeting message to verify the client
      sendHandshakeGreetingMessage();
      connectionStatus = ConnectionStatus::CONNECTING;
    } else if (connectionStatus == ConnectionStatus::CONNECTING) {
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
          sendStatus();
          break;
        case Commands::SEND_POSITION:
          sendPosition();
          break;
        case Commands::SET_HOME:
          setHome();
          break;
      }
    }
  }

  // TODO Stepping
}
