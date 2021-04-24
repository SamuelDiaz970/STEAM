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
  // Start with valid data, then (after 20 bytes) switch to invalid
  // Then after another 20 bytes, switch to valid data
  if(i < 20 || i > 40) {
    data = rand()%256;
  } else {
    data = 0;
  }
  Serial.print("Data sent to RS-232: "); Serial.println(data); // print data to console
  Serial.write(data); // send byte to receiver

  valid = digitalRead(invalidPin);
  if (valid) // invalid pin is active low
    Serial.println("Valid");
  else
    Serial.println("Invalid");
  // check transmitted data on AD2

  Serial.println();

  delay(500); // delay 0.5 second before next iteration (time amount can be changed)
  i++;

  if(i >= 60) i=0; // reset after 60 cycles
}
