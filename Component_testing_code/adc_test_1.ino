#include <Wire.h>
#define Addr 0x34

// configuration bytes
#define AIN0_CONFIG 0b00000001
#define AIN1_CONFIG 0b00000011  // last bit signifies single-ended mode

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(300);

  Serial.println();
}

void loop() {
  uint16_t data[4];
  float v[4];
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Channel Selection
  Wire.write(AIN1_CONFIG);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 3 bytes of data
  Wire.requestFrom(Addr, 4);
  
  // Read 3 bytes of data
  while (Wire.available() < 4);

  for(int i=0; i<2; i++) {
    // read first byte
    data[i] = Wire.read() << 8;
    // read second byte
    data[i] |= Wire.read();
  
    data[i] &= 0xfff;

    // 5V pin on Arduino outputs 5.06
    v[i] = data[i]/4095.0*5.06;
  }
  
  // Output data to serial monitor
  Serial.print("AIN0 voltage: ");
  Serial.println(v[0]);
  Serial.print("AIN1 voltage: ");
  Serial.println(v[1]);
  //Serial.print("AIN2 voltage: ");
  //Serial.println(v[2]);

  Serial.println();
  delay(300);
}
