#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    8
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

#define LDR A0
#define motor 5 //motor pin is D5
#define ldrThreshold 307

//const float Q=5.0/1023;
int code;

void setup() {
    //delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
 
    pinMode(motor,OUTPUT); //motor pin is output
    Serial.begin(9600);   // baud rate
}


void loop()
{
    
    code = analogRead(LDR); //reads analog input
    Serial.println(code);
    if (code < ldrThreshold){  // light exceeds threshold
      analogWrite(motor,155);
      Serial.println("Motor on");
      
      ChangePalettePeriodically();
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1; /* motion speed */
      FillLEDsFromPaletteColors( startIndex,255);
      FastLED.show();
      FastLED.delay(1000 / UPDATES_PER_SECOND);
      FastLED.setBrightness(  BRIGHTNESS );
      
  } else {
      analogWrite(motor, 255);
      FillLEDsFromPaletteColors( 0,0);
      Serial.println("Motor off");
      FastLED.setBrightness(  0 );
      FastLED.delay(0);
      //digitalWrite(LED_PIN, LOW); //turn off led 
  }
  delay(20);  // delay between samples
}

void FillLEDsFromPaletteColors( uint8_t colorIndex,uint8_t brightness)
{
    //uint8_t brightness = 255;

    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
    }
}
