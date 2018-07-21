#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


#define PIN_OUT 6
#define LUM 5 //luminosité de 0 a 255
#define NBRLEDS 16


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
  cadran[0].bleu = 255;
  afficherCadran(cadran);
  delay(500);
  cadran[2].vert = 255;
  afficherCadran(cadran);
  delay(500);
  toutEteindre();
  delay(500);

}


void afficherCadran(struct led *cadran) //prend en entrée l'adresse du tableau cadran
{
  for (int i = NBRLEDS; i >= 0; i--)
  {

    if (cadran[i].rouge > 255)
      cadran[i].rouge = 255;

    if (cadran[i].vert > 255) // vérifications
      cadran[i].vert = 255;

    if (cadran[i].bleu > 255)
      cadran[i].bleu = 255;


    strip.setPixelColor(i, strip.Color(cadran[i].rouge, cadran[i].vert, cadran[i].bleu));
  }
  strip.show();
}




void toutEteindre()
{
  memset(cadran, 0, sizeof(struct led) * NBRLEDS);
  afficherCadran(cadran);
}










//
//void placerAiguille(enum t_aiguille aiguille, int position)
//{
//  (position > 0) ? (position %= 60) : (position = (position % -60) + 60);
//
//  int positionEntiere = (int) (position / 3.75);
//  float positionDecimale = position / 3.75 - positionEntiere;
//
//
//
//  strip.show();
//
//
//}


