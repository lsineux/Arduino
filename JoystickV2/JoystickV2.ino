/*
   Joystick
   Reads ATARI Joystick position and sends to serial
 */

// Correspondance of Arduino pins vs Atari pins
int pinUp    = 0; // Atari Pin 1 (my favourite ;-)
int pinDown  = 1; // Atari Pin 2
int pinLeft  = 2; // Atari Pin 3
int pinRight = 3; // Atari Pin 4
int pinFire  = 4; // Atari Pin 6
// ... and just for info GND is Atari pin 8.

// Define global toggle variables to see if key pressed or released
  int toggleUp = HIGH;
  int toggleDown = HIGH;
  int toggleLeft = HIGH;
  int toggleRight = HIGH;
  int toggleFire = HIGH;

// Define value of space character
# define KEY_SPACE 32

// the setup routine runs once when you press reset:
void setup() {
  // initialize Keyboard
  Keyboard.begin();
  // set up the pins as inputs with internal pullup resistors :
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);  
  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);
  pinMode(pinFire, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  int keyPressed;
  // read pinUp
  keyPressed = digitalRead(pinUp);
  // if transition high to low, key is pressed
  if (keyPressed == LOW && toggleUp == HIGH) Keyboard.press(KEY_UP_ARROW);
  // if transition is low to high, key is released
  if (keyPressed == HIGH && toggleUp == LOW) Keyboard.release(KEY_UP_ARROW);
  // memorize status in global variable for next time
  toggleUp = keyPressed;

  // read pinDown
  keyPressed = digitalRead(pinDown);
  // if transition high to low, key is pressed
  if (keyPressed == LOW && toggleDown == HIGH) Keyboard.press(KEY_DOWN_ARROW);
  // if transition is low to high, key is released
  if (keyPressed == HIGH && toggleDown == LOW) Keyboard.release(KEY_DOWN_ARROW);
  // memorize status in global variable for next time
  toggleDown = keyPressed;

  // read pinLeft
  keyPressed = digitalRead(pinLeft);
  // if transition high to low, key is pressed
  if (keyPressed == LOW && toggleLeft == HIGH) Keyboard.press(KEY_LEFT_ARROW);
  // if transition is low to high, key is released
  if (keyPressed == HIGH && toggleLeft == LOW) Keyboard.release(KEY_LEFT_ARROW);
  // memorize status in global variable for next time
  toggleLeft = keyPressed;

  // read pinRight
  keyPressed = digitalRead(pinRight);
  // if transition high to low, key is pressed
  if (keyPressed == LOW && toggleRight == HIGH) Keyboard.press(KEY_RIGHT_ARROW);
  // if transition is low to high, key is released
  if (keyPressed == HIGH && toggleRight == LOW) Keyboard.release(KEY_RIGHT_ARROW);
  // memorize status in global variable for next time
  toggleRight = keyPressed;

  // read pinFire
  keyPressed = digitalRead(pinFire);
  // if transition high to low, key is pressed
  if (keyPressed == LOW && toggleFire == HIGH) Keyboard.press(KEY_SPACE);
  // if transition is low to high, key is released
  if (keyPressed == HIGH && toggleFire == LOW) Keyboard.release(KEY_SPACE);
  // memorize status in global variable for next time
  toggleFire = keyPressed;

  // print out the state of the button:
  delay(100);        // delay in between reads for stability
}



