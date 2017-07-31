#include <AccelStepper.h>

#define ENA 51
#define PUL 47
#define DIR 49

enum Mode {
  Timelapse,
  Video,
  Panorama
};

bool isRunning = false;
bool isConnected = false;

int slide = 150;
int pan = 7200;
int tilt = -5000;
int focus = 0;
int zoom = 5200;

AccelStepper stepper(AccelStepper::DRIVER, PUL, DIR);

void setup() {
  Serial2.begin(115200);
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);

  stepper.setEnablePin(ENA);
  stepper.setPinsInverted(false, false, true);
  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(10000);
  stepper.enableOutputs();

  stepper.setCurrentPosition(0);
  stepper.moveTo(43969);
  stepper.setSpeed(500);
}

void loop() {
  //stepper.move(1000);
  
  if (Serial2.available()) {
    String msg = Serial2.readString();
    msg.trim();
    Serial.print(msg);

    if (msg == String("Hello, Camera Slider!")) {
      Serial2.println("Hello, Android!");
    } else if (msg == String("STATUS?")) {
      Serial2.println("STATUS:RUNNING;MODE:TIMELAPSE;ELAPSED:5230;REMAINING:6250");
    } else if (msg == String("HOME?")) {
      //Serial2.println("HOME:GET;SLIDE:" + slide + ";PAN:" + pan + ";TILT:" + tilt + ";FOCUS:" + focus + ";ZOOM:" + zoom);
      Serial2.println("HOME:GET;SLIDE:1500;PAN:7200;TILT:-5000;FOCUS:0;ZOOM:5200");
    }
  }

  if (Serial.available()) {
    Serial2.write(Serial.read());
  }

  stepper.run();
}

void goHome() {}

void setHome() {}
