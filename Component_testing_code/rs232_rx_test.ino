const long baud = 50;

const short invalidPin = 1; // read invalid signal
const short invalidOut = 2; // connect LED here
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
  // Then after another 40 bytes, switch to valid data
  if(i < 20 || i > 60) {
    data = rand()*256;
  } else {
    data = 0;
  }
  Serial.print("Data sent to RS-232: "); Serial.println(data); // print data to console
  Serial.println();
  Serial.write(data); // send byte to receiver

  valid = digitalRead(invalidPin);
  if (valid) // invalid pin is active low
    Serial.println("Valid");
  else
    Serial.println("Invalid");
  // check transmitted data on AD2

  delay(500); // delay 0.5 second before next iteration (time amount can be changed)
  i++;
}
