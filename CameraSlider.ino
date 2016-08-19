#include <AccelStepper.h>

#define RIGHTBTN A1
#define LEFTBTN A0

#define DIR_PIN 3
#define PUL_PIN 2
#define ENA_PIN 4

AccelStepper stepper(AccelStepper::DRIVER, PUL_PIN, DIR_PIN); 

void setup() {  
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  digitalWrite(ENA_PIN, LOW);

  pinMode(RIGHTBTN, INPUT);
  pinMode(LEFTBTN, INPUT);

  stepper.setMaxSpeed(800);
  stepper.setAcceleration(1000);
}

void loop() {
    //stepper.run();
    if(digitalRead(RIGHTBTN) == HIGH && digitalRead(LEFTBTN) == HIGH) {
      // Do nothing
    } else if(digitalRead(RIGHTBTN) == HIGH) {
      stepper.move(100);
    } else if(digitalRead(LEFTBTN) == HIGH) {
      stepper.move(-100);
    }

    stepper.run();
}
