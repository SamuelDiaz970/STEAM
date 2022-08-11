#include <Wire.h>
#define Addr 0x34

// configuration bytes
#define AIN0_CONFIG 0b00000001
#define AIN1_CONFIG 0b00000011  // last bit signifies single-ended mode

#define ADC_REF     3.3 // supplying ADC 3.3V

#define R1          100000.0 // high side resistor in voltage sense circuit, 470k for 28V sense
#define R2          22000.0  // low side resistor in voltage sense circuit

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(300);

  Serial.println();
}

void loop() {
  uint16_t data[4];
  float v[4];
  float v_in[4];
  Serial.println("Beginning test");
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

    // convert to voltage
    v[i] = data[i]/4095.0*ADC_REF;

    // convert from ADC voltage to input (measured) voltage
    v_in[i] = v[i]/R2 * (R1+R2);
  }
  
  // Output data to serial monitor
  Serial.print("AIN0 voltage: ");
  Serial.println(v[0]);
  Serial.print("AIN1 voltage: ");
  Serial.println(v[1]);
  //Serial.print("AIN2 voltage: ");
  //Serial.println(v[2]);

  Serial.println();
  Serial.print("Input voltage 0: ");
  Serial.println(v_in[0]);
  Serial.print("Input voltage 1: ");
  Serial.println(v_in[1]);

  Serial.println();
  delay(3000);
}
