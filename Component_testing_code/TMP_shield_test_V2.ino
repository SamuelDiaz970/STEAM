//#include <SparkFun_HIH4030.h>
#include <Wire.h>
// This is the Arduino Pin that will read the sensor output
int sensePin0 = A0; int sensePin1 = A1; int sensePin2 = A2; int sensePin3 = A3; int sensePin4 = A4; int sensePin5 = A5; int sensePin6 = A6;                         

// The variable we will use to store the sensor input
int sensorInput[7];                    

// The variable we will use to store temperature in degrees.
double temp[7];

// Analog IO Pin Connected to OUT
#define HIH4030_OUT A0

// Supply Voltage - 5 V
#define HIH4030_SUPPLY 5

//HIH4030 sensorSpecs(HIH4030_OUT, HIH4030_SUPPLY);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);                 // Start the Serial Port at 9600 baud 
//Wire.begin();                     // I2C: Utilized by TMP102 Temperature Sensor
}

void printData(double * temp){

  for (int i=0; i<7; i++) {
    Serial.print("Current Temperature (pin A");
    Serial.print(i+1); Serial.print("): ");

    Serial.print(temp[i]); Serial.print(" F    ");
    Serial.println(millis());
  }
}

void loop() {
  
  // put your main code here, to run repeatedly:
sensorInput[0]  = analogRead(A1);     // TMP 
sensorInput[1]  = analogRead(A2);     // TMP 
sensorInput[2]  = analogRead(A3);     // TMP 
sensorInput[3]  = analogRead(A4);     // TMP 
sensorInput[4]  = analogRead(A5);     // TMP 
sensorInput[5]  = analogRead(A6);     // TMP 
sensorInput[6]  = analogRead(A7);     // TMP 

  for(int i=0; i<7; i++) {
    temp[i] = (double) sensorInput[i] / 1024.0; // find percentage of input reading
    temp[i] *= 5;                               // multiply by 5V to get voltage
    temp[i] -= 0.5;                             // Subtract the offset 
    temp[i] *= 100;                             // Convert to degrees C

    temp[i] = 9.0/5.0*temp[i] + 32.0;           // Convert to deg F
  }
  
printData(temp);
Serial.println();                     // Return Space Between Readings

delay(500); // wait 0.5 sec between readings

}

//// Print to txt file 
//Serial.println(value);
//
//import processing.serial.*;
//Serial mySerial;
//PrintWriter output;
//void setup() {
//   mySerial   = new Serial( this, Serial.list()[0], 9600 );
//   output     = createWriter( "data.txt" );
//}
//void draw() {
//    if (mySerial.available() > 0 ) {
//         String value = mySerial.readString();
//         if ( value != null ) {
//              output.println( value );
//         }
//    }
//}
//
//void keyPressed() {
//    output.flush();  // Writes the remaining data to the file
//    output.close();  // Finishes the file
//    exit();  // Stops the program
//}
//
//
