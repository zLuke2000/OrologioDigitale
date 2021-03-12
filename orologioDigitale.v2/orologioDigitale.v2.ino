/*
  Pin +5V           -> Alimentazione modulo RTC
  Pin GND           -> GND modulo RTC
  Pin Digital 2     -> Pin CE - RST modulo RTC
  Pin Digital 3     -> Pin I/O - DAT modulo RTC
  Pin Digital 4     -> Pin CLCK - CLK modulo RTC
*/

#include <DS1302.h>
#include <Adafruit_NeoPixel.h>
#include <TimedAction.h>

DS1302 rtc(2, 3, 4);

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 6
#define LED_COUNT 102
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// COLORE INIZIALE
uint32_t colore = strip.Color(255, 0, 0);

const unsigned short arrayColori[24][3] = {{18, 19, 41},     // 00:xx
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
const unsigned short pallini[2] = {50, 51};
const unsigned short offsetCifre[4] =  {0, 25, 52, 77};
const short moduli[7][4] = {{0, 1, 2, 3},
                            {4, 5, 6, -1},
                            {7, 8, 9, 10},
                            {11, 12, 13, -1},
                            {14, 15, 16, 17},
                            {18, 19, 20, -1},
                            {21, 22, 23, 24}};
const short numeri[10][7] = {{1, 2, 3, 5, 6, 7, -1},      //0
                             {3, 7, -1, -1, -1, -1, -1},  //1
                             {1, 2, 4, 6, 7, -1, -1},     //2
                             {2, 3, 4, 6, 7, -1, -1},     //3
                             {3, 4, 5, 7, -1, -1, -1},    //4
                             {2, 3, 4, 5, 6, -1, -1},     //5
                             {1, 2, 3, 4, 5, 6, -1},      //6
                             {3, 6, 7, -1, -1, -1, -1},   //7
                             {1, 2, 3, 4, 5, 6, 7},       //8
                             {2, 3, 4, 5, 6, 7, -1}};     //9

String oraAvvio = "";
String oraCorrente = "";
unsigned short cifraUno = 0;
unsigned short cifraDue = 0;
unsigned short cifraTre = 0;
unsigned short cifraQuattro = 0;
unsigned short uno[7];
unsigned short due[7];
unsigned short tre[7];
unsigned short quattro[7];
boolean statusPallini;

void updateClock() {
    getCurrentTime();

    // CIFRA UNO
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
    
    changeColor((String(cifraUno) + String(cifraDue)).toInt());
    strip.clear();
    // ACCENSIONE CIFRA UNO
    for(int i=0; i<7; i++) {
        if(uno[i] != -1) {
        for(int j=0; j<4; j++) {
            if(moduli[uno[i]-1][j] != -1) {
            updateLed(moduli[uno[i]-1][j]);
            }
        }
        }
    }
    // ACCENSIONE CIFRA DUE
    for(int i=0; i<7; i++) {
        if(due[i] != -1) {
        for(int j=0; j<4; j++) {
            if(moduli[due[i]-1][j] != -1) {
            updateLed(moduli[due[i]-1][j] + offsetCifre[1]);
            }
        }
        }
    }
    // ACCENSIONE CIFRA TRE
    for(int i=0; i<7; i++) {
        if(tre[i] != -1) {
        for(int j=0; j<4; j++) {
            if(moduli[tre[i]-1][j] != -1) {
            updateLed(moduli[tre[i]-1][j] + offsetCifre[2]);
            }
        }
        }
    }
    // ACCENSIONE CIFRA QUATTRO
    for(int i=0; i<7; i++) {
        if(quattro[i] != -1) {
        for(int j=0; j<4; j++){
            if(moduli[quattro[i]-1][j] != -1) {
            updateLed(moduli[quattro[i]-1][j] + offsetCifre[3]);
            }
        }
        }
    }
    strip.show();
}

void blinkDots() {
    if(statusPallini) {
        strip.setPixelColor(pallini[0], 0, 0, 0);
        strip.setPixelColor(pallini[1], 0, 0, 0);
    }
    else {
        strip.setPixelColor(pallini[0], colore);
        strip.setPixelColor(pallini[1], colore);
    }
    statusPallini = !statusPallini;
    strip.show();
}

void getCurrentTime() {
    oraCorrente = rtc.getTimeStr();
    Serial.println(oraCorrente);
    cifraUno = oraCorrente[0] - 48;
    cifraDue = oraCorrente[1] - 48;
    cifraTre = oraCorrente[3] - 48;
    cifraQuattro = oraCorrente[4] - 48;
}

void changeColor(int ora) {
    colore = strip.Color(arrayColori[ora][0], arrayColori[ora][1], arrayColori[ora][2]);
}

void updateLed(int led) {
    strip.setPixelColor(led, colore);
}

TimedAction TAUpdateClock = TimedAction(5000,updateClock);
TimedAction TABlinkDots = TimedAction(1000,blinkDots);

void setup() {
    Serial.begin(9600);
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif

    strip.begin();            // Inizializzazione Neopixel
    strip.clear();            // Spegne tutti i led
    strip.show();             // Spegne tutti i led
    strip.setBrightness(255); // Luminosità (0 - 255)

    updateClock();
    //SINCRONIZZAZIONE
    oraAvvio = rtc.getTimeStr();
    while(oraAvvio == rtc.getTimeStr()) {
    }
    TAUpdateClock.reset();
    TABlinkDots.reset();
}

void loop() {
    TAUpdateClock.check();
    TABlinkDots.check();
}