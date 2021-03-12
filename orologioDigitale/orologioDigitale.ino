/*
  Pin +5V           -> Alimentazione modulo RTC
  Pin GND           -> GND modulo RTC
  Pin Digital 2     -> Pin CE - RST modulo RTC
  Pin Digital 3     -> Pin I/O - DAT modulo RTC
  Pin Digital 4     -> Pin CLCK - CLK modulo RTC
*/

#include <DS1302.h>
#include <Adafruit_NeoPixel.h>

DS1302 rtc(2, 3, 4);

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 6
#define LED_COUNT 102
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t colore = strip.Color(255, 255, 255);

const short arrayColori[24][3] = {{18, 19, 41},     // 00:xx
                                  {29, 23, 55},     // 01:xx
                                  {39, 37, 85},     // 02:xx
                                  {42, 46, 106},    // 03:xx
                                  {1, 87, 94},      // 04:xx
                                  {0, 104, 134},    // 05:xx
                                  {0, 146, 161},    // 06:xx
                                  {56, 255, 75},    // 07:xx
                                  {110, 255, 81},   // 08:xx
                                  {189, 195, 41},   // 09:xx
                                  {221, 220, 31},   // 10:xx
                                  {255, 255, 0},    // 11:xx
                                  {243, 174, 0},    // 12:xx
                                  {239, 121, 0},    // 13:xx
                                  {244, 89, 0},     // 14:xx
                                  {231, 56, 39},    // 15:xx
                                  {209, 49, 39},    // 16:xx
                                  {222, 28, 20},    // 17:xx
                                  {227, 6, 19},     // 18:xx
                                  {192, 22, 52},    // 19:xx
                                  {129, 22, 52},    // 20:xx
                                  {103, 27, 85},    // 21:xx
                                  {70, 51, 138},    // 22:xx
                                  {40, 46, 101}};   // 23:xx
const short int pallini[2] = {50, 51};
const short int offsetCifre[4] =  {0, 25, 52, 77};
const short int moduli[7][4] = {{0, 1, 2, 3},
                                {4, 5, 6, -1},
                                {7, 8, 9, 10},
                                {11, 12, 13, -1},
                                {14, 15, 16, 17},
                                {18, 19, 20, -1},
                                {21, 22, 23, 24}};
const short int numeri[10][7] = {{1, 2, 3, 5, 6, 7, -1},      //0
                                 {3, 7, -1, -1, -1, -1, -1},  //1
                                 {1, 2, 4, 6, 7, -1, -1},     //2
                                 {2, 3, 4, 6, 7, -1, -1},     //3
                                 {3, 4, 5, 7, -1, -1, -1},    //4
                                 {2, 3, 4, 5, 6, -1, -1},     //5
                                 {1, 2, 3, 4, 5, 6, -1},      //6
                                 {3, 6, 7, -1, -1, -1, -1},   //7
                                 {1, 2, 3, 4, 5, 6, 7},       //8
                                 {2, 3, 4, 5, 6, 7, -1}};     //9

String oraCorrente = "";
int short cifraUno = 0;
int short cifraDue = 0;
int short cifraTre = 0;
int short cifraQuattro = 0;
int short uno[7];
int short due[7];
int short tre[7];
int short quattro[7];
boolean statusPallini;

void orologio() {
  strip.clear();
  
  for(int i=0; i<7; i++) {
    uno[i] = numeri[cifraUno][i];
  }
  // CIFRA DUE
  for(int i=0; i<7; i++) {
    due[i] = numeri[cifraDue][i];
  }
  // CIFRA TRE
  for(int i=0; i<7; i++) {
    tre[i] = numeri[cifraTre][i];
  }
  // CIFRA QUATTRO
  for(int i=0; i<7; i++) {
    quattro[i] = numeri[cifraQuattro][i];
  }

  for(int i=0; i<7; i++) {
    if(uno[i] != -1) {
      for(int j=0; j<4; j++) {
        if(moduli[uno[i]-1][j] != -1) {
          coloraCifra(moduli[uno[i]-1][j]);
        }
      }
    }
  }
  
  for(int i=0; i<7; i++) {
    if(due[i] != -1) {
      for(int j=0; j<4; j++) {
        if(moduli[due[i]-1][j] != -1) {
          coloraCifra(moduli[due[i]-1][j] + offsetCifre[1]);
        }
      }
    }
  }
  
  for(int i=0; i<7; i++) {
    if(tre[i] != -1) {
      for(int j=0; j<4; j++) {
        if(moduli[tre[i]-1][j] != -1) {
          coloraCifra(moduli[tre[i]-1][j] + offsetCifre[2]);
        }
      }
    }
  }

  for(int i=0; i<7; i++) {
    if(quattro[i] != -1) {
      for(int j=0; j<4; j++){
        if(moduli[quattro[i]-1][j] != -1) {
          coloraCifra(moduli[quattro[i]-1][j] + offsetCifre[3]);
        }
      }
    }
  }
  strip.show();
}

void blinkPallini() {
  if(statusPallini) {
    statusPallini = !statusPallini;
    strip.setPixelColor(pallini[0], 0, 0, 0);
    strip.setPixelColor(pallini[1], 0, 0, 0);
  }
  else {
    statusPallini = !statusPallini;
    strip.setPixelColor(pallini[0], colore);
    strip.setPixelColor(pallini[1], colore);
  }
  strip.show();
}

void setup()
{
  //Serial.begin(9600);
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();            // Inizializzazione Neopixel
  strip.clear();            // Spegne tutti i led
  strip.show();             // Spegne tutti i led
  strip.setBrightness(255); // Luminosità (0 - 255)
}

void loop()
{
  oraCorrente = rtc.getTimeStr();
  cifraUno = oraCorrente[0] - 48;
  cifraDue = oraCorrente[1] - 48;
  cifraTre = oraCorrente[3] - 48;
  cifraQuattro = oraCorrente[4] - 48;

  orologio();
  blinkPallini();
  cambioColore((String(cifraUno) + String(cifraDue)).toInt());
  delay(1000);
}

void cambioColore(int ora) {
  colore = strip.Color(arrayColori[ora][0], arrayColori[ora][1], arrayColori[ora][2]);
}

void coloraCifra(int led) {
  strip.setPixelColor(led, colore);
}
