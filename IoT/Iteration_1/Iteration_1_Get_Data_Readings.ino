#include <Arduino_BMI270_BMM150.h>

float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

void setup() {
  // put your setup code here, to run once:
  IMU.begin();
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (IMU.accelerationAvailable() 
      && IMU.gyroscopeAvailable() 
      && IMU.magneticFieldAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);
    IMU.readMagneticField(mx, my, mz);

    Serial.print(ax); Serial.print(",\t");
    Serial.print(ay); Serial.print(",\t");
    Serial.print(az); Serial.print(",\t");
    Serial.print(gx); Serial.print(",\t");
    Serial.print(gy); Serial.print(",\t");
    Serial.print(gz); Serial.print(",\t");
    Serial.print(mx); Serial.print(",\t");
    Serial.print(my); Serial.print(",\t");
    Serial.println(mz);
    
    delay(500);      //delay 1s between each reading
  }
}









