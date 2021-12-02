/* RS232 Transciever to Spectrometer Code
 * This code is used to send a UART message to the RS232 Transciever. The transciever
 * will then send a message to the Spectrometer. The Spectrometer returns a packet, received
 * by the Transciever and returned to the Arduino
 */
 
const long baud = 115200;

const short invalidPin = 4; // read invalid signal
const short invalidOut = 5; // connect LED here
int valid;

int data;

//---------------------------------------------------------------------------------
// Receiver test
//---------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  // open serial port for transmit -- 8 bit word, 50 baud rate
  // connect RS-232 receiver pin to D1
}

void loop() {
  int i = 0;
  char buffer[20];

  Serial.write((byte)0xf5);

  Serial.write((byte)0xFA);
  
  Serial.write(3);
  
  Serial.write(5);
  
  Serial.write(0);
  
  Serial.write(0);
  
  Serial.write((byte)0xFE);
  
  Serial.write((byte)0x09);

  Serial.println("\xf5\xfa\x3\x5\0\0\xfe\x9");

//  if(Serial.available()){
//    while(Serial.available()){
//      buffer[i] = Serial.read();
//    Serial.println(buffer[i], HEX);
//    if((buffer[i-1] & 0xff == 0x82) && (buffer[i] & 0xff == 0x05)) {
//      Serial.println("Successful write");
//      break;
//    }
//    i++;
//  }
//  }

// Serial.print(buffer);
//  if((buffer[i] & 0xff) == 0x82 && (buffer[i+1] & 0xff) == 0x05)
//    Serial.println("Successful write");
//  else
//    Serial.println("Command write failed");

 delay(3000);
}
