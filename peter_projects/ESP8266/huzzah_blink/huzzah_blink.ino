void setup() {
  pinMode(0, OUTPUT);
}
void loop() {
  digitalWrite(0, HIGH);
  delay(100);
  digitalWrite(0, LOW);
  delay(10);
}
