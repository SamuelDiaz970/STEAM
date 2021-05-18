#include <Wire.h>
#include <math.h>
#define Addr 0x34

// configuration bytes
#define AIN0_CONFIG 0b00000001
#define AIN1_CONFIG 0b00000011  // last bit signifies single-ended mode

#define B 3978 // constant [K]

// thermistor constants (unused)
#define A1 0.003354016
#define B1 0.0002569850
#define C1 0.000002620131
#define D1 6.383091/100000000

#define R0 10000.0 // thermistor reference value
#define R1 10000.0 // 10k resistor in series with thermistor

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

    // 5V pin on Arduino outputs = 5.05 V
    v[i] = data[i]/4095.0*5.05;

    r_therm[i] = R1*v[i]/(5.05-v[i]); // calculate resistance of thermistor
    //temp[i] = 1/(A1 + B1*log(r_therm[i]/R_REF) + C1*pow(log(r_therm[i]/R_REF), 2)
    //              + D1*pow(log(r_therm[i]/R_REF), 3));
    //temp[i] = B/(log(r_therm[i]/(R0*exp(-B/298.15))));
    temp[i] = 1/(log(r_therm[i]/R0)/B+1/298.15);

    temp_c[i] = temp[i] - 273.15;       // convert to celsius
    temp_f[i] = 9.0/5.0*temp_c[i] + 32; // convert to fahrenheit
  }
  
  // Output data to serial monitor
  Serial.print("AIN0 voltage : ");
  Serial.println(v[0]);
  Serial.print("AIN0 temp (F): ");
  Serial.println(temp_f[0]);
  
  Serial.print("AIN1 voltage : ");
  Serial.println(v[1]);
  Serial.print("AIN1 temp (F): ");
  Serial.println(temp_f[1]);
  
  //Serial.print("AIN2 voltage: ");
  //Serial.println(v[2]);

  Serial.println();
  delay(300);
}
