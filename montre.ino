#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <DS3231.h>
#include <Wire.h>


#define PIN_OUT 6
#define LUM 2 //luminosité de 0 a 255
#define NBRLEDS 16

#define HEURE 8
#define MINUTE 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN_OUT, NEO_GRB + NEO_KHZ800);
DS3231 horloge;

uint32_t off = strip.Color(0, 0, 0);
bool h12;
bool PM;

struct led //trois valeurs de couleurs, de 0 a 255
{
  char vert;
  char rouge;
  char bleu;
};



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
}

void loop()
{
  //                                           Test de 0 a 59
  //  for (int i = 0; i < 60; i++)
  //  {
  //    int i16 = soixanteVersSeize(i);
  //    Serial.print("Allumage de la led ");
  //    Serial.print(i16);
  //    Serial.print(" minute ");
  //    Serial.println(i);
  //
  //    cadran[i16].vert = 255;
  //    afficherCadran(cadran);
  //    delay(200);
  //  }


  Serial.print(horloge.getHour(h12, PM), DEC);
  Serial.print(" heures ");
  Serial.print(horloge.getMinute(), DEC);
  Serial.print(" minutes et ");
  Serial.print(horloge.getSecond(), DEC);
  Serial.println(" secondes. ");



  //                                          Affichage de l'heure
  cadran[soixanteVersSeize((int) HEURE * (60.0 / 12.0) )].bleu = 255;
  cadran[soixanteVersSeize(MINUTE)].vert = 255;
  afficherCadran(cadran);
  delay(700);

  Serial.println("Extinction de l'anneau");
  toutEteindre();
  delay(300);

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


