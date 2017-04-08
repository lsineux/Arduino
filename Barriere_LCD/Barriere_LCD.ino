/*
  Barriere_LCD
  (Exercice pour le MOOC semaine 10)
  
  Branchements écran Philips LTN 211R-10 :
 LCD    Arduino    Libellé
  ===============================================================
    1      -        GND
    2      -        +5v
    3      -        Contraste
    4      2        RS (register select)
    5      -        RW (read / write) Branché sur 0v
    6      3        E  (enable)
    7      NC       D0
    8      NC       D1
    9      NC       D2
   10      NC       D3    
   11      4        D4 (données bit 4)
   12      5        D5 (données bit 5)
   13      6        D6 (données bit 6)
   14      7        D7 (données bit 7)
*/

#include <Servo.h> // bibliothèque de pilotage des servos
#include <LiquidCrystal.h> // bibliothèque pour écran LCD
                           // à base d'Hitachi HD44100

// Déclaration des GPIOs
const int pinBouton = 8; // le bouton d'appel est sur la patte 8
const int pinServo = 9; // le servo est sur la patte 9
const int pinVerte = 10; // LED verte sur la patte 10
const int pinRouge = 11; // LED rouge sur la patte 11

// Nombre de voitures
int nbVoitures = 0;

// Déclaration du Servo
Servo monServo; // objet qui permet de manipuler un servo
#define offset 15 // mon 0° du servo est décalé de 15°
int currentPos = 0 + offset;    // Position du servo (en degrés) 

// Déclaration des pattes du LCD
#define LCD_rs 2
#define LCD_enable 3
#define LCD_d4 4
#define LCD_d5 5
#define LCD_d6 6
#define LCD_d7 7

// Déclarer l'afficheur LCD
  LiquidCrystal lcd(LCD_rs, LCD_enable, LCD_d4, LCD_d5, LCD_d6, LCD_d7);

/************setup********************/
// Boucle d'initialisation, exécutée une seule fois au démarrage
// du programme
void setup() 
{ 
  //déclaration des pattes en entrée
  pinMode(pinBouton, INPUT);
  
  //déclaration des pattes des LEDs en sortie
  pinMode(pinVerte, OUTPUT);
  pinMode(pinRouge, OUTPUT);
  
  // relie le Servo à la patte pinServo
  monServo.attach(pinServo);  
  
  // On met le feu au rouge, on éteint le vert
  digitalWrite(pinRouge,HIGH);
  digitalWrite(pinVerte,LOW);
  
  // On démarre avec la barrière fermée
  monServo.write(currentPos); // on démarre avec barrière fermée

  // afficher la ligne du haut
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Voiture  numero:");
  lcd.setCursor(0,1);
  lcd.print(nbVoitures);
} 


/**********mouvementBarriere*********/
void mouvementBarriere(int posDebut, int posFin)
{
  int pos;
  if(posDebut < posFin) 
  {
   // si fin > début, on ouvre
  // -----Ouverture
  for(pos = posDebut; pos <= posFin ; pos++)  
    {
       monServo.write(pos);             // aller à la position pos 
       delay(10);                       // attendre 10ms 
    }
  }
  else 
  {
    // si fin <= début, on ferme
   // ---Fermeture
    for(pos = posDebut; pos >= posFin ; pos--)  
    {
       monServo.write(pos);             // aller à la position pos 
       delay(10);                       // attendre 10ms 
    }    
  }
  currentPos=pos; // on se rappelle de la dernière position !
}


/*******boucle principale************/ 
void loop() 
{ 
  
  if(digitalRead(pinBouton)==HIGH) // si un appui est détecté
  {
    // On ouvre la barrière
    mouvementBarriere(currentPos,90 + offset);
    // mon servo est décalé de 15°
    
    // On met le feu au vert, on éteint le rouge
    digitalWrite(pinVerte,HIGH);
    digitalWrite(pinRouge,LOW);
    
    // on attend 5s
    delay(5000);
  
    // On met le feu au rouge, on éteint le vert
    digitalWrite(pinRouge,HIGH);
    digitalWrite(pinVerte,LOW);
  
    // et on referme la barrière
 
   mouvementBarriere(currentPos, 0 + offset);
   // (mon servo est décalé de 15°)
 
    // on incrémente le nombre de voitures
      nbVoitures++;
      lcd.setCursor(0,1);
      lcd.print(nbVoitures);

  }
  // ...sinon, on ne fait rien, on reboucle !
}
