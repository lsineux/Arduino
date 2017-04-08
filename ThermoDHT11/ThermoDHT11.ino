
/******************************************************
 * ThermoDHT11
 *
 * Thermostat with DHT11 sensor, 
 * 16x1 display (Philips LTN111),
 * target temperature set by 10k pot,
 * relay driving the central heating (gas boiler) 
 *
 ******************************************************/

/** "DATA" pin of DHT11 ***************************/
const byte DHT11_PIN = 7;

/** Degree (°) character */
char deg = 223; // ASCII for "°" on the LTN 111
 
/** readDHT11() error codes **********/
enum {
  DHT11_SUCCESS,       //!< Pas d'erreur
  DHT11_TIMEOUT_ERROR, //!< Temps d'attente dépassé
  DHT11_CHECKSUM_ERROR //!< Données reçues erronées
};

/** Schematic for the display ****************************************
*  N°   LTN111  <--->   Arduino
*  1    GND             GND
*  2    VSS             +5V
*  3    Vo(contrast)    GND (via 200ohm resistor)
*  4    RS              PIN_8
*  5    R/W             GND (always writing)
*  6    E               PIN_9
*  7    D0              N/C
*  8    D1              N/C
*  9    D2              N/C
*  10   D3              N/C
*  11   D4              PIN_10
*  12   D5              PIN_11
*  13   D6              PIN_12
*  14   D7              PIN_13
*
*  Note: LTN111 is based on Hitachi HD44780 so we are using the
*  LiquidCrystal library
*
***********************************************************************/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);


/****** Define potentiometer analog input pin ********/
const int potPin = A0;

 
/** Function setup() */
void setup() {
 
  /* Initialize serial port */
  Serial.begin(9600);
  Serial.println(F("DHT11 Thermostat"));

  /* Initialize lcd display 
  *
  *  Caution : 16x1 display is actually seen as a 8x2 lines
  *
  */
  lcd.begin(8, 2);
  
  /* set potentiometer analog pin as an input */
  // no! this would make it a digital input
  // pinMode(potPin, INPUT);     
}
 
/** Function loop() */
void loop() {
 
  /* declaration of variables */
  byte temperature, humidity;
  int rawInput; // that's right, analog input is 10 bits
  byte setting, decimal;
  
 /* read potentiometer, convert into temp setting */
  rawInput = analogRead(potPin);
  //Serial.print(F("Potentiometer value : "));
  //Serial.println(rawInput);
  
  setting = map (rawInput,0,1023,0,255);
  decimal = setting % 10;
  setting /= 10;
  
 
  /* read temperature & humidity and handle errors */
  switch (readDHT11(DHT11_PIN, &temperature, &humidity)) {
  case DHT11_SUCCESS: 
     
    /* Log data to serial interface */
    Serial.print(F("Humidite (%): "));
    Serial.println((int) humidity);
    Serial.print(F("Temperature (^C): "));
    Serial.println((int) temperature);
    /* Print data to LCD screen */
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print((int) temperature);
    lcd.print(deg);
    lcd.print("/");
    lcd.print(setting);
    lcd.print(".");
    lcd.print(decimal);
    lcd.setCursor(0, 1);
    lcd.print(deg);
    lcd.print(" ");
    lcd.print("H=");
    lcd.print((int) humidity);
    lcd.print("%");
    
    break;
 
  case DHT11_TIMEOUT_ERROR: 
    Serial.println(F("Temps d'attente depasse !")); 
    break;
 
  case DHT11_CHECKSUM_ERROR: 
    Serial.println(F("Erreur de checksum !")); 
    break;
  }
   
  
  delay(300);
  
  
}
 
/**
 * Lit la température et le taux d'humidité capté par un capteur DHT11
 *
 * @param pin Broche sur laquelle est câblé le capteur
 * @param temperature Pointeur vers la variable stockant la température
 * @param humidity Pointeur vers la variable stockant le taux d'humidité
 * @return DHT11_SUCCESS si aucune erreur, DHT11_TIMEOUT_ERROR en cas de timeout, ou DHT11_CHECKSUM_ERROR en cas d'erreur de checksum
 */
byte readDHT11(byte pin, byte* temperature, byte* humidity) {
 
  /* data[] -> buffer contenant les données du cpateur
   * counter -> compteur permettant de savoir quel bit est reçu (bitwise)
   * index -> compteur permettant de savoir quel octet est reçu (bitwise)
   * timeout -> compteur pour le timeout
   */
  byte data[5] = { 0 }, counter = 7, index = 0;
  unsigned int timeout;
 
  /* Conversion du numéro de broche Arduino en ports/masque binaire "bas niveau" */
  /* Utiliser les registres du microcontrôleur est bien plus rapide que digitalWrite() */
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *ddr = portModeRegister(port); // Registre MODE (INPUT / OUTPUT)
  volatile uint8_t *out = portOutputRegister(port); // Registre OUT (écriture)
  volatile uint8_t *in = portInputRegister(port); // Registre IN (lecture)
 
  /* Réveil du capteur */
  *ddr |= bit; // OUTPUT
  *out &= ~bit; // LOW
  delay(18); // Temps d'attente à LOW causant le réveil du capteur
  *out |= bit; // HIGH
  delayMicroseconds(40);
  *ddr &= ~bit; // INPUT
 
  /* Attente de la réponse du capteur */
  timeout = 0;
  while(!(*in & bit)) /* Attente d'un état LOW */
    if (++timeout == 10000)
      return DHT11_TIMEOUT_ERROR;
  timeout = 0;
  while(*in & bit) /* Attente d'un état HIGH */
    if (++timeout == 10000)
      return DHT11_TIMEOUT_ERROR;
 
  /* Lecture des données du capteur (40 bits) */
  for (byte i = 0; i < 40; ++i) {
 
    /* Attente d'un état LOW */
    timeout = 0;
    while(!(*in & bit))
      if (++timeout == 10000)
        return DHT11_TIMEOUT_ERROR;
 
    /* Mise en mémoire du temps courant */
    unsigned long t = micros();
 
    /* Attente d'un état HIGH */
    timeout = 0;
    while(*in & bit)
      if (++timeout == 10000)
        return DHT11_TIMEOUT_ERROR;
 
    /* Si le delta Temps est supérieur à 40µS c'est un "1", sinon c'est un "0" */
    if ((micros() - t) > 40)
      data[index] |= (1 << counter); // "1"
    // Le tableau data[] est initialisé à "0" par défaut <span title=";)" class="wp-smiley emoji emoji-wink">;)</span>
 
    /* Si le compteur de bits atteint zéro */
    if (counter-- == 0) {
      counter = 7; /* On passe à l'octet suivant */
      ++index;
    }
  }
 
  /* Format des données :
   * [0] = humidité en %
   * [1] = zéro
   * [2] = température en degrés Celsius
   * [3] = zéro
   * [4] = checksum (humidité + température)
   */
  *humidity = data[0]; 
  *temperature = data[2]; 
 
  /* Vérifie la checksum */
  if (data[4] != (data[0] + data[2]))
    return DHT11_CHECKSUM_ERROR; /* Erreur de checksum */
  else
    return DHT11_SUCCESS; /* Pas d'erreur */
}

