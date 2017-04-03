/* Arduino de la lune + blink */

int bzz = 5; // patte n°5 pour le buzzer
unsigned long present = 0; // temps présent (sans accent)
unsigned long derniereNote = 0; // heure de la dernière note jouée
unsigned long dernierCligno = 0; // heure du dernier clignotement

int ledPin = 13;
int ledState = LOW;

// Fréquence des notes
#define DO      262
#define DOd    277
#define RE      294
#define REd    311
#define MI      330
#define FA      349
#define FAd    370
#define SOL    392
#define SOLd  415
#define LA      440
#define LAd    466
#define SI        494

// Mélodie
static int musique [11] [3] = {
  {SOL, 1,1}, 
  {SOL, 1,1}, 
  {SOL, 1,1}, 
  {LA, 1,1}, 
  {SI, 3,1}, 
  {LA, 2,2}, 
  {SOL, 1,1}, 
  {SI, 1,1}, 
  {LA, 1,1}, 
  {LA, 1,1}, 
  {SOL, 4,4}
};

void setup()
{
  pinMode (ledPin, OUTPUT);
  
}
void loop()
{
  for(int i=0 ; i<11 ; i++)
    {
      playTone(musique[i][0], musique[i][1]); //on joue la note : 1ere colonne=fréquence, 2e colonne=durée
      present = millis();
      derniereNote = present;
     while (present < derniereNote + 300*musique[i][1]+100*musique[i][2])
     // on tourne en rond en attendant un délai qui correspond à la durée de la note plus celle du silence
     {
          present=millis();
          
          // ici on en profite pour faire changer l'état de la led toutes les demi-secondes
          if(present >= dernierCligno + 500)
          {
              if(ledState == LOW)
              {
                  ledState = HIGH;
              }
              else
              {
                  ledState=LOW;
              };
              dernierCligno=present;
              digitalWrite(ledPin,ledState);
          };
        
       };     
    }

}

void playTone (int freq, int duree)
{
    tone (bzz, freq, duree*300);
 }
