// Arduino code for Maxim MAX11613 and OP462
// Last edited: 5/25/2021 MD AV

#include <Wire.h>
#define Addr 0x34 // I2C address for ADC

#define AIN0_CONFIG 0b00000001
#define AIN1_CONFIG 0b00000011
#define AIN2_CONFIG 0b00000101
#define AIN3_CONFIG 0b00000111

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  delay(300);

  Serial.println("Beginning test");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t  data[1];
  float current[1];
  Wire.beginTransmission(Addr);

  Wire.write(AIN0_CONFIG);

  Wire.endTransmission();

  Wire.requestFrom(Addr, 2);

  while (Wire.available() < 2);

 for(int i=0; i<1; i++) {
    // read first byte MSB
    data[i] = Wire.read() << 8;
    // read second byte LSB
    data[i] |= Wire.read();
  
    data[i] &= 0xfff; // We only want the first 12 bits 

  current[i] = data[i]/4095.00*3.3;
 }
  Serial.print("Current : ");
  Serial.println(current[0]);
  delay(1000);
}
