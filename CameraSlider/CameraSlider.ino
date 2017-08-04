#include <AccelStepper.h>

#define ENA 51
#define PUL 47
#define DIR 49

#define STEPS 200
#define MICROSTEPPING 1
#define RATIO 13.73

typedef enum {
  Timelapse,
  Video,
  Panorama
} Mode;

typedef enum {
  CW,
  CCW,
  STOP,
  STEP
} Direction;


Direction dir = STOP;

bool isRunning = false;
bool isConnected = false;

int slide = 150;
int pan = 7200;
int tilt = -5000;
int focus = 0;
int zoom = 5200;

AccelStepper stepper(AccelStepper::DRIVER, PUL, DIR);

/* 
 * Convert a given angle to steps required for the motor to rotate the angle
 */
int angleToSteps(double angle) {
  return (int) (angle / 360.0 * STEPS * MICROSTEPPING * RATIO);
}

void setup() {
  Serial2.begin(115200);
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);

  stepper.setEnablePin(ENA);
  stepper.setPinsInverted(false, false, true);
  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(100000);
  stepper.enableOutputs();

  stepper.setCurrentPosition(0);
  //stepper.moveTo(43969);
  //stepper.moveTo(angleToSteps(90.0));
  stepper.setSpeed(800);

  pinMode(13, OUTPUT);
  Serial2.setTimeout(5);
}

int l = angleToSteps(3);
int c = angleToSteps(3) * -1;

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop() {
  if (Serial2.available()) {
    String msg = Serial2.readStringUntil("\n");
    msg.trim();

    if (msg == String("Hello, Camera Slider!")) {
      Serial2.println("Hello, Android!");
    } else if (msg == String("STATUS?")) {
      Serial2.println("STATUS:RUNNING;MODE:TIMELAPSE;ELAPSED:5230;REMAINING:6250");
    } else if (msg == String("HOME?")) {
      //Serial2.println("HOME:GET;SLIDE:" + slide + ";PAN:" + pan + ";TILT:" + tilt + ";FOCUS:" + focus + ";ZOOM:" + zoom);
      Serial2.println("HOME:GET;SLIDE:1500;PAN:7200;TILT:-5000;FOCUS:0;ZOOM:5200");
    } else if (msg.startsWith("MOTOR")) {
      String m = getValue(msg, ';', 0);
      String d = getValue(msg, ';', 1);
      String m_a = getValue(m, ':', 0);
      String m_b = getValue(m, ':', 1);
      String d_a = getValue(d, ':', 0);
      String d_b = getValue(d, ':', 1);

      if (d_b == "STOP") {
        Serial.println("STOP");
        dir = STOP;
      } else if (d_b == "CW") {
        Serial.println("CW");
        stepper.setSpeed(500);
        dir = CW;
      } else if (d_b == "CCW") {
        Serial.println("CCW");
        stepper.setSpeed(-500);
        dir = CCW;
      }
    } else if (msg.startsWith("STEP")) {
      String m = getValue(msg, ';', 0);
      String d = getValue(msg, ';', 1);
      String m_a = getValue(m, ':', 0);
      String m_b = getValue(m, ':', 1);
      String d_a = getValue(d, ':', 0);
      String d_b = getValue(d, ':', 1);

       if (d_b == "CW") {
        stepper.move(l);
        dir = STEP;
      } else if (d_b == "CCW") {
        stepper.move(c);
        dir = STEP;
      }
    }
  } 

  //if (Serial.available()) {
  //  Serial2.write(Serial.read());
  //}

  //stepper.run();
  if (dir == CW || dir == CCW) {
    stepper.runSpeed();
  } else if (dir == STEP) {
    stepper.run();

    if (stepper.distanceToGo() == 0) {
      dir = STOP;
    }
  }
}

void goHome() {}

void setHome() {}
