#include <AccelStepper.h>
#include "enums.h"

class Motor {
  public:
    AccelStepper stepper;

  private:
    int microsteps;
    int stepsPerRevolution;
    double gearRatio;
    MoveDirection moveDirection;

  public:
    /**
     * Motor class constructor.
     *
     * @param pulPin             Stepper driver PUL pin
     * @param dirPin             Stepper driver DIR pin
     * @param microsteps         Number of microsteps
     * @param stepsPerRevolution Full steps per revolution
     * @param gearRatio          Motor gearbox ratio
     */
    Motor(const unsigned int pulPin,
          const unsigned int dirPin,
          const unsigned int microsteps,
          const unsigned int stepsPerRevolution,
          const double gearRatio);

    /**
     * Convert a given angle to steps.
     *
     * @param  angle Angle in degrees
     * @return       Steps to move
     */
    int angleToSteps(const int angle);

    /**
     * Set the direction the motor is moving.
     *
     * @param direction CW, CCW, None
     */
    void setMoveDirection(const MoveDirection direction);
};
