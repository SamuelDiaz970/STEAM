#include <Wire.h>
#include <math.h>
#define Addr 0x34

// configuration bytes--last bit signifies single-ended mode
#define AIN0_CONFIG 0b00000001
#define AIN1_CONFIG 0b00000011
#define AIN2_CONFIG 0b00000101
#define AIN3_CONFIG 0b00000111

#define B 3943 // constant [K]

// thermistor constants (unused)
#define A1 0.003354016
#define B1 0.0002569850
#define C1 0.000002620131
#define D1 6.383091/100000000

#define R0 30000.0 // thermistor reference value
#define R1 22000.0 // 22k resistor in series with thermistor (at 5V)
#define R2 15000.0 // 15k resistor in series with thermistor (at GND)

#define ADC_REF 3.3 // is this 2.048

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(300);

  Serial.println("Beginning test");
}

void loop() {
  uint16_t data[4];
  float v[4];
  double r_therm[4], temp[4], temp_c[4], temp_f[4];
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Channel Selection (AIN3)
  Wire.write(AIN3_CONFIG);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 8 bytes of data
  Wire.requestFrom(Addr, 8);
  
  // Read 8 bytes of data
  while (Wire.available() < 8);

  int r_tot; // use to find total resistance, then find thermal resistance

  for(int i=0; i<2; i++) {
    // read first byte
    data[i] = Wire.read() << 8;
    // read second byte
    data[i] |= Wire.read();
  
    data[i] &= 0xfff;

    v[i] = data[i]/4095.0*ADC_REF;

    // calculate resistance of thermistor
    r_tot = 5 / (v[i]/R1);
    r_therm[i] = r_tot - R1 - R2;

    temp[i] = 1/(log(r_therm[i]/R0)/B+1/298.15);

    temp_c[i] = temp[i] - 273.15;       // convert to celsius
//    temp_f[i] = 9.0/5.0*temp_c[i] + 32; // convert to fahrenheit
  }
  
  // Output data to serial monitor
  Serial.print("AIN0 voltage : ");
  Serial.println(v[0]);
  Serial.print("AIN0 temp (C): ");
  Serial.println(temp_c[0]);
  
  Serial.print("AIN1 voltage : ");
  Serial.println(v[1]);
  Serial.print("AIN1 temp (C): ");
  Serial.println(temp_c[1]);
  
//  Serial.print("AIN2 voltage : ");
//  Serial.println(v[2]);
//  Serial.print("AIN2 temp (F): ");
//  Serial.println(temp_f[2]);
//
//  Serial.print("AIN3 voltage : ");
//  Serial.println(v[3]);
//  Serial.print("AIN3 temp (F): ");
//  Serial.println(temp_f[3]);

  Serial.println();
  delay(10000); // wait 10 sec before next reading
}
