/* Arduino de la lune */

int bzz = 5; // patte n°5 pour le buzzer

// Fréquence des notes
int DO = 262;
int DOd = 277;
int RE = 294;
int REd = 311;
int MI = 330;
int FA = 349;
int FAd = 370;
int SOL = 392;
int SOLd = 415;
int LA = 440;
int LAd = 466;
int SI = 494;



void setup()
{
  
}

void loop()
{
  playTone (SOL, 1,1);
  playTone (SOL, 1,1);
  playTone (SOL, 1,1);
  playTone (LA, 1,1);
  playTone (SI, 3,1);
  playTone (LA, 3,3);
  playTone (SOL, 1,1);
  playTone (SI, 1,1);
  playTone (LA, 1,1);
  playTone (LA, 1,1);
  playTone (SOL, 3,5);


}

void playTone (int freq, int duree, int delai)
{
    tone (bzz, freq, duree*300);
    delay (duree*300+delai*100);
}
