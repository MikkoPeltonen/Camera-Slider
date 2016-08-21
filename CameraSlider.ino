#include <AccelStepper.h>

#define RIGHTBTN A1
#define LEFTBTN A0

// Slider motor
#define SLIDER_DIR 3
#define SLIDER_PUL 2
#define SLIDER_ENA 4

// Pitch motor
#define PITCH_DIR 1
#define PITCH_PUL 1
#define PITCH_ENA 1

// Yaw motor
#define YAW_DIR 2
#define YAW_PUL 2
#define YAW_ENA 2

// Focus motor
#define FOCUS_DIR 3
#define FOCUS_PUL 3
#define FOCUS_ENA 3

// Zoom motor
#define ZOOM_DIR 4
#define ZOOM_PUL 4
#define ZOOM_ENA 4


AccelStepper slider(AccelStepper::DRIVER, SLIDER_PUL, SLIDER_DIR); 

void setup() {  
  pinMode(RIGHTBTN, INPUT);
  pinMode(LEFTBTN, INPUT);

  // Specify enable pin for the motor
  slider.setEnablePin(SLIDER_ENA);
  slider.setPinsInverted(false, false, true);
  slider.enableOutputs();
  
  slider.setMaxSpeed(715);
  slider.setAcceleration(20000);  

  Serial.begin(9600);
}

void serialEvent() {
  while(Serial.available()) {
    char c = (char) Serial.read();
    int input = (int) c - 48;

    if(input == 1) {
      slider.move(-1000);
    } else if(input == 2) {
      slider.move(1000);
    } else if(input == 3) {
      slider.disableOutputs();
    } else if(input == 4) {
      slider.enableOutputs();
    }
  }
}

void loop() {
    if(digitalRead(RIGHTBTN) == HIGH && digitalRead(LEFTBTN) == HIGH) {
      // Do nothing
    } else if(digitalRead(RIGHTBTN) == HIGH) {
      slider.move(1000);
    } else if(digitalRead(LEFTBTN) == HIGH) {
      slider.move(-1000);
    }    

    slider.run();
}
