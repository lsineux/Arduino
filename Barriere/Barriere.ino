/*
  Barriere
  (Exercice pour le MOOC semaine 6)
*/

#include <Servo.h> // bibliothèque de pilotage des servos

// Déclaration des GPIOs
const int pinBouton = 2; // le bouton d'appel est sur la patte 2
const int pinServo = 9; // le servo est sur la patte 9
const int pinVerte = 10; // LED verte sur la patte 10
const int pinRouge = 11; // LED rouge sur la patte 11
 
Servo monServo; // objet qui permet de manipuler un servo
 
int pos = 0;    // Position du servo (en degrés) 

// Boucle d'initialisation, exécutée une seule fois au démarrage
// du programme
void setup() 
{ 
  // Initialise la communication avec l'ordinateur
  Serial.begin(9600);
  //déclaration des pattes en entrée
  pinMode(pinBouton, INPUT);
  //déclaration des pattes en sortie
  pinMode(pinVerte, OUTPUT);
  pinMode(pinRouge, OUTPUT);
  // relie le Servo à la patte pinServo
  monServo.attach(pinServo);   
  // On met le feu au rouge, on éteint le vert
  digitalWrite(pinRouge,HIGH);
  digitalWrite(pinVerte,LOW);
  // On démarre avec la barrière fermée
  monServo.write(15); // mon servo est décalé de 15°
                      // 15° = fermée
                      // 105° = ouverte d'un quart de tour
} 


// boucle principale 
void loop() 
{ 
  
  if(digitalRead(pinBouton)==HIGH) // si un appui est détecté
  {
    // On envoie un message sur l'ordi
    Serial.println("Bouton appuyé !");  
    // On ouvre la barrière
    for(pos = 15; pos <= 105 ; pos++)  // de 0 à 90° + 15°
    {
       monServo.write(pos);             // aller à la position pos 
       delay(20);                       // attendre 20ms
                                         // (j'ai un servo lent <:o) 
    }
    // On met le feu au vert, on éteint le rouge
    digitalWrite(pinVerte,HIGH);
    digitalWrite(pinRouge,LOW);
    // On envoie un message sur l'ordi
    Serial.println("C'est ouvert !");  
    // on attend 5s
    delay(5000);
    // On met le feu au rouge, on éteint le vert
    digitalWrite(pinRouge,HIGH);
    digitalWrite(pinVerte,LOW);
    // et on referme la barrière
    for(pos = 105; pos >=15 ; pos-- )  // de 90° à 0 
    {
      monServo.write(pos);             // aller à la position pos 
      delay(20);                       // attendre 20ms 
    }
    // On envoie un message sur l'ordi
    Serial.println("C'est fermé !");  
  }
  // ...sinon, ben on ne fait rien, on reboucle !
}
