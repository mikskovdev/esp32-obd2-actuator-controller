#include <Arduino.h>
#include <RCSwitch.h>

RCSwitch mySwitch;

unsigned long lastReceiveTime = 0;
unsigned long lastValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  mySwitch.enableReceive(27);
}

void loop() {
  if (mySwitch.available()) {
    unsigned long now = micros();
    unsigned long value = mySwitch.getReceivedValue();

    Serial.print("Value: ");
    Serial.println(value);

    Serial.print("Pulse length (us): ");
    Serial.println(mySwitch.getReceivedDelay());

    // Check interval between identical transmissions
    if (value == lastValue && lastReceiveTime != 0) {
      Serial.print("Send interval (us): ");
      Serial.println(now - lastReceiveTime);
    }

    lastReceiveTime = now;
    lastValue = value;

    Serial.println("----------------------");

    if (value == 5054561) digitalWrite(2, HIGH);
    if (value == 5054562) digitalWrite(2, LOW);

    mySwitch.resetAvailable();
  }
}