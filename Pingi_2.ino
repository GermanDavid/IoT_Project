#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define NUMBER_OF_SAMPLES 24
Adafruit_NeoPixel weekly(7, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel second(46, 4, NEO_GRB + NEO_KHZ800);

double averageSamples [NUMBER_OF_SAMPLES];
int countNumberOfMeasures [NUMBER_OF_SAMPLES];

void initArrays(){
  for (int i = 0; i < NUMBER_OF_SAMPLES; i++){
    averageSamples[i] = 0;
    countNumberOfMeasures[i] = 0;
  }
}

void colorWholeStripWithGivenColor(Adafruit_NeoPixel ledStrip , uint32_t color){
  for(int i = 0 ; i < ledStrip.numPixels() ; i++){
    ledStrip.setPixelColor(i, color);
  }
  ledStrip.show();
}


void setup() {
  Serial.begin(9600);
  weekly.begin();
  second.begin();
}
