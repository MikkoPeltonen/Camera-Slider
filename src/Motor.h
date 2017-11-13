#include <AccelStepper.h>
#include "enums.h"

// Used to determine which microstepping mode the motor uses
enum class Microstepping {
  MODE1 = 1,
  MODE2 = 2,
  MODE4 = 4,
  MODE8 = 8,
  MODE16 = 16
};

class Motor {
  public:
    AccelStepper stepper;

  private:
    Microstepping microsteppingMode;
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
          Microstepping microsteppingMode,
          const unsigned int stepsPerRevolution,
          const double gearRatio);

    void setMicrosteppingMode(Microstepping microsteppingMode);

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
