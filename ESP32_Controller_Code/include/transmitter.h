#ifndef transmitter_h
#define transmitter_h

#include <RCSwitch.h>

#define RF_TX_PIN 25

bool valveBusy = false;
bool valveTargetState = false; // true = open, false = closed
unsigned long valveLastSend = 0;
int valveSendCount = 0;

const int valveRepeats = 4;
const unsigned long valveInterval = 60;

bool valveState = 0; // 0 = closed, 1 = open
RCSwitch mySwitch;

void initTransmitter() {
    pinMode(RF_TX_PIN, OUTPUT);
    mySwitch.enableTransmit(RF_TX_PIN);
    mySwitch.setProtocol(1);
    mySwitch.setPulseLength(350);  // use value printed by sniffer if available

    Serial.println("RF Transmitter Initialized");
}

void OpenValve() {
  if (valveBusy) return;

  Serial.println("Opening Valve");
  valveTargetState = true;
  valveBusy = true;
  valveSendCount = 0;
}

void CloseValve() {
  if (valveBusy) return;

  Serial.println("Closing Valve");
  valveTargetState = false;
  valveBusy = true;
  valveSendCount = 0;
}

void handleValve() {
  if (!valveBusy) return;

  unsigned long now = millis();

  if (now - valveLastSend >= valveInterval) {
    valveLastSend = now;

    if (valveTargetState) {
      mySwitch.send(5054561, 24);
    } else {
      mySwitch.send(5054562, 24);
    }

    valveSendCount++;

    if (valveSendCount >= valveRepeats) {
      valveBusy = false;
      valveSendCount = 0;
      valveState = valveTargetState;
    }
  }
}



#endif