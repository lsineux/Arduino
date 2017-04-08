/*
   Joystick
   Reads ATARI Joystick position and sends to serial
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pinUp    = 0; // Atari Pin 1 (my favourite ;-)
int pinDown  = 1; // Atari Pin 2
int pinLeft  = 2; // Atari Pin 3
int pinRight = 3; // Atari Pin 4
int pinFire  = 4; // Atari Pin 6
// just for info GND is pin 8...

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // set up the pins as inputs :
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);  
  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);
  pinMode(pinFire, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int keyPressed;
  keyPressed = digitalRead(pinUp);
  if (keyPressed == LOW) Serial.println("Up");
  keyPressed = digitalRead(pinDown);
  if (keyPressed == LOW) Serial.println("Down");
  keyPressed = digitalRead(pinLeft);
  if (keyPressed == LOW) Serial.println("Left");
  keyPressed = digitalRead(pinRight);
  if (keyPressed == LOW) Serial.println("Right");
  keyPressed = digitalRead(pinFire);
  if (keyPressed == LOW) Serial.println("Fire");
  // print out the state of the button:
  delay(100);        // delay in between reads for stability
}



