#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <DS3231.h>
#include <Wire.h>
#include <time.h>

#define PIN_OUT 6
#define PIN_BOUTON 2
#define LUM 10       //luminosité de 0 a 255
#define NBRLEDS 16
#define SETTIME 0 //mise a l'heure de l'horloge. Compiler deux fois avec 1 puis 0 pour maj le RTC.
#define DELAIREMPLISSAGE 30

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN_OUT, NEO_GRB + NEO_KHZ800);
DS3231 horloge;

bool h12; //recupération de paramètres de l'horloge
bool PM;
bool Century;


struct posAiguilles
{
  byte posHeures;
  byte posMinutes;
};


int off = strip.Color(0, 0, 0);
uint32_t rouge = strip.Color(255, 0, 0);
uint32_t vert = strip.Color(0, 255, 0);
uint32_t bleu = strip.Color(0, 0, 255);


uint32_t etatCadran[NBRLEDS] = {0}; //Tableau de strip.Color(). La LED 0 est midi, le tableau les représente dans le sens des aiguilles
struct posAiguilles aiguilles = {0};

void setup()
{
  pinMode(PIN_BOUTON, INPUT);
  Serial.begin(9600);
  strip.begin();
  Wire.begin();
  toutEteindre();
  strip.setBrightness(LUM);
  delay(100);
  strip.show();

  if (SETTIME)
  {
    char *cTime = __TIME__;
    char *cDate = __DATE__;

    Serial.println(cDate);
    horloge.setMonth(moisVersNombre(strsep(&cDate, " ")));
    horloge.setDate(atoi(strsep(&cDate, " ")));
    horloge.setYear(atoi(strsep(&cDate, " ")) - 2000);

    horloge.setClockMode(true); // Heure
    horloge.setHour(atoi(strsep(&cTime, ":")));
    horloge.setMinute(atoi(strsep(&cTime, ":")));
    horloge.setSecond(atoi(cTime));

    free(cTime);
    free(cDate);

  }
}

void loop()
{


  int heures = (int) horloge.getHour(h12, PM); //recuperation de l'heure
  int minutes = (int) horloge.getMinute();
  int secondes = (int) horloge.getSecond();


  aiguilles.posHeures = soixanteVersSeize((int) (heures % 12) * (60.0 / 12.0) ); //placement des aiguilles sur le cadran
  aiguilles.posMinutes = soixanteVersSeize(minutes);

  //  Serial.print("Position des aiguilles : ");
  //  Serial.print(aiguilles.posHeures);
  //  Serial.print("  ");
  //  Serial.println(aiguilles.posMinutes);


  delay(1000);



  while (!digitalRead(PIN_BOUTON) == HIGH) //bouton presse
  {
    ;
  }

  etatCadran[2] = 0xFFFFFF;
  etatCadran[1] = 0x00FF00;
  
  afficherCadran();
  printCadran();

  delay(1000);
  toutEteindre();

}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
void printCadran()
{
  Serial.println("Etat du cadran : ");
  for (int i = 0; i < NBRLEDS; i++)
  {
    Serial.print("  R : ");
    Serial.print(etatCadran[i] & rouge, HEX);
    Serial.print("  G : ");
    Serial.print(etatCadran[i] & vert, HEX);
    Serial.print("  B : ");
    Serial.println(etatCadran[i] & bleu, HEX);
  }
}



void afficherCadran() //met a jour le neoring avec etatCadran, verifie au passage que rien ne déborde
{
  for (int i = 0; i < NBRLEDS; i++)
  {
    uint32_t composanteRouge = etatCadran[i] & rouge; //masquage avec FF 00 00 (rouge)
    uint32_t composanteVert = etatCadran[i] & vert; //masquage avec 00 FF 00 (vert)
    uint32_t composanteBleu = etatCadran[i] & bleu; //masquage avec 00 00 FF (bleu)


    if (composanteRouge > 0xFF0000)
      composanteRouge = 0xFF0000;
      
    if (composanteVert > 0x00FF00)
      composanteVert = 0x00FF00;
      
    if (composanteBleu > 0x0000FF)
      composanteBleu = 0x0000FF;


    strip.setPixelColor(15 - i, composanteRouge + composanteVert + composanteBleu);


  }
  strip.show();
}
//
//void remplirCadran(struct led *cadran, struct posAiguilles positionArret, struct led couleurHeures, struct led couleurMinutes) //remplit le tableau de 255 jusqu'à la première led de la led (couleur) donnée
//{
//  int i = 0;
//
//  while (i <= positionArret.posHeures)
//  {
//    cadran[i].rouge += couleurHeures.rouge;
//    cadran[i].vert += couleurHeures.vert;
//    cadran[i].bleu += couleurHeures.bleu;
//    afficherCadran(cadran);
//
//    i++;
//    delay(DELAIREMPLISSAGE);
//  }
//  i = 0;
//  while (i <= positionArret.posMinutes)
//  {
//    cadran[i].rouge += couleurMinutes.rouge;
//    cadran[i].vert += couleurMinutes.vert;
//    cadran[i].bleu += couleurMinutes.bleu;
//    afficherCadran(cadran);
//
//    i++;
//    delay(DELAIREMPLISSAGE);
//  }
//}
//
void toutEteindre()
{
  memset(etatCadran, 0, sizeof(uint32_t) * NBRLEDS);
  afficherCadran();
}




int soixanteVersSeize(int position)
{
  //  (position > 0) ? (position %= 60) : (position = (position % -60) + 60);

  int positionEntiere = (int) (position / 3.75); //indice de la led a allumer
  float positionDecimale = position / 3.75 - positionEntiere; //proximité entre 0 et 1- de la led suivante

  return positionEntiere;
}

int moisVersNombre(char *mois)
{
  byte Month;

  if (!strcmp(mois, "Jan"))
  {
    Month = 1;
  }

  else if (!strcmp(mois, "Fev"))
  {
    Month = 2;
  }

  else if (!strcmp(mois, "Mar"))
  {
    Month = 3;
  }
  else if (!strcmp(mois, "Apr"))
  {
    Month = 4;
  }

  else if (!strcmp(mois, "May"))
  {
    Month = 5;
  }

  else if (!strcmp(mois, "Jun"))
  {
    Month = 6;
  }

  else if (!strcmp(mois, "Jul"))
  {
    Month = 7;
  }

  else if (!strcmp(mois, "Aug"))
  {
    Month = 8;
  }

  else if (!strcmp(mois, "Sep"))
  {
    Month = 9;
  }

  else if (!strcmp(mois, "Oct"))
  {
    Month = 10;
  }

  else if (!strcmp(mois, "Nov"))
  {
    Month = 11;
  }

  else if (!strcmp(mois, "Dec"))
  {
    Month = 12;
  }

  else
  {
    Month = 0; //erreur
    Serial.println("Erreur dans la lecture du mois !");
  }

  return Month;

}




