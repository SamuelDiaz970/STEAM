#include <Wire.h>

char i2c_msg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(2); // set Arduino slave address
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Wire.available()) {
    i2c_msg = Wire.read();
    Serial.print(i2c_msg);
  }

  delay(100);
}
