/* RS232 Transciever to Spectrometer Code
 * This code is used to send a UART message to the RS232 Transciever. The transciever
 * will then send a message to the Spectrometer. The Spectrometer returns a packet, received
 * by the Transciever and returned to the Arduino
 */
const long baud = 9600;

const short invalidPin = 2; // read invalid signal
const short invalidOut = 3; // connect LED here
int valid;

int i = 0;
int data;

//---------------------------------------------------------------------------------
// Receiver test
//---------------------------------------------------------------------------------

void setup() {
  Serial.begin(baud, SERIAL_8N1);
  // open serial port for transmit -- 8 bit word, 50 baud rate
  // connect RS-232 receiver pin to D1
}

void loop() {
  int i = 0;
  char buffer[20];

//  Serial.write((byte)0xf5);
//
//  Serial.write((byte)0xFA);
//  
//  Serial.write(3);
//  
//  Serial.write(5);
//  
//  Serial.write(0);
//  
//  Serial.write(0);
//  
//  Serial.write((byte)0xFE);
//  
//  Serial.write((byte)0x09);

  if(Serial.available()){
    while(Serial.available()){
      buffer[i] = Serial.read();
    i++;
  }
  }

  Serial.print(buffer);
}
