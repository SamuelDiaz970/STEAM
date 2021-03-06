
int Relay = 40;     // Digital pin 40

void setup() {
  
  pinMode(Relay, OUTPUT);

}

void loop() {
  digitalWrite(Relay, HIGH);
  delay(1000);
  digitalWrite(Relay, LOW);
  delay(1000);
}
