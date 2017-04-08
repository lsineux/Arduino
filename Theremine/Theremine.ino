/*
  Theremine
  (TP pour la semaine 5 du MOOC)
  On mesure la luminosité sur la broche A0
  On sort un son sur la broche 8 dont
  la fréquence dépend de la luminosité
 
 */

// Initialisation des constantes :
const int analogInPin = A0;   // Numéro de la broche à laquelle est connecté la photorésistance
const int analogOutPin = 8;   // Numéro de la broche à laquelle est connectée la LED

int sensorValue = 0;          // Valeur lue sur la photorésistance
int freq = 0;                // fréquence du son joué


// fonction exécutée une seule fois au démarrage
void setup() {
  // Initialise la communication avec l'ordinateur
  Serial.begin(9600);
  
  // Indique que la broche analogOutPin est une sortie :
  pinMode(analogOutPin, OUTPUT);      
  // Indique que la broche analogInPin est une entrée :
  pinMode(analogInPin, INPUT);     
}

// boucle principale
void loop() {
  // lit la valeur de la photorésistance et
  // stocke le résultat dans sensorValue :
  sensorValue = analogRead(analogInPin);            
  // convertit sensorValue de 0 à 1023
  // en fréquence de 50Hz à 30kHz :
  freq = map(sensorValue, 0, 1023, 50, 30000);  
  // On joue la note
  tone(analogOutPin, freq);
  delay(200);

  // envoie tout ça vers l'ordinateur
  Serial.print("Luminosité = " );                       
  Serial.print(sensorValue);      
  Serial.print("\t fréquence = ");      
  Serial.println(freq);
}

