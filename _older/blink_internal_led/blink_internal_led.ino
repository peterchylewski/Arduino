
int ledPinRed = 1;
int ledPinGreen = 2;
int ledPinYellow = 3;

int myDelay = 500;
int potPin = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  myDelay = analogRead(potPin);    // read the value from the sensor
  Serial.println(myDelay, DEC);
  
  digitalWrite(ledPinGreen, HIGH);
  delay(myDelay);
  digitalWrite(ledPinGreen, LOW);
  digitalWrite(ledPinRed, HIGH);
  delay(myDelay);
  digitalWrite(ledPinRed, LOW);
  digitalWrite(ledPinYellow, HIGH);
  delay(myDelay);
  digitalWrite(ledPinYellow, LOW);
}
