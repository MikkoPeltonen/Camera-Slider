#include <AccelStepper.h>
#include <HardwareSerial.h>
#include "commands.h"
#include "pinouts.h"
#include "enums.h"

#define STEPS 200
#define MICROSTEPPING 1
#define RATIO 13.73

// Used to determine the Serial connection which is used to send data to the client.
// Assigning the Serial port to a different pointer makes it easier to change it later.
HardwareSerial *Client = &Serial2;

ConnectionStatus connectionStatus = ConnectionStatus.DISCONNECTED;

int slide = 150;
int pan = 7200;
int tilt = -5000;
int focus = 0;
int zoom = 5200;

// Initialize motors
AccelStepper panMotor(AccelStepper::DRIVER, PAN_PUL, PAN_DIR);
AccelStepper tiltMotor(AccelStepper::DRIVER, TILT_PUL, TILT_DIR);
AccelStepper slideMotor(AccelStepper::DRIVER, SLIDE_PUL, SLIDE_DIR);

/* 
 * Convert a given angle to steps required for the motor to rotate the angle
 */
int angleToSteps(double angle) {
  return (int) (angle / 360.0 * STEPS * MICROSTEPPING * RATIO);
}

/**
 * Send data to Serial
 */
void sendData(byte data[]) {
  Client->write(data, sizeof(data));
}

/**
 * Turns integers into byte arrays
 */
char intToByteArray(uint32_t n) {
  unsigned char bytes[4];
  bytes[0] = (n >> 24) & 0xFF;
  bytes[1] = (n >> 16) & 0xFF;
  bytes[2] = (n >> 8) & 0xFF;
  bytes[3] = n & 0xFF;

  return bytes;
}

/**
 * Sends a handshake greeting message to the client
 */
void sendHandshakeGreetingMessage() {
  byte msg[18];
  msg[0] = FLAG_START;
  msg[1] = HANDSHAKE_GREETING;
  memcpy(&msg, HANDSHAKE_GREETING, sizeof(HANDSHAKE_GREETING));
  msg[18] = FLAG_STOP;

  sendData(msg);
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
  byte msg[27];
  msg[0] = FLAG_START;
  msg[1] = CMD_POSITION;



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

