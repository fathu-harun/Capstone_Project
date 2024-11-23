#include <ArduinoBLE.h>

void setup() {
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  // Set up the BLE service and characteristics
  BLE.setLocalName("Nano33BLE");

  // Start advertising (which means to make it seen other Bluetooth devices)
  BLE.advertise();
  Serial.println("Bluetooth device is now advertising");
}

void loop() {
  // Listen for BLE connection
  Serial.println("Waiting to be paired by other Bluetooth devices...");
  // Serial.println(BLE.address());
  BLEDevice paired_device = BLE.central();
  delay(2000);
  if(paired_device) {
    Serial.println("Successfully connected via Bluetooth!");
    // Serial.println(paired_device.address());

    while (paired_device.connected()){
      Serial.println("I am connected!"); delay(1000);}
    Serial.println("Going to disconnect...");

    Serial.println("Successfully disconnected from paired device");
    // Serial.println(paired_device.address());
    delay(2000);
  }
}






