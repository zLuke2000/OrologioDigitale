/*
  Pin +5V           -> Alimentazione modulo RTC
  Pin GND           -> GND modulo RTC
  Pin Digital 2     -> Pin CE - RST modulo RTC
  Pin Digital 3     -> Pin I/O - DAT modulo RTC
  Pin Digital 4     -> Pin CLCK - CLK modulo RTC
*/

#include <DS1302.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 6
#define LED_COUNT 102
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

DS1302 rtc(2, 3, 4);

uint32_t colore = strip.Color(255, 0, 0);

const short int pallini[] = {50, 51};
const short  int offsetCifre[] =  {0, 25, 52, 77};
const short  int moduli[7][4] = {{0, 1, 2, 3}, {4, 5, 6, -1}, {7, 8, 9, 10}, {11, 12, 13, -1}, {14, 15, 16, 17}, {18, 19, 20, -1}, {21, 22, 23, 24}};
const short  int numeri[10][7] = {{1, 2, 3, 5, 6, 7, -1}, {3, 7, -1, -1, -1, -1, -1}, {1, 2, 4, 6, 7, -1, -1}, {2, 3, 4, 6, 7, -1, -1}, {3, 4, 5, 7, -1, -1, -1}, {2, 3, 4, 5, 6, -1, -1}, {1, 2, 3, 4, 5, -1, -1}, {3, 6, 7, -1, -1, -1, -1}, {1, 2, 3, 4, 5, 6, 7}, {3, 4, 5, 6, 7, -1, -1}};

String oraCorrente = "";
int short cifraUno = 0;
int short cifraDue = 0;
int short cifraTre = 0;
int short cifraQuattro = 0;
int short uno[7];
int short due[7];
int short tre[7];
int short quattro[7];

void setup()
{
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();            // Inizializzazione Neopixel
  strip.clear();            // Spegne tutti i led
  strip.show();             // Spegne tutti i led
  strip.setBrightness(255); // Luminosità (0 - 255)

  // Imposta RTC in run-mode e disabilita la protezione da scrittura
  rtc.halt(false);
  rtc.writeProtect(false);

  // Le seguenti linee possono essere commentate per utilizzare i valori già memorizzati nel DS1302
  rtc.setDOW(SUNDAY);         // Imposta il giorno della settimana a SUNDAY
  rtc.setTime(18, 32, 0);     // Imposta l'ora (Formato 24hr)
  rtc.setDate(1, 1, 2020);    // Imposta la data

}

void loop()
{
  strip.clear();
  
  oraCorrente = rtc.getTimeStr();
  cifraUno = oraCorrente[0] - 48;
  cifraDue = oraCorrente[1] - 48;
  cifraTre = oraCorrente[3] - 48;
  cifraQuattro = oraCorrente[4] - 48;
    
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
  
  //Controllo ora per impostare il colore dell'orologio
  controlloOra((String(oraCorrente[0]) + String(oraCorrente[1])).toInt());
  
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
  delay(1000);
}

void controlloOra(int ora) {
  switch(ora) {
  case 0:
    colore = strip.Color(18, 19, 41);
    break;
  case 1:
    colore = strip.Color(29, 23, 55);
    break;
  case 2:
    colore = strip.Color(39, 37, 85);
    break;
  case 3:
    colore = strip.Color(42, 46, 106);
    break;
  case 4:
    colore = strip.Color(1, 87, 94);
    break;
  case 5:
    colore = strip.Color(0, 104, 134);
    break;
  case 6:
    colore = strip.Color(0, 146, 161);
    break;
  case 7:
    colore = strip.Color(103, 179, 108);
    break;
  case 8:
    colore = strip.Color(139, 255, 81);
    break;
  case 9:
    colore = strip.Color(18, 19, 41);
    break;
  case 10:
    colore = strip.Color(221, 220, 31);
    break;
  case 11:
    colore = strip.Color(255, 255, 0);
    break;
  case 12:
    colore = strip.Color(243, 174, 0);
    break;
  case 13:
    colore = strip.Color(239, 121, 0);
    break;
  case 14:
    colore = strip.Color(244, 89, 0);
    break;
  case 15:
    colore = strip.Color(231, 59, 39);
    break;
  case 16:
    colore = strip.Color(209, 49, 39);
    break;
  case 17:
    colore = strip.Color(222, 28, 20);
    break;
  case 18:
    colore = strip.Color(227, 6, 19);
    break;
  case 19:
    colore = strip.Color(192, 22, 52);
    break;
  case 20:
    colore = strip.Color(129, 22, 52);
    break;
  case 21:
    colore = strip.Color(103, 27, 85);
    break;
  case 22:
    colore = strip.Color(70, 51, 138);
    break;
  case 23:
    colore = strip.Color(40, 46, 101);
    break;
  default:
    colore = strip.Color(255, 255, 255);
    break;
  }
}

void coloraCifra(int led) {
  strip.setPixelColor(led, colore);
}
