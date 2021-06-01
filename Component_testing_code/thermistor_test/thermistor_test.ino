// Arduino code for Maxim MAX11613 12 Bit ADC thermistor implementation
// Last edited: 5/25/2021 MD

#include <Wire.h>
#include <math.h>
#define Addr 0x34 // I2C address for ADC

// configuration bytes
// [7] Register bit. 1 = setup byte 0 = configuration byte
// [6:5] Scan select bits. Two bits select the scanning configuration. See table 5 of the user manual
// [4:1] Channel select bits. Choose what channel to go up/down to while scanning. Default is 0000 at power up. AIN0
// [1] Mode select bit. 1 = single-ended 0 = differential
#define AIN0_CONFIG 0b00000001
#define AIN1_CONFIG 0b00000011
#define AIN2_CONFIG 0b00000101
#define AIN3_CONFIG 0b00000111
#define B 3978 // constant [K] - defined within data sheet 
#define thermistor_count 4


//// thermistor constants (unused)
//#define A1 0.003354016
//#define B1 0.0002569850
//#define C1 0.000002620131
//#define D1 6.383091/100000000

//#define R0 10000 // thermistor reference value
#define R1 10000 // 10k resistor in series with thermistor
#define R2 15000 // 15k resistor in series with thermistor

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(300);

  Serial.println("Beginning test");
}

void loop() {
  int  n = thermistor_count * 2; // = number of bytes to be read, 2 bytes per ADC read
  uint16_t data[thermistor_count];
  float v[thermistor_count];
  float r_therm[thermistor_count], temp[thermistor_count], temp_c[thermistor_count], temp_f[thermistor_count];
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Channel Selection
  // Determine configuration based on thermistor count
  switch(thermistor_count){
    case 1:
      Wire.write(AIN0_CONFIG);
      break;
    case 2:
      Wire.write(AIN1_CONFIG);
      break;
    case 3:
      Wire.write(AIN2_CONFIG);
      break;
    case 4:
      Wire.write(AIN3_CONFIG);
      break;
     default:
       Serial.println("No proper thermistor count specified");
       break;
  }
//  Wire.write(AIN1_CONFIG);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 3 bytes of data
  Wire.requestFrom(Addr, n);
  
  // Read 3 bytes of data
  while (Wire.available() < n);

  for(int i=0; i<thermistor_count; i++) {
    // read first byte MSB
    data[i] = Wire.read() << 8;
    // read second byte LSB
    data[i] |= Wire.read();
  
    data[i] &= 0xfff; // We only want the first 12 bits 

    // 5V pin on Arduino outputs = 5.05 V
    v[i] = data[i]/4095.00*5.05;
    if( i == 0){
    r_therm[i] = R1*v[i]/(5.05-v[i]); // calculate resistance of thermistor - voltage divider equation
    }
    else{ r_therm[i] = R2*v[i]/(5.05-v[i]); }
    //temp[i] = 1/(A1 + B1*log(r_therm[i]/R_REF) + C1*pow(log(r_therm[i]/R_REF), 2)
    //              + D1*pow(log(r_therm[i]/R_REF), 3));
    //temp[i] = B/(log(r_therm[i]/(R0*exp(-B/298.15))));
    temp[i] = 1/(log(r_therm[i]/R1)/B+1/298.15);

    temp_c[i] = temp[i] - 273.15;       // convert to celsius
    temp_f[i] = 9.0/5.0*temp_c[i] + 32; // convert to fahrenheit
  // Output data to serial monitor
  Serial.print("AIN");
  Serial.print(i);
  Serial.print(" Voltage :         ");
  Serial.println(v[i]);
  Serial.print("AIN");
  Serial.print(i);
  Serial.print(" Temperature (F) : ");
  Serial.println(temp_f[i]);
  }
//  Serial.print("AIN0 voltage : ");
//  Serial.println(v[0]);
//  Serial.print("AIN0 temp (F): ");
//  Serial.println(temp_f[0]);
//  
//  Serial.print("AIN1 voltage : ");
//  Serial.println(v[1]);
//  Serial.print("AIN1 temp (F): ");
//  Serial.println(temp_f[1]);
//  
  //Serial.print("AIN2 voltage: ");
  //Serial.println(v[2]);
//  Serial.print("AIN1 temp (F): ");
//  Serial.println(temp_f[1]);

  Serial.println();
  delay(1000);
}
