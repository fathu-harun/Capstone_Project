#include <ArduinoBLE.h>
#include <Arduino_BMI270_BMM150.h>

// Declaring accelerometer
BLEService accService("12345678-1234-5678-1234-56789abcdef0");  
BLEFloatCharacteristic accXChar("2a5b0001-1234-5678-1234-56789abcdef0", BLERead | BLENotify);
BLEFloatCharacteristic accYChar("2a5b0002-1234-5678-1234-56789abcdef0", BLERead | BLENotify);
BLEFloatCharacteristic accZChar("2a5b0003-1234-5678-1234-56789abcdef0", BLERead | BLENotify);

// Declaring gyroscope
BLEService gyroService("5A1D0001-AB45-4A7D-84EE-2B4894A4E33B"); 
BLEFloatCharacteristic gyroXChar("2a5b0004-1234-5678-1234-56789abcdef0", BLERead | BLENotify);
BLEFloatCharacteristic gyroYChar("2a5b0005-1234-5678-1234-56789abcdef0", BLERead | BLENotify);
BLEFloatCharacteristic gyroZChar("2a5b0006-1234-5678-1234-56789abcdef0", BLERead | BLENotify);

// Declaring magnetometer 
BLEService magService("6B2D0002-BC56-4B8D-94FF-3C5985B5F44C");  
BLEFloatCharacteristic magXChar("2a5b0007-1234-5678-1234-56789abcdef0", BLERead | BLENotify);
BLEFloatCharacteristic magYChar("2a5b0008-1234-5678-1234-56789abcdef0", BLERead | BLENotify);
BLEFloatCharacteristic magZChar("2a5b0009-1234-5678-1234-56789abcdef0", BLERead | BLENotify);

// Declaring gesture to tell Arduino to send data
BLEService gestureService("19b10000-e8f2-537e-4f6c-d104768a1214"); 
BLEByteCharacteristic gestureCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);

enum {
  GESTURE_NONE  = -1,
  GESTURE_UP    = 0,
  GESTURE_DOWN  = 1,
  GESTURE_LEFT  = 2,
  GESTURE_RIGHT = 3,
  GESTURE_MIDDLE = 4
};

int gesture = -1;

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  if (!IMU.begin()) {
  Serial.println("IMU initialization failed");
  while (1);
}

  // Set up the BLE service and characteristics
  BLE.setLocalName("Fathu_Proj_06_Flexosense");

  // Advertise comms
  BLE.setAdvertisedService(gestureService);
  gestureService.addCharacteristic(gestureCharacteristic);
  BLE.addService(gestureService);
  gestureCharacteristic.writeValue(-1);

  // Advertise the acc service
  BLE.setAdvertisedService(accService);
  accService.addCharacteristic(accXChar);
  accService.addCharacteristic(accYChar);
  accService.addCharacteristic(accZChar);
  BLE.addService(accService);

  // Advertise the acc service
  BLE.setAdvertisedService(gyroService);
  gyroService.addCharacteristic(gyroXChar);
  gyroService.addCharacteristic(gyroYChar);
  gyroService.addCharacteristic(gyroZChar);
  BLE.addService(gyroService);
  
  // Advertise the acc service
  BLE.setAdvertisedService(magService);
  magService.addCharacteristic(magXChar);
  magService.addCharacteristic(magYChar);
  magService.addCharacteristic(magZChar);
  BLE.addService(magService);

  // Start advertising (which means to make it seen other Bluetooth devices)
  BLE.advertise();
  Serial.println("Bluetooth device is now advertising");
}

void loop() {
  Serial.print("This Arduino ");
  String address = BLE.address();
  Serial.print("has the address: ");
  Serial.println(address);
  delay(1000);

  BLEDevice central = BLE.central();

  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float accXCharVal, accYCharVal, accZCharVal;        
  float gyroXCharVal, gyroYCharVal, gyroZCharVal;        
  float magXCharVal, magYCharVal, magZCharVal;

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) {
      if (gesture == 4) {
        writeGesture(gesture);
      }
      if (gestureCharacteristic.written()) {
        gesture = gestureCharacteristic.value();
        if (gesture == 4) {Serial.println("* Actual value: MIDDLE");}
        writeGesture(gesture);
      }
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }

  // delay(1000); //wait for 1 second before repeating loop
}
void writeGesture(int gesture) {

   switch (gesture) {
      case GESTURE_UP:
        Serial.println("* Actual value: UP (red LED on)");
        Serial.println(" ");
        digitalWrite(LEDR, LOW);
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, HIGH);
        digitalWrite(LED_BUILTIN, LOW);
        break;
      case GESTURE_DOWN:
        Serial.println("* Actual value: DOWN (blue LED on)");
        Serial.println(" ");
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDB, HIGH);
        digitalWrite(LED_BUILTIN, LOW);
        break;
      case GESTURE_LEFT:
        Serial.println("* Actual value: LEFT (green LED on)");
        Serial.println(" ");
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        break;
      case GESTURE_RIGHT:
        Serial.println("* Actual value: RIGHT (built-in LED on)");
        Serial.println(" ");
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        break;
      case GESTURE_MIDDLE:
        if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {

          float ax, ay, az;
          float gx, gy, gz;
          float mx, my, mz;
          float accXCharVal, accYCharVal, accZCharVal;        
          float gyroXCharVal, gyroYCharVal, gyroZCharVal;        
          float magXCharVal, magYCharVal, magZCharVal;

          IMU.readAcceleration(ax, ay, az);
          IMU.readGyroscope(gx, gy, gz);
          IMU.readMagneticField(mx, my, mz);

          accXChar.writeValue(ax);
          accXCharVal = accXChar.value();
          accYChar.writeValue(ay);
          accYCharVal = accYChar.value();
          accZChar.writeValue(az);
          accZCharVal = accZChar.value();

          gyroXChar.writeValue(gx);
          gyroXCharVal = gyroXChar.value();
          gyroYChar.writeValue(gy);
          gyroYCharVal = gyroYChar.value();
          gyroZChar.writeValue(gz);
          gyroZCharVal = gyroZChar.value();

          magXChar.writeValue(mx);
          magXCharVal = magXChar.value();
          magYChar.writeValue(my);
          magYCharVal = magYChar.value();
          magZChar.writeValue(mz);
          magZCharVal = magZChar.value();

          Serial.print(accXCharVal); Serial.print(", ");
          Serial.print(accYCharVal); Serial.print(", ");
          Serial.print(accZCharVal); Serial.print(", ");
          Serial.print(gyroXCharVal); Serial.print(", ");
          Serial.print(gyroYCharVal); Serial.print(", ");
          Serial.print(gyroZCharVal); Serial.print(", ");
          Serial.print(magXCharVal); Serial.print(", ");
          Serial.print(magYCharVal); Serial.print(", ");
          Serial.println(magZCharVal);
        } 
        // if (!IMU.accelerationAvailable()){
        //   Serial.print("Acc is not working. ");
        // }
        // if (!IMU.gyroscopeAvailable()){
        //   Serial.print("Gyro is not working. ");
        // }
        // if (!IMU.magneticFieldAvailable()){
        //   Serial.println("Mag is not working");
        // }
        
        // delay(500);      //delay 500ms between each reading
        break;
      default:
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, HIGH);
        digitalWrite(LED_BUILTIN, LOW);
        break;
      }      
}