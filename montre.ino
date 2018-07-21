#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


#define PIN_OUT 6
#define LUM 2 //luminosité de 0 a 255
#define NBRLEDS 16

#define HEURE 2   //exemples, a remplacer par les vraies heures
#define MINUTE 17


Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN_OUT, NEO_GRB + NEO_KHZ800);

uint32_t off = strip.Color(0, 0, 0);

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

         //                                    Test sur 0, 15, 30, 45
  for (int j = 0; j < 4; j++)
  {
    int i = 15*j;
    int i16 = soixanteVersSeize(i); 
    Serial.print("Allumage de la led ");
    Serial.print(i16);
    Serial.print(" minute ");
    Serial.println(i);

    cadran[i16].vert = 255;
    afficherCadran(cadran);
    delay(2000);
  }

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


