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

int delayval = 500; // delay for half a second
const int buttonPin = 7;
int lastButtonState = 0;
int actButtonState = 0;

char colors[] = 
{5, 0, 0,
 0, 5, 0,
 0, 0, 2,
 5, 5, 2 };

int brightness = 1;
int second = 0;
int minute = 0;
int hour = 0;

enum State
{
  STATE_NORMAL = 0,
  STATE_SET_MINUTE = 1,
  STATE_SET_HOUR = 2
} current_state = STATE_NORMAL;


uint32_t pattern[NUMPIXELS_RING+NUMPIXELS_STICK];

void printState()
{
  switch(current_state)
  {
    case STATE_NORMAL:
      Serial.println("State is NORMAL");
      break;
    case STATE_SET_MINUTE:
      Serial.println("State is SET MINUTE");
      break;
    case STATE_SET_HOUR:
      Serial.println("State is SET HOUR");
      break;
    default:
      Serial.println("Unknown state, call the police!");
      break;
  }
}

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

void animate_hour()
{
  pattern[hour] += 
    pixels.Color(brightness*colors[6], brightness*colors[7], brightness*colors[8]);  
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

void update_brightness()
{
  int poti = analogRead(A5);
  // convert from 72 -- 1100 measurement range to 1-51 brightness multiplier range
  // but make sure that brightness value is at least 1 
  brightness = poti>72?map(poti, 72, 1100, 1,51):1; 
}

void set_minute()
{
  int poti = analogRead(A0);
  // convert from 72 -- 1100 measurement range to 1-51 brightness multiplier range
  // but make sure that brightness value is at least 1 
  minute = poti>72?map(poti, 72, 1100, 0,59):0; 
}

void set_hour()
{
  int poti = analogRead(A0);
  hour = poti>72?map(poti, 72, 1100, 0,11):0; 
}

void update_time()
{
  // TIME MOVES FORWARD HERE
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
}

void setup() 
{
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() 
{
  update_brightness();
  reset_pattern();
  animate_seconds();
  animate_minute();
  animate_hour();                                         
  send_pattern();
  pixels.show(); // This sends the updated pixel color to the hardware.

  actButtonState = digitalRead(buttonPin);

  if(lastButtonState == 0 && actButtonState != 0)
  {
      Serial.println("Button pressed");
      current_state = (State)(((int)current_state+1)%3);
      printState();
  }
  
  switch(current_state)
  {
    case STATE_NORMAL:
      update_time();
      break;
    case STATE_SET_MINUTE:
      set_minute();
      break;
    case STATE_SET_HOUR:
      set_hour();
      break;
  }


  // Print current time on serial
  Serial.print("Current time is: ");
  Serial.print(hour, DEC); Serial.print(":"); 
  Serial.print(minute, DEC); Serial.print(":");
  Serial.println(second, DEC);

  lastButtonState = actButtonState;
  delay(delayval);
}
