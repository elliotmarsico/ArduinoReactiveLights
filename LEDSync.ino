#include <FastLED.h>

#define NUM_LEDS 300 //Max of 300
#define MIDPOINT 75
#define DATA_PIN 2

int sensorValue = 0;
int sensorPin = A2; //Input port on the arduino

//Color Palette (Works with pretty much any color you can think of)
CRGBPalette16 Rager_p = CRGBPalette16(
  CRGB::Red, CRGB::Red, CRGB::Purple, CRGB::Purple,
  CRGB::Blue, CRGB::Blue, CRGB::Red, CRGB::Purple,
  CRGB::Blue, CRGB::Red, CRGB::Red, CRGB::Purple,
  CRGB::Purple, CRGB::Blue, CRGB::Blue, CRGB::Red
  );

CRGBPalette16 BlackOut_p = CRGBPalette16(
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
  );

CRGBPalette16 Orange_p = CRGBPalette16(
  CRGB::Orange, CRGB::Orange, CRGB::Orange, CRGB::Orange, 
  CRGB::Orange, CRGB::Orange, CRGB::Orange, CRGB::Orange, 
  CRGB::Orange, CRGB::Orange, CRGB::Orange, CRGB::Orange, 
  CRGB::Orange, CRGB::Orange, CRGB::Orange, CRGB::Orange 
  );
 
CRGBPalette16 BlueTones_p = CRGBPalette16(
  CRGB::Purple, CRGB::Purple, CRGB::Purple, CRGB::Purple,
  CRGB::Blue, CRGB::Blue, CRGB::Pink, CRGB::Purple,
  CRGB::Blue, CRGB::Purple, CRGB::Pink, CRGB::Purple,
  CRGB::Purple, CRGB::Blue, CRGB::Blue, CRGB::Pink
  );
 
CRGBPalette16 PinkTones_p = CRGBPalette16(
  CRGB::Purple, CRGB::Purple, CRGB::Purple, CRGB::Purple,
  CRGB::Pink, CRGB::Pink, CRGB::Pink, CRGB::Purple,
  CRGB::Pink, CRGB::Purple, CRGB::Pink, CRGB::Purple,
  CRGB::Purple, CRGB::Pink, CRGB::Pink, CRGB::Pink
  );

CRGB leds[NUM_LEDS];
//RainbowColors_p, HeatColors_p, ForestColors_p, OceanColors_p, LavaColors_p, CloudColors_p, RainbowStripeColors_p, PartyColors_p
CRGBPalette16 currentPalette(OceanColors_p);
//static uint8_t paletteQuantityLength = 128 / 50; //value between 0 and 10, how much of the lights should be lit up at once, 0 being all
//TBlendType blendingType; //tBlendType is a type of value like int/char/uint8_t etc., use to choose LINERBLEND and NOBLEND


//Noise Variables (Lights go from 0-100 brightness)
uint16_t  brightnessScale = 150;
uint16_t  indexScale = 10;

void setup() {
  delay(1000);
  Serial.begin(9600);
  LEDS.addLeds<WS2812,DATA_PIN,GRB>(leds,NUM_LEDS);
  //fill_solid(leds, NUM_LEDS, CRGB::Blue); //Makes all NUM_LEDS one CRGB color
  //blendingType = LINEARBLEND;
  //fill_palette(leds, NUM_LEDS, startPoint, paletteQuantityLength, currentPalette, 10, blendingType);
  //fill_gradient_RGB(leds, NUM_LEDS, CRGB::Red, CRGB::Purple, CRGB::Blue, CRGB::Purple);
  LEDS.setBrightness(50);
  FastLED.show(); //Updates the LED lights with any changes that were made (necessary any time theres a change)
}

int SensitivityConst = 25; //Higher leads to more sensitive to quieter sounds (Built around a median of 10)
int iter = 0;
int SVTotal = 0;
uint8_t colorIndex = 0;
void loop() {
  leds[iter%NUM_LEDS] = ColorFromPalette( currentPalette , colorIndex, 255, LINEARBLEND);
  iter++;
  colorIndex = colorIndex + 3;
  sensorValue = analogRead(sensorPin); //The value with no noise input is 63, so this just changes that to 0
  SVTotal = SVTotal + abs(sensorValue); //Sometimes the input is negative, so this alters it to positive (might be worth changing but i havent tried)
  if(iter%10 == 0){ //Since the lights read in at 9600 bits/s, I slow it down to 1/10th update speed
    sensorValue = SVTotal/10; //average of the last 10 values
    SVTotal = 0;
    int brightness = 0;
    delay(75); //Slight pause to make it sync with the music (delay is caused by speakers bluetooth while input is instant from aux cord)
    //sensorValue -= 570;
    Serial.println(sensorValue);
    if (sensorValue*SensitivityConst/10 > 200){
      brightness = 100;
    }else if(sensorValue*SensitivityConst/10 > 150){
      brightness = 100;
    }else if(sensorValue*SensitivityConst/10 > 100){
      brightness = 100;
    }else if(sensorValue*SensitivityConst/10 > 75){
      brightness = 30;
    }else if(sensorValue*SensitivityConst/10 > 50){
      brightness = 15;
    }else if(sensorValue*SensitivityConst/10 > 25){
      brightness = 15;
    }else if(sensorValue*SensitivityConst/10 > 10){
      brightness = 5;
    }else{
      brightness = 0;
    }
    FastLED.setBrightness(brightness);
    
    FastLED.show();
  }
}
