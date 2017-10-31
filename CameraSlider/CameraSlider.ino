#include <AccelStepper.h>
#include <HardwareSerial.h>
#include "commands.h"
#include "pinouts.h"
#include "enums.h"

// Used to determine the Serial connection which is used to send data to the client.
// Assigning the Serial port to a different pointer makes it easier to change it later.
HardwareSerial *Client = &Serial2;

ConnectionStatus connectionStatus = ConnectionStatus.DISCONNECTED;

// Initialize motors
AccelStepper panMotor(AccelStepper::DRIVER, PAN_PUL, PAN_DIR);
AccelStepper tiltMotor(AccelStepper::DRIVER, TILT_PUL, TILT_DIR);
AccelStepper slideMotor(AccelStepper::DRIVER, SLIDE_PUL, SLIDE_DIR);

/**
 * Send a message via Serial.
 */
void sendMessage(uint32_t * command, unsigned char * payload, uint32_t payloadLength) {
  uint32_t size = sizeof(char) * (payloadLength + 2);
  unsigned char msg[size];

  msg[0] = START;
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
 * Turns four byte char array into a uint_32t integer. Char array must be in big endian format (MSB first).
 */
uint32_t byteArrayToInt(unsigned char * bytes) {
    return bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
}

/**
 * Sends a handshake greeting message to the client
 */
void sendHandshakeGreetingMessage() {
  sendMessage(HANDSHAKE_GREETING, HANDSHAKE_GREETING, sizeof(HANDSHAKE_GREETING));
}

/**
 *
 */
void sendStatus() {
  char data[20];
  sendMessage(SEND_STATUS, data, sizeof(data));
}

/**
 * Send a position message (slide, pan, tilt, focus, zoom) via Serial.
 */
void sendPosition() {
  char data[20];

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
  
  sendMessage(SEND_POSITION, data, sizeof(data));
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
    Client->readBytesUntil(FLAG_STOP, msg, 30);

    // The second byte in the message is the command requested
    char cmd = msg[1];
    
    if (connectionStatus == ConnectionStatus.DISCONNECTED) {
      // Upon first connection, send a greeting message to verify the client
      sendHandshakeGreetingMessage();
      connectionStatus = ConnectionStatus.CONNECTING;
    } else if (connectionStatus == ConnectionStatus.CONNECTING) {
      // After the greeting message was sent, a special response is expected. If received
      // we proceed with the connection. Otherwise we refuse it.
      if (strcmp(HANDHSAKE_RESPONSE, msg) == 0) {
        connectionStatus = ConnectionStatus.CONNECTED;
      } else {
        connectionStatus = ConnectionStatus.DISCONNECTED;
      }
    } else {
      // Connection established, ready to receive commands.
      switch (cmd) {
        case SEND_STATUS:
          sendStatus();
          break;
        case SEND_POSITION:
          sendPosition();
          break;
      } 
    }
  }

  // TODO Stepping
}
