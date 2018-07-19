#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN_OUT 6
#define LUM 5 //luminosité
#define NBRLEDS 16


Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN_OUT, NEO_GRB + NEO_KHZ800);

uint32_t off = strip.Color(0, 0, 0);

struct led
{
  char rouge;
  char vert;
  char bleu;
};

struct led cadran[NBRLEDS];


void setup() {
  Serial.begin(9600);
  strip.begin();
  toutEteindre();
  strip.setBrightness(LUM);
  delay(100);
  strip.show();

}

void loop() {
  
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

void toutEteindre()
{
  for (int i = 0; i < NBRLEDS; i++)
  {
    strip.setPixelColor(i, off);
  }
}






