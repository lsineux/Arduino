
/******************************************************
 * ThermostatI2C
 * by Laurent Sineux, february 2016
 * 
 * Thermostat with DHT11 sensor on pin 7
 * 16x1 display (Philips LTN111) on Adafruit's 
 * I2C LCD shield, using SDA=pin A4, SCL=pin A5
 * target temperature set buttons (UP/DOWN),
 * relay driving the central heating (gas boiler) on pin 8 
 *
 ******************************************************/

/** "DATA" pin of DHT11 ***************************/
const byte DHT11_PIN = 7; // uses 1-wire protocol

/** Relay pin **********************/
const byte RELAY_PIN = 8;

/** LED pin *************/
const byte LED_PIN = 13;

/** Degree (°) character */
char deg = 223; // ASCII for "°" on the LTN 111

/** Default temperature setting at reboot ***/
byte setting = 20;

/** Lag (to avoid relay chattering) **/
/** relay will toggle only after (MAX_LAG) consistent measures */
const byte MAX_LAG =20;
byte lag = MAX_LAG;
 
/** WAIT time between each loop (in ms)*******/
const byte WAIT = 100; // 300 is not enough


#include <Wire.h> // library for I2C protocol
#include <Adafruit_MCP23017.h> // library for I2C DMUX
#include <Adafruit_RGBLCDShield.h> // library for Adafruit LCD shield
#include "dht11.h" // function to read the DHT-11

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
 
/*****************************/
/* Function setup()
/* initialize configuration once
/*****************************/
void setup() {
 
//  /* Initialize serial port */
//  Serial.begin(9600);
//  Serial.println(F("DHT11 Thermostat"));

  /* Initialize lcd display 
  *  Note : 16x1 display is actually seen as a 8x2 lines*/
  lcd.begin(8, 2);
  /* define relay pin as an output */
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}
 
/**************************************/
/* Function loop()
/* main program
/**************************************/
void loop() {
 
  byte temperature; // measured temp,
  byte humidity;      // measured humidity

//read LCD shield buttons
  uint8_t keypressed = lcd.readButtons();
    
  if(keypressed)
  {
      if (keypressed & BUTTON_DOWN)
      {
        setting--;
      }
      if (keypressed & BUTTON_UP)
      {
        setting++;
      }
  }
 
  /* read temperature & humidity and handle errors */
  switch (readDHT11(DHT11_PIN, &temperature, &humidity)) {
  case DHT11_SUCCESS: 
     
    /* Log data to serial interface */
 //   Serial.print(F("Humidite (%): "));
 //   Serial.println((int) humidity);
 //   Serial.print(F("Temperature (^C): "));
  //  Serial.println((int) temperature);

    /* Print data to LCD screen */
 //   lcd.clear();
    lcd.setCursor(0, 0); 
    //start of 1st line, here it's the leftmost char
    lcd.print((int) temperature);
    lcd.print(deg);
    lcd.print("/");
    lcd.print(setting);
    lcd.print(deg);
    lcd.print("  ");
    lcd.setCursor(0, 1);
    // start of 2nd line, here it's the 9th char
    lcd.print(" ");
    lcd.print("H=");
    lcd.print((int) humidity);
    lcd.print("%  ");
    
    /* Relay control */
    if (setting > temperature)
      {
        /* we must turn the relay on if real temp has been cooler than setting */
        /* for a (MAX_LAG) number of times */
        if (--lag <= 1)
          {
            lag = 1;
            digitalWrite(RELAY_PIN, HIGH);
            digitalWrite(LED_PIN, HIGH);
          }
      }
    else
      {
        /* we must turn relay off if abient temp has been above setting */
        /* for a (MAX_LAG) number of times */
        if(++lag >= MAX_LAG)
        {
           lag = MAX_LAG;
           digitalWrite(RELAY_PIN, LOW);
           digitalWrite(LED_PIN, LOW);
        }
      }
//    Serial.print(F("LAG : "));
//    Serial.println((int) lag);

    break;
 
  case DHT11_TIMEOUT_ERROR: 
 //   Serial.println(F("Temps d'attente depasse !")); 
    break;
 
  case DHT11_CHECKSUM_ERROR: 
 //   Serial.println(F("Erreur de checksum !")); 
    break;
  }
   
    
} /**************End of main loop ******************/

