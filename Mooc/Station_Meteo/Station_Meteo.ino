/*
  Station Météo
  Laurent Sineux, avril 2017

  Exercice du Mooc "Fabriquez un objet connecté"
  
  Montage :
   Shield Ethernet sur les broches 10, 11, 12, 13
   LED RVB sur les broches R=5, V=6, B=7
   Servo sur la broche n°4
*/

#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>
#include <Servo.h>

// config carte réseau
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 177); //si le DHCP ne marche pas
IPAddress myDns(208, 67, 222, 222); //OpenDNS

// initialize the library instance:
EthernetClient client;
TextFinder finder( client );
Servo servoM;

// Branchements
int pinServo = 4;
int ledR = 5;
int ledV = 6;
int ledB = 7;


// Angles
#define angleBonnet 140
#define anglePluie 100
#define angleNormal 62
#define angleSoleil 27

// Seuils de température
#define tempFroid 5 // Froid si <5°C
#define tempChaud 20 // Chaud si >20°C
#define tempCanicule 25
#define tempGel 0
// Seuil de pluie
#define mmPluie 6 // Pluie si plus de 6mm

boolean firstConnection = 1;

boolean x = 0;
char server[] = "infoclimat.fr";

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 20L*1000L; // delay between updates, in milliseconds : 1 heure
// the "L" is needed to use long type numbers
unsigned long retryInterval = postingInterval;

/* Fonction d'initialisation des variables */

void setup() 
{
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
  pinMode(ledB, OUTPUT);
  Serial.begin(9600);
  
  startDHCP();

  servoM.attach(pinServo);
  delay(200);
  servoM.write(angleBonnet);
  // génère une séquence aléatoire
  // en lisant une entrée non connectée
  randomSeed(analogRead(0));
}

/* boucle principale */
void loop() 
{
  if (!firstConnection) 
  {
    if (millis() - lastConnectionTime > postingInterval)
    {
      Serial.println("Autre connexion");
      httpRequest();
    }
    
  }
  else
  {
    Serial.println("Première connexion");
    httpRequest();
  }

}


/* fonction httpRequest
Interroge serveur web météo */

void httpRequest() 
{

  client.stop();
  float TK; // température en Kelvin
  float TC; // température en Celcius
  float MM; // millimètres de pluie
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

//requête serveur  
  if (client.connect(server, 80))
  {
    Serial.println("connecting...");
    client.println("GET http://www.infoclimat.fr/public-api/gfs/xml?_ll=48.8655,2.3426&_auth=ABpSRQ5wU3FQfVJlAnRQeVA4UmcAdgEmVCgFZg9qAH1SOQNhVTNcO1AgUSxVZwIoU2NTNw0wBjgCflIqC2VQMwBgUjYOZVM3UDtSLwItUHlQYlItACABOFQ%2BBWcPfABlUjkDeVU3XDdQOlEtVWYCNlN%2FUywNMwY6AmdSMgtkUDMAYFI2DmxTMVAgUi8CN1BjUGRSMQBtAW1UZAVgD2sAY1I5AzVVYlw%2FUCFRN1VmAjVTZFMyDTYGNgJmUioLeVBKABBSKw4tU3NQalJ2Ai9QMVA9UmY%3D&_c=573a00a7aa38c7671cef9ed6318a073c HTTP/1.1");
    client.println("Host: infoclimat.fr");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();
    retryInterval = postingInterval; // en cas de succès on remet un intervalle long

// recherche de la température à 24H (en Kelvin)
    if (finder.find("<echeance hour=\"159\" ")) 
    {
      if (finder.find("<temperature>")) 
      {
        if (finder.find("<level val=\"2m\">")) 
        {
          TK = finder.getFloat(' ');
          Serial.print("TK : "); Serial.println(TK);
          TC = TK - 273, 15;
        }

      }
    }
    else     // si le serveur est injoignable,
          // mode démo : on renvoit n'importe quoi
    {
       Serial.println("Contenu illisible, envoi valeurs aléatoires");
       TC = float(random(-10,36)); //envoyer une température aléatoire
       MM = float(random(0,9)); // envoyer une pluie aléatoire
       //startDHCP();
       retryInterval = 3L*1000L; // réessayer dans 3 secondes au lieu de 20 
     }

// recherche de la pluie prévue, en millimètres
    if (finder.find("<pluie interval=\"3h\">")) 
    {
      MM = finder.getFloat();
    }
    client.stop();
  } 
  else     // si le serveur est injoignable,
          // mode démo : on renvoit n'importe quoi
  {
    Serial.println("échec de connexion, envoi valeurs aléatoires");
    TC = float(random(-10,36)); //envoyer une température aléatoire
    MM = float(random(0,9)); // envoyer une pluie aléatoire
    retryInterval = 3L*1000L; // réessayer dans 3 secondes au lieu de 20 
  }

  

  firstConnection = 0;
  afficheAlarme(TC);
  afficheMeteo(TC,MM);
}

/* fonction afficheMeteo
 * tourne le servo pour afficher le personnage
 * correspondant à la température et à la pluie
 */
void afficheMeteo(float temp, float pluie) 
{

  Serial.print("TC : "); Serial.println(temp);
  Serial.print("MM : "); Serial.println(pluie);

  if(pluie < mmPluie)
  {
    if(temp > tempChaud)
    {
      Serial.println("lunettes et pieds nus");
      servoM.write(angleSoleil); //0
    }
    else if(temp <= tempFroid)
    {
      Serial.println("Bonnet et Moonboots");
      servoM.write(angleBonnet); //135 degré
    }
      else // ni chaud ni froid, normal
    {
      Serial.println("Chaussures de ville");
      servoM.write(angleNormal); // 45 degré - tempéré et pas de pluie
    }
 
  }
  else // s'il pleut
  {
    Serial.println("capuche et bottes");
    servoM.write(anglePluie); //90 degré Pluie
  }
} // fin de la fonction afficheMeteo

/* fonction afficheAlarme
 * positionne la LED RVB selon la température :
 * rouge = canicule
 * vert = normal
 * bleue = risque de gel
 */
void afficheAlarme(float temp)
{

  if(temp >= tempCanicule)
  {
    digitalWrite(ledR,HIGH);
    digitalWrite(ledV,LOW);
    digitalWrite(ledB,LOW);
  }
  else if(temp <= tempGel)
  {
    digitalWrite(ledR,LOW);
    digitalWrite(ledV,LOW);
    digitalWrite(ledB,HIGH);
  }
  else
  {
    digitalWrite(ledR,LOW);
    digitalWrite(ledV,HIGH);
    digitalWrite(ledB,LOW);
  }
} //fin de la fonction afficheAlarme

/*
 * fonction startDHCP
 * redémarre le réseau pour obtenir une adresse
 */
 void startDHCP()
 {
    Serial.println("Démarrage du DHCP...");
    if (Ethernet.begin(mac) == 0)
    {
      Serial.println("Failed to configure Ethernet using DHCP");
      // try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    }
  delay(1000);
 }
