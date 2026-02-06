#include <Arduino.h>
#include "transmitter.h"
#include "oled_screen.h"
#include "obd_scanner.h"

#define rpm_open_value 2800.0
#define rpm_close_value 2500.0
#define throttle_open_value 80.0
#define throttle_close_value 60.0

bool OBD_mode = true;

unsigned long lastDisplayUpdate = 0;
const unsigned long displayInterval = 100;  // ms (10 Hz)

void controlValve() {
  if (valveBusy) return;  // don't issue new commands mid-move

  if (!valveState) { // Valve is closed
    if (rpm2 > rpm_open_value || throttle2 > throttle_open_value) {
      OpenValve();
    }
  } 
  else if (valveState) { // Valve is open
    if (rpm2 < rpm_close_value && throttle2 < throttle_close_value) {
      CloseValve();
    }
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print(F("Running"));
  
  display.setTextSize(1);

  display.setCursor(0,16);
  display.print(F("Valve pos: "));
  if (valveBusy) {
    display.println(F("Moving"));
  }
  else if (valveState) {
    display.println(F("Open"));
  }
  else if (!valveState) {
    display.println(F("Closed"));
  }

  display.setCursor(0,26);
  display.println(F("---------------------"));

  display.setCursor(0,36);
  display.print(F("RPM:      "));
  display.print(rpm2, 0);
  display.println(F(" rpm"));

  display.setCursor(0,46);
  display.print(F("Throttle: "));
  display.print(throttle2, 0);
  display.println(F(" %"));
  
  display.display();
}

void setup() {
  Serial.begin(115200);

  initTransmitter();
  initOLED();
  //I2C_scan();
  CloseValve(); // Ensure valve is closed at startup

  if (OBD_mode) {
    initOBDScanner();
  }
}

void loop() {
  if (OBD_mode) {
    scanOBD();
  }
  controlValve();
  handleValve();

  unsigned long now = millis();

  if (now - lastDisplayUpdate >= displayInterval) {
    lastDisplayUpdate = now;
    updateDisplay();
  }

  // // demo RPM logic (already non-blocking)
  // static unsigned long lastRPMUpdate = 0;
  // static int rpmStep = 100;   // +100 going up, -100 going down

  // if (now - lastRPMUpdate >= 100) {
  //   lastRPMUpdate = now;

  //   rpm += rpmStep;

  //   if (rpm >= 6000) {
  //    rpm = 6000;
  //     rpmStep = -100;   // start decreasing
  //  }
  //  else if (rpm <= 0) {
  //    rpm = 0;
  //    rpmStep = 100;    // start increasing again
  //  }
  // }
}
