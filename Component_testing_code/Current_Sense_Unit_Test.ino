

#include <Wire.h>
#include <math.h>
#define Addr 0x34


// configuration bytes--last bit signifies single-ended mode
#define AIN0_CONFIG 0b00000001
#define AIN1_CONFIG 0b00000011
#define AIN2_CONFIG 0b00000101
#define AIN3_CONFIG 0b00000111



#define Vref 2.048 // Internal voltage referance of ADC

#define op1_ideal 2 // Ideal gain of stage 1 opamp relative to sense resistor
#define op2_ideal 8 // Ideal gain of output of stage 2 opamp relative to sense resistor

// ------------------------------------------------------------------------------------------- 
    
    double supply_voltage = 0.5; // (UPDATE) Gain and current calculations depend on this value                                                                                  

// ------------------------------------------------------------------------------------------

void setup() {

  
  Wire.begin();
  Serial.begin(9600);
  delay(300);
  
  Serial.println("Beginning test");
}

void loop() {
  uint16_t data[4]; // Use array for all 4 channels in case a unit test is ever needed with all 4
  float v[4];
  double ideal_high_side_voltage;
  double real_gain;
  double real_op1_gain;
  double real_op2_gain;
  double ideal_current;
  double real_gain_current;
  double current_ideal_gain;
  
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
  
  for(int i = 0; i < 2; i++) {
    // read first byte
    data[i] = Wire.read() << 8;
    // read second byte
    data[i] |= Wire.read();
    data[i] &= 0xfff;
    
    // ADC powered off 3.3V for current sense unit testing
    v[i] = data[i] / 4095.0 * 3.3;
                             
  }

      // Calculate variables using voltage array and constants
     
    ideal_high_side_voltage = supply_voltage * (0.05 / (1.428 + 0.05) ); // Voltage divider of supply voltage across RS
    real_gain = v[1] / ideal_high_side_voltage; // Real voltage gain of full circuit                                                                                
    real_op1_gain = v[0] / ideal_high_side_voltage; // Real gain of stage 1 op amp  
    real_op2_gain = v[1] / v[0] ; // Real gain of stage 2 op amp

    ideal_current = ideal_high_side_voltage / 0.05; // Realistically what the current should be
    real_gain_current = (v[1] / real_gain) / 0.05; // Current calculation with gain values that occur
    current_ideal_gain = (v[1] / 8) / 0.05;  // Current calculation with ideal gain values


  
  // Output data to serial monitor

  Serial.print("Number of ADC counts (AIN0) : ");
  Serial.println(data[0]);
  Serial.print("Number of ADC counts (AIN1) : ");
  Serial.println(data[1]);
  
  Serial.print("Ideal high side voltage (V) : ");
  Serial.println(ideal_high_side_voltage);
  Serial.print("Real calculated gain : ");
  Serial.println(real_gain);
  Serial.print("Real stage 1 gain : ");
  Serial.println(real_op1_gain);
  Serial.print("Real stage 2 gain : ");
  Serial.println(real_op2_gain);
  Serial.print("Ideal current (A) : ");
  Serial.println(ideal_current);
  Serial.print("Real measured current (A) : ");
  Serial.println(real_gain_current);
  Serial.print("Current calculated using ideal gain (A) : ");
  Serial.println(current_ideal_gain);

  


  Serial.println();
  delay(15000); // wait 15 sec before next reading
}
