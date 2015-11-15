const int buttonPin = 7;     // the number of the pushbutton pin

// variables will change:
int lastButtonState = 0;
int actButtonState = 0;

enum State
{
  STATE_NORMAL = 0,
  STATE_SET_MINUTE = 1,
  STATE_SET_HOUR = 2
} current_state = STATE_NORMAL;

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

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
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
      //Serial.println("State is NORMAL");
      break;
    case STATE_SET_MINUTE:
      //Serial.println("State is SET MINUTE");
      break;
    case STATE_SET_HOUR:
      //Serial.println("State is SET HOUR");
      break;
  }

  lastButtonState = actButtonState;
  delay(50);
}
