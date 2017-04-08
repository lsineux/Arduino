/*
  256 Shades of Processing
  (Exercice pour le Mooc semaine 11)
  Lit une entrée analogique A0
  écrit la valeur sur le port série,
  de 0 à 1023
*/

#define pinPot A0 // le potentiomètre est branché sur A0

/* fonction setup exécutée une seule fois */
void setup() {
  // initialise la liaison série à 9600 bauds
     Serial.begin(9600);
}

/* Boucle principale */
void loop() {
  int mesure;
  // lire la valeur du potentionmètre
  mesure = analogRead(pinPot);
  // écrire la valeur sur le port série
  Serial.println(mesure);
  delay(10);        // On temporise pour ne pas saturer le port série
}
