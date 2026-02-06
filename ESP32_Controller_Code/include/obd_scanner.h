#ifndef OBD_SCANNER_H
#define OBD_SCANNER_H

#include "BluetoothSerial.h"
#include "ELMduino.h"

BluetoothSerial SerialBT;
#define ELM_PORT SerialBT
#define DEBUG_PORT Serial

ELM327 myELM327;

typedef enum { ENG_RPM , THROTTLE,} obd_pid_states;
obd_pid_states obd_state = ENG_RPM;

float rpm = 0;
float throttle = 0;

float rpm2 = 0;
float throttle2 = 0;

void initOBDScanner() {
    // SerialBT.setPin("1234");
    ELM_PORT.begin("ArduHUD", true);

    DEBUG_PORT.println("Connecting to OBD scanner...");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(F("Setting up"));
    display.setTextSize(1);
    display.setCursor(0,16);
    display.print(F("Connecting to OBD..."));

    display.display();

    if (!ELM_PORT.connect("OBDII"))
    {
        DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.print(F("OBD Error"));
        display.setTextSize(1);
        display.setCursor(0,16);
        display.print(F("Coudn't connect to"));
        display.setCursor(0,26);
        display.print(F("OBD - Phase 1"));
        display.display();
        while (1)
            ;
    }

    if (!myELM327.begin(ELM_PORT, true, 2000))
    {
        DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 2");
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.print(F("OBD Error"));
        display.setTextSize(1);
        display.setCursor(0,16);
        display.print(F("Coudn't connect to OBD"));
        display.setCursor(0,26);
        display.print(F("OBD - Phase 2"));
        display.display();
        while (1)
            ;
    }

    DEBUG_PORT.println("Connected to ELM327");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(F("OBD Ready"));
    display.display();

    delay(2000);
}

void scanOBD() {
  switch (obd_state)
  {
    case ENG_RPM:
    {
      rpm = myELM327.rpm();
      if (myELM327.nb_rx_state == ELM_SUCCESS)
      {
        DEBUG_PORT.print("rpm: ");
        DEBUG_PORT.println(rpm);
        rpm2 = rpm;
        obd_state = THROTTLE;
      }
      else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
      {
        myELM327.printError();
        obd_state = THROTTLE;
      }
      
      break;
    }


    case THROTTLE:
    {

      throttle = myELM327.throttle();

      if (myELM327.nb_rx_state == ELM_SUCCESS)
      {
        DEBUG_PORT.print("throttle position: ");
        DEBUG_PORT.println(throttle);
        throttle2 = throttle;
        obd_state = ENG_RPM;
      }
      else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
      {
        myELM327.printError();
        obd_state = ENG_RPM;
      }
      
      break;
    }
  }
}

#endif