#include <AccelStepper.h>
#include "enums.h"

// Thruth table for microstepping modes
// MS1   MS2   MS3   Microsteps
//  0     0     0    1
//  1     0     0    2
//  0     1     0    4
//  1     1     0    8
//  1     1     1    16

unsigned int microstepTruthTable[5][3] = {
  { 0, 0, 0 },
  { 1, 0, 0 },
  { 0, 1, 0 },
  { 1, 1, 0 },
  { 1, 1, 1 }
};

// Used to determine which microstepping mode the motor uses.
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

    // Microstepping mode selection pins for Big Easy Driver
    int ms1;
    int ms2;
    int ms3;

  public:
    /**
     * Motor class constructor.
     *
     * @param pulPin             Stepper driver PUL pin
     * @param dirPin             Stepper driver DIR pin
     * @param enablePin          Stepper driver enable pin
     * @param microsteps         Number of microsteps
     * @param stepsPerRevolution Full steps per revolution
     * @param gearRatio          Motor gearbox ratio
     */
    Motor(const unsigned int pulPin,
          const unsigned int dirPin,
          const unsigned int enablePin,
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
     * Moves the stepper by the given angle.
     *
     * @param angle Angle in degrees
     */
    void move(const double angle);
};
