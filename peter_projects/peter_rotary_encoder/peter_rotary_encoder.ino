/* Read Quadrature Encoder
  * Connect Encoder to Pins encoder0PinA, encoder0PinB, and +5V.
  *
  * Sketch by max wolf / www.meso.net
  * v. 0.1 - very basic functions - mw 20061220
  *
  */


int val;
int encoder0PinA = 5;
int encoder0PinB = 6;
int encoder0PinClick = 4;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;
int c = LOW;
int clickstate = LOW;

void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode (encoder0PinClick, INPUT);
  Serial.begin(9600);
}

void loop() {

  c = digitalRead(encoder0PinClick);
  if (c != clickstate) {
    if (c == LOW) {
        Serial.print ("click down\n");
    } else {
        Serial.print ("click up\n");
    }
   clickstate = c;
  }
  
  
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    Serial.print (encoder0Pos);
    Serial.print ("\n");
  }
  encoder0PinALast = n;
}
