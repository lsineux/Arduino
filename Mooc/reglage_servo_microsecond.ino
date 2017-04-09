#include <Servo.h> // Inclure la librairie Servo
Servo myservo;     // créer une variable de type servo
int nbre = 0;     // Variable pour tester l'angle de rotation du servo
void setup() {
  Serial.begin(9600); // Initialisation de la liaison série
  myservo.attach(9);  // Connect le servo moteur à la broche 9


}

void loop() {
  while (Serial.available() > 0) { //Tant que le moniteur série à une donnée à envoyer
    int nbre = Serial.parseInt(); // On place la donnée dans une variable nbre
    // parseInt renvoie le premier nombre entier valide placé dans la mémoire tampon de la liaison série
    Serial.println(nbre);
    myservo.writeMicroseconds(nbre); // affecte l'angle de rotation au servo moteur
  }

}








