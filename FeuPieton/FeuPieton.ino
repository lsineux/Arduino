/*
  FeuPieton
  (Exercice pour le MOOC semaine 4)
 */
 
// Branchements
const int pinPietonVerte = 8;
const int pinPietonRouge = 9;
const int pinVerte = 10;
const int pinJaune = 11;
const int pinRouge = 12;
const int pinBouton = 2;
// Variables globales
int bouton = 0;

// Initialisation quand on appuie reset:
void setup() {                
  // définir les broches en sortie
  pinMode(pinPietonVerte, OUTPUT);     
  pinMode(pinPietonRouge, OUTPUT);     
  pinMode(pinVerte, OUTPUT);     
  pinMode(pinJaune, OUTPUT);     
  pinMode(pinRouge, OUTPUT);
  // définir les broches en entrée
  pinMode (pinBouton, INPUT);  
  // initialiser les feux des piétons, la première fois
  digitalWrite(pinPietonVerte, LOW); // éteindre le vert piétons
  digitalWrite(pinPietonRouge, HIGH); //rouge pour les piétons
}

// boucle principale :
void loop() {
  // feu jaune
  digitalWrite(pinJaune, HIGH); // jaune pour les voitures
  delay(1000);               // Attendre 1s
  // feu rouge
  digitalWrite(pinJaune, LOW);    // Eteindre le jaune
  digitalWrite(pinRouge, HIGH);   // Allumer le rouge voiture
  if (bouton == 1) // si un piéton a appuyé pendant le feu vert
  {
   digitalWrite(pinPietonVerte, HIGH); // vert pour les piétons
   digitalWrite(pinPietonRouge, LOW); // éteindre rouge piétons
   delay(5000); // attendre 5s
   digitalWrite(pinPietonVerte, LOW); // éteindre le vert piétons
   digitalWrite(pinPietonRouge, HIGH); //rouge pour les piétons
  }
  else
  {
  delay(3000);               // Attendre 3s
  }
  digitalWrite(pinRouge, LOW);    // Eteindre le rouge
  digitalWrite(pinVerte, HIGH);   // Allumer la LED
  // lecture du bouton, qui doit être "maintenu"
  // c-à-d pressé longtemps, disons pendant 1 seconde
  for (int i=0;i<30;i++) // 30/10e de secondes, soit 3s
  {
     bouton += digitalRead(pinBouton); //compter le nombre d'appuis
     delay(100); // 1/10 de seconde
  }
  // si on a appuyé pendant 1s ou plus, l'appel est validé, sinon
  // ça ne compte pas
  if (bouton >= 9) bouton = 1;
  else bouton = 0;
  digitalWrite(pinVerte, LOW);    // Eteindre la LED
}
