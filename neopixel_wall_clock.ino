#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS_RING      12
#define NUMPIXELS_STICK      8

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS_RING + NUMPIXELS_STICK, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 5; // delay for half a second

char colors[] = 
{5, 0, 0,
 0, 5, 0,
 0, 0, 5,
 5, 5, 5 };

int brightness = 1;
int second = 0;
int minute = 0;
int hour = 0;

uint32_t pattern[NUMPIXELS_RING+NUMPIXELS_STICK];

void animate_seconds()
{
  pattern[NUMPIXELS_RING+(second/8)] = 
    pixels.Color(brightness*colors[0], brightness*colors[1], brightness*colors[2]);
}

void animate_minute()
{
  pattern[minute/5] = 
    pixels.Color(brightness*colors[3], brightness*colors[4], brightness*colors[5]);
}

void reset_pattern()
{
  for(int i=0;i<NUMPIXELS_RING+NUMPIXELS_STICK;i++)
    pattern[i] = 0;
}

void send_pattern()
{
  for(int i=0;i<NUMPIXELS_RING+NUMPIXELS_STICK;i++)
    pixels.setPixelColor(i, pattern[i]);
}

void setup() 
{
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() 
{
  reset_pattern();
  animate_seconds();
  animate_minute();                                         
  send_pattern();
  pixels.show(); // This sends the updated pixel color to the hardware.

  int poti = analogRead(A5);
  // convert from 72 -- 1100 measurement range to 1-51 brightness multiplier range
  // but make sure that brightness value is at least 1 
  brightness = poti>72?map(poti, 72, 1100, 1,51):1; 

  ++second;

  if(second > 59)
  {
    ++minute;
    second %= 60;
    Serial.print("SECOND IS 60, minute changed to ");
    Serial.println(minute);
  }

  if(minute == 60)
  {
    ++hour;
    minute %= 60;
  }

  hour %= 12;

//  Serial.println(brightness);

  Serial.print("Current time is: ");
  Serial.print(hour, DEC); Serial.print(":"); 
  Serial.print(minute, DEC); Serial.print(":");
  Serial.println(second, DEC);

  delay(delayval);
}
