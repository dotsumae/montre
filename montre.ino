#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <DS3231.h>
#include <Wire.h>
#include <time.h>

#define PIN_OUT 6
#define LUM 100      //luminosité de 0 a 255
#define NBRLEDS 16
#define SETTIME 0 //mise a l'heure de l'horloge. Uploader avec 1 puis 0 pour maj le RTC.


Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN_OUT, NEO_GRB + NEO_KHZ800);
DS3231 horloge;

bool h12; //recupération de paramètres de l'horloge
bool PM;
bool Century;

struct led //trois valeurs de couleurs, de 0 a 255
{
  int rouge;
  int vert;
  int bleu;
};

uint32_t off = strip.Color(0, 0, 0);

struct led cadran[NBRLEDS] = {0}; //La LED 0 est midi, le tableau les représente dans le sens des aiguilles


void setup()
{

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
  struct led fullVert;
  fullVert.rouge = 0;
  fullVert.bleu = 0;
  fullVert.vert = 255;

  struct led fullBleu;
  fullBleu.rouge = 0;
  fullBleu.bleu = 255;
  fullBleu.vert = 0;

  int heures = (int) horloge.getHour(h12, PM); //recuperation de l'heure
  int minutes = (int) horloge.getMinute();
  int secondes = (int) horloge.getSecond();


  cadran[soixanteVersSeize((int) (heures % 12) * (60.0 / 12.0) )].bleu = 255;
  cadran[soixanteVersSeize(minutes)].vert = 255; //mise a 255 des deux dernières led

  Serial.println("Avant remplissage : ");
  printCadran(cadran);

  Serial.println("Après remplissage : ");
  printCadran(cadran);

 remplirCadran(cadran, fullBleu);
  remplirCadran(cadran, fullVert);

  delay(5000);
  toutEteindre();

}














void printCadran(struct led *cadran)
{
  Serial.println("Etat du cadran : ");
  for (int i = 0; i < NBRLEDS; i++)
  {
    Serial.print("  R : ");
    Serial.print(cadran[i].rouge);
    Serial.print("  G : ");
    Serial.print(cadran[i].vert);
    Serial.print("  B : ");
    Serial.println(cadran[i].bleu);
  }
}



void afficherCadran(struct led *cadran) //prend en entrée l'adresse du tableau cadran
{
  for (int i = 0; i < NBRLEDS; i++)
  {

    if (cadran[i].rouge > 255)
      cadran[i].rouge = 255;

    if (cadran[i].vert > 255) // vérifications
      cadran[i].vert = 255;

    if (cadran[i].bleu > 255)
      cadran[i].bleu = 255;


    strip.setPixelColor(15 - i, strip.Color(cadran[i].rouge, cadran[i].vert, cadran[i].bleu));


  }
  strip.show();
}

void remplirCadran(struct led *cadran, struct led couleur) //remplit le tableau de 255 jusqu'à la première led de la led (couleur) donnée
{
  int i = 0;

  while ((cadran[i].bleu != couleur.bleu && cadran[i].vert != couleur.vert && cadran[i].rouge != couleur.rouge) && i != NBRLEDS)//cadran[i].rouge != couleur.rouge && cadran[i].vert != couleur.vert && cadran[i].bleu != couleur.bleu && i != NBRLEDS)
  {
    Serial.print("Led ");
    Serial.print(i);
    Serial.println(" mise à jour.");
    cadran[i].rouge += couleur.rouge;
    cadran[i].vert += couleur.vert;
    cadran[i].bleu += couleur.bleu;
    i++;
    delay(200);
    afficherCadran(cadran);
  }
}


void toutEteindre()
{
  memset(cadran, 0, sizeof(struct led) * NBRLEDS);
  afficherCadran(cadran);
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




