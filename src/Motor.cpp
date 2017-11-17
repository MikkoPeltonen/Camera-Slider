#include <AccelStepper.h>
#include "Motor.h"
#include "enums.h"

unsigned int microstepTruthTable[5][3] = {
  { 0, 0, 0 },
  { 1, 0, 0 },
  { 0, 1, 0 },
  { 1, 1, 0 },
  { 1, 1, 1 }
};

Motor::Motor(const unsigned int pulPin,
             const unsigned int dirPin,
             const unsigned int enablePin,
             Microstepping microsteppingMode,
             const unsigned int stepsPerRevolution,
             const double gearRatio) {
  this->stepper = AccelStepper(AccelStepper::DRIVER, pulPin, dirPin);

  this->stepper.setEnablePin(enablePin);
  this->stepper.setPinsInverted(false, false, true);
  this->stepper.setMaxSpeed(2000);
  this->stepper.setAcceleration(10000);
  this->stepper.enableOutputs();
  this->stepper.setCurrentPosition(0);
  this->stepper.setSpeed(800);

  this->microsteppingMode = microsteppingMode;
  this->stepsPerRevolution = stepsPerRevolution;
  this->gearRatio = gearRatio;
}

void Motor::setMicrosteppingMode(Microstepping microsteppingMode) {
  this->microsteppingMode = microsteppingMode;

  unsigned int pinTable[] = { 0, 0, 0 };
  switch (this->microsteppingMode) {
    case Microstepping::MODE1:
      memcpy(&pinTable, &microstepTruthTable[0], sizeof(pinTable));
      break;
    case Microstepping::MODE2:
      memcpy(&pinTable, &microstepTruthTable[1], sizeof(pinTable));
      break;
    case Microstepping::MODE4:
      memcpy(&pinTable, &microstepTruthTable[2], sizeof(pinTable));
      break;
    case Microstepping::MODE8:
      memcpy(&pinTable, &microstepTruthTable[3], sizeof(pinTable));
      break;
    case Microstepping::MODE16:
      memcpy(&pinTable, &microstepTruthTable[4], sizeof(pinTable));
      break;
  }

  digitalWrite(ms1, pinTable[0]);
  digitalWrite(ms2, pinTable[1]);
  digitalWrite(ms3, pinTable[2]);
}

int Motor::angleToSteps(const int angle) {
  return angle / 360.0 * stepsPerRevolution * static_cast<int>(microsteppingMode) * gearRatio;
}

void Motor::move(const double angle) {
  this->stepper.move(angleToSteps(angle));
}
