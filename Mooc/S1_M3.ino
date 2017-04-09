int ledR = 7;
int ledV = 8;
int ledB = 9;

void setup() {
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
  pinMode(ledB, OUTPUT);
}

void loop() {
  digitalWrite(ledR,HIGH);
  digitalWrite(ledV,LOW);
  digitalWrite(ledB,LOW);
  delay(1000);
  digitalWrite(ledR,LOW);
  digitalWrite(ledV,HIGH);
  digitalWrite(ledB,LOW);
  delay(1000);
  digitalWrite(ledR,LOW);
  digitalWrite(ledV,LOW);
  digitalWrite(ledB,HIGH);
  delay(1000);
}
