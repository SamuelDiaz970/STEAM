#include <string.h>
#include <assert.h>

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

/*
 * Send "Comm Test - Echo Packet" to SXR, echo "Hello World"
 */

void loop() {
  int i = 0;
  char buffer[20];

    Serial.write((byte)0xf5);
  
    Serial.write((byte)0xFA);
  
    Serial.write((byte)0xf1);
  
    Serial.write((byte)0x7f);

    char str[] = "Hello world\n";
    int str_len  = strlen(str);

    // length of data
    Serial.write((byte)(str_len  & 0xff00)); // MSB
    Serial.write((byte)(str_len & 0xff));    // LSB

    int sum_bytes = 0xf5 + 0xfa + 0xf1 + 0x7f + (str_len & 0xff00) + (str_len & 0xff);

    // transmit data
    for(int i=0; i<str_len; i++) {
      Serial.write(str[i]);
      sum_bytes += (int)str[i];
    }

    // checksum
//    int checksum = -sum_bytes;
//    assert(checksum == 0x7d0);
    int checksum = 0x7d0;
    
    Serial.write((byte)checksum);
//    Serial.println("Reached checksum");
//     if(Serial.available()){
//      Serial.print("data available\n");
//      }

//  if(Serial.available()){
//    while(Serial.available()){
//      buffer[i] = Serial.read();
//      Serial.println(buffer[i++], HEX);
//    }
//  }

// Serial.print(buffer);
//  if((buffer[i] & 0xff) == 0x82 && (buffer[i+1] & 0xff) == 0x05)
//    Serial.println("Successful write");
//  else
//    Serial.println("Command write failed");

 delay(2500);
}
//
//#include <string.h>
//#include <assert.h>
//
///* RS232 Transciever to Spectrometer Code
// * This code is used to send a UART message to the RS232 Transciever. The transciever
// * will then send a message to the Spectrometer. The Spectrometer returns a packet, received
// * by the Transciever and returned to the Arduino
// */
//
//const long baud = 115200;
//
//const short invalidPin = 4; // read invalid signal
//const short invalidOut = 5; // connect LED here
//int valid;
//
//int data;
//
////---------------------------------------------------------------------------------
//// Receiver test
////---------------------------------------------------------------------------------
//
//void setup() {
//  Serial.begin(115200, SERIAL_8N1);
//  // open serial port for transmit -- 8 bit word, 50 baud rate
//  // connect RS-232 receiver pin to D1
//}
//
///*
// * Send "Comm Test - Echo Packet" to SXR, echo "Hello World"
// */
//
//void loop() {
//  int i = 0;
//  char buffer[20];
//
//    Serial.write((byte)0xf5);
//  
//    Serial.write((byte)0xFA);
//  
//    Serial.write((byte)0xf1);
//  
//    Serial.write((byte)0x04);
//    
//    Serial.write((byte)0x00);
//    
//    Serial.write((byte)0x00);
//    
//    Serial.write((byte)0x02);
//    
//    Serial.write((byte)0xe4);
//    
////    char str[] = "Hello world\n";
////    int str_len  = strlen(str);
////
////    // length of data
////    Serial.write((byte)(str_len  & 0xff00)); // MSB
////    Serial.write((byte)(str_len & 0xff));    // LSB
////
////    int sum_bytes = 0xf5 + 0xfa + 0xf1 + 0x7f + (str_len & 0xff00) + (str_len & 0xff);
////
////    // transmit data
////    for(int i=0; i<str_len; i++) {
////      Serial.write(str[i]);
////      sum_bytes += (int)str[i];
////    }
//
//    // checksum
////    int checksum = -sum_bytes;
////    assert(checksum == 0x7d0);
////    int checksum = 0x7d1;
//    
////    Serial.write((byte)checksum);
////    Serial.println("Reached checksum");
////     if(Serial.available()){
////      Serial.print("data available\n");
////      }
//
////  if(Serial.available()){
////    while(Serial.available()){
////      buffer[i] = Serial.read();
////      Serial.println(buffer[i++], HEX);
////    }
////  }
//
//// Serial.print(buffer);
////  if((buffer[i] & 0xff) == 0x82 && (buffer[i+1] & 0xff) == 0x05)
////    Serial.println("Successful write");
////  else
////    Serial.println("Command write failed");
//
// delay(50);
//}
