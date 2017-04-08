/*
  FeuRouge
  (Exercice pour le MOOC)
  Je dois allumer:
  la led verte pendant 3 secondes
  la led jaune pendant 1 secondes
  la led rouge pendant 3 secondes
  etc
 */
 
// Branchements
int verte = 11;
int jaune = 12;
int rouge = 13;

// Initialisation quand on appuie reset:
void setup() {                
  // définir les broches comme des sorties.
  pinMode(verte, OUTPUT);     
  pinMode(jaune, OUTPUT);     
  pinMode(rouge, OUTPUT);     
}

// boucle principale :
void loop() {
  digitalWrite(jaune, HIGH);   // Allumer la LED
  delay(1000);               // Attendre 3s
  digitalWrite(jaune, LOW);    // Eteindre la LED
  digitalWrite(rouge, HIGH);   // Allumer la LED
  delay(3000);               // Attendre 3s
  digitalWrite(rouge, LOW);    // Eteindre la LED
  digitalWrite(verte, HIGH);   // Allumer la LED
  delay(3000);               // Attendre 3s
  digitalWrite(verte, LOW);    // Eteindre la LED
}
