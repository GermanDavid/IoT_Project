#include <EEPROM.h>

/*
 * // day is 144 samples (every 10 minute we sample)
 * We will manage the EEPROM with 3 blocks of memory:
 *  - 0-143(including): mapped byte value of consumption average.
 *  - 144-287(including): times we sampled each time at day (in order to calc average)
 *  - 288-431(including): multiplayer of times we sample the data (in order to have more than 255 days we sampled the data)
*/

#define LOW_CONSUMPTION 0
#define HIGH_CONSUMPTION 1000
#define DAYILY_SAMPLES 144

/**
 * This function clears all the EEPROM memory
 * In addition, this function put 1 on each cell on the the third block of memory 
 */
void initEEPROM(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

  // change third block to 1's:
  int startIndex = DAYILY_SAMPLES * 2;
  int endIndex = DAYILY_SAMPLES * 3;

  for (int i = startIndex ; i < endIndex ; i++){
    EEPROM.write(i, 1);
  }
}

/**
 * This function maps a double sample to byte sample
 */
byte mapDoubleSampleToByte(double givenSampleValue){
  byte mappedSampleValue = map(givenSampleValue , LOW_CONSUMPTION , HIGH_CONSUMPTION , 0 ,255);
  Serial.println(mappedSampleValue);
  return mappedSampleValue;
}

/**
 * This function maps a byte sample to double sample
 */
double mapByteSampleToDouble(byte givenSampleValue){
  double mappedSampleValue = map(givenSampleValue , 0 , 255 , LOW_CONSUMPTION , HIGH_CONSUMPTION);
  return mappedSampleValue;
}

/*
 * This function updates the eeprom value at given index to correct average consmpution on given time
 */
void updateAverageConsumtionAtGivenIndex(int index , double givenConsumption){

   // get cuurent consumption (in bytes)
   byte currentByteAtIndex;
   EEPROM.get(index,currentByteAtIndex);

   // DEBUG: (can be deleted)
   Serial.println("at adrres:");
   Serial.println(currentByteAtIndex);

   // convert byte stored at EEPROM to its double value
   double mappedCurrentConsmption = mapByteSampleToDouble(currentByteAtIndex);
    
   // DEBUG: (can be deleted)
   Serial.println("mapped value:");
   Serial.println(mappedCurrentConsmption);
   
   // get current number of samples (in order to calculate average):
   byte secondBlockSamples;
   EEPROM.get(index + DAYILY_SAMPLES , secondBlockSamples);
   byte multiOfDays;
   EEPROM.get(index + DAYILY_SAMPLES + DAYILY_SAMPLES  , multiOfDays);

   // calculate total number of samples:
   int totalNumberOfSamples = secondBlockSamples * multiOfDays;

   // calculate new consumption:
   double newConsumption = mappedCurrentConsmption * totalNumberOfSamples + givenConsumption;

   // update second block:
   if (secondBlockSamples == 254){
    secondBlockSamples = 0;
    multiOfDays += 1;
   }
   else{
    secondBlockSamples += 1;
   }
   
   // divide new consumption by number of samples
   double newAverage = newConsumption / totalNumberOfSamples;

   // map it to bytes:
   byte bytedNewAverageConsumption = mapDoubleSampleToByte(newAverage);

   // update eeproms:
   EEPROM.update(index , bytedNewAverageConsumption);
   EEPROM.update(index + DAILY_SAMPLES , secondBlockSamples);
   EEPROM.update(index + DAILY_SAMPLES + DAILY_SAMPLES , multiOfDays);
}

void setup() {
  Serial.begin(9600);
  updateAverageConsumtionAtGivenIndex(13);
}

void loop() {
  // put your main code here, to run repeatedly:

}
