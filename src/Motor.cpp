#include <AccelStepper.h>
#include "Motor.h"
#include "enums.h"

Motor::Motor(const unsigned int pulPin,
             const unsigned int dirPin,
             Microstepping microsteppingMode,
             const unsigned int stepsPerRevolution,
             const double gearRatio) {
  this->stepper = AccelStepper(AccelStepper::DRIVER, pulPin, dirPin);

  this->stepper.setEnablePin(24);
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
}

int Motor::angleToSteps(const int angle) {
  return angle / 360.0 * stepsPerRevolution * static_cast<int>(microsteppingMode) * gearRatio;
}

void Motor::setMoveDirection(const MoveDirection direction) {
  this->moveDirection = direction;
}
