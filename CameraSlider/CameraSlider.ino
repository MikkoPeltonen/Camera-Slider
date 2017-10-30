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
 * Turns integers into byte arrays.
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
 * Sends a handshake greeting message to the client
 */
void sendHandshakeGreetingMessage() {
  sendMessage(HANDSHAKE_GREETING, HANDSHAKE_GREETING, sizeof(HANDSHAKE_GREETING));
}

/**
 *
 */
void sendStatus() {

}

/**
 *
 */
void sendPosition() {
  
}

void setup() {
  Client->begin(115200);
  Serial.begin(9600);

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
    byte msg[30];
    Client->readBytesUntil(FLAG_STOP, msg, 30);
    byte cmd = msg[1];
    
    if (connectionStatus == ConnectionStatus.DISCONNECTED) {
      sendHandshakeGreetingMessage();
      connectionStatus = ConnectionStatus.CONNECTING;
    } else if (connectionStatus == ConnectionStatus.CONNECTING) {

      if (strcmp(HANDHSAKE_RESPONSE, msg) == 0) {
        connectionStatus = ConnectionStatus.CONNECTED;
      } else {
        connectionStatus = ConnectionStatus.DISCONNECTED;
      }
    } else {
      switch (cmd) {
        case CMD_STATUS:
          break;
      } 
    }
  }
}

