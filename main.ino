// REGION LIBRARY
#include <IRremote.h> //sensor IR
// END REGION LIBRARY

// REGION PIN
#define PIN_OPEN_SWITCH 3
#define PIN_CLOSE_SWITCH 4
#define PIN_GREEN_LED 5
#define PIN_RED_LED 6
#define PIN_BUTTON 7
#define PIN_ENGINE_OPEN 8
#define PIN_ENGINE_CLOSE 9
#define PIN_ACTIVATE_ENGINE 10
#define PIN_REMOTE_CONTROL_IR 11
// END REGION PIN

// REGION STATE
#define STATE_OPENED 0
#define STATE_CLOSED 1
#define STATE_OPENING 2
#define STATE_CLOSING 3
#define STATE_STOP_OPENING 4
#define STATE_STOP_CLOSING 5
// END REGION STATE

// REGION EVENT
#define EVENT_CLOSING_DETECTED 0
#define EVENT_OPENING_DETECTED 1
#define EVENT_CHANGED_STATE 2
#define EVENT_NOT_MODIFIED 3
// END REGION EVENT

// REGION DIRECTION ENGINE
#define DIRECTION_CLOSE_ENGINE -1
#define DIRECTION_STOP_ENGINE 0
#define DIRECTION_OPEN_ENGINE 1
// REGION DIRECTION ENGINE

// REGION OTHERS
#define DEFAULT_BAUND_RATE 9600
// END REGION OTHERS

// REGION VARIABLES
int power = 0;
int state = 0;
int event = 0;
// END REGION VARIABLES

void initInputs()
{
  pinMode(PIN_OPEN_SWITCH, INPUT);
  pinMode(PIN_CLOSE_SWITCH, INPUT);
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_REMOTE_CONTROL_IR, INPUT);
  IrReceiver.begin(PIN_REMOTE_CONTROL_IR, ENABLE_LED_FEEDBACK);
}
void initOutputs()
{
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(PIN_ENGINE_OPEN, OUTPUT);
  pinMode(PIN_ENGINE_CLOSE, OUTPUT);
  pinMode(PIN_ACTIVATE_ENGINE, OUTPUT);
}
void defineInitState()
{
  readSensors();
  switch (event)
  {
  case EVENT_CLOSING_DETECTED:
    state = STATE_CLOSED;
    break;
  case EVENT_OPENING_DETECTED:
    state = STATE_OPENED;
    break;
  default:
    state = STATE_CLOSING;
    break;
  }
}

void onLed(int pinOn, int pinOff)
{
}

void moveEngine(int direction)
{
}

void readSensors()
{
}

void stateMachine()
{
  readSensors();
  switch (state)
  {
    case STATE_CLOSED:
    switch (event)
    {
      case EVENT_CLOSING_DETECTED:
        // NOTHING TODO
        break;
      case EVENT_OPENING_DETECTED:
        Serial.println("Check Open sensor failure");
        break;
      case EVENT_CHANGED_STATE:
        moveEngine(DIRECTION_OPEN_ENGINE);
        onLed(PIN_RED_LED, PIN_GREEN_LED);
        Serial.println("Opening..");
        state = STATE_OPENING;
        break;
      case EVENT_NOT_MODIFIED:
        Serial.println("Check Close sensor failure");
        break;
      default:
        Serial.println("Invalid Event");
        break;
    }
    break;
  case STATE_OPENED:
    switch (event)
    {
      case EVENT_CLOSING_DETECTED:
        Serial.println("Check Close sensor failure");
        break;
      case EVENT_OPENING_DETECTED:
        // NOTHING TODO
        break;
      case EVENT_CHANGED_STATE:
        moveEngine(DIRECTION_CLOSE_ENGINE);
        Serial.println("Closing..");
        state = STATE_CLOSING;
        break;
      case EVENT_NOT_MODIFIED:
        Serial.println("Check Open sensor failure");
        break;
      default:
        Serial.println("Invalid Event");
        break;
    }
    break;
  case STATE_OPENING:
    switch (event)
    {
      case EVENT_CLOSING_DETECTED:
        Serial.println("Check Close sensor failure");
        break;
      case EVENT_OPENING_DETECTED:
        moveEngine(DIRECTION_STOP_ENGINE);
        Serial.println("Open");
        state = STATE_OPENED;
        break;
      case EVENT_CHANGED_STATE:
        moveEngine(DIRECTION_STOP_ENGINE);
        Serial.println("Stop Opening");
        state = STATE_STOP_OPENING;
        break;
      case EVENT_NOT_MODIFIED:
        moveEngine(DIRECTION_OPEN_ENGINE);
        break;
      default:
        Serial.println("Invalid Event");
        break;
    }
    break;
    case STATE_STOP_OPENING:
      switch (event)
      {
        case EVENT_CLOSING_DETECTED:
          Serial.println("Check Close sensor failure");
          break;
        case EVENT_OPENING_DETECTED:
          Serial.println("Check Open sensor failure");
          break;
        case EVENT_CHANGED_STATE:
          moveEngine(DIRECTION_CLOSE_ENGINE);
          Serial.println("Closing..");
          state = STATE_CLOSING;
          break;
        case EVENT_NOT_MODIFIED:
          // NOTHING TODO
          break;
      default:
        Serial.println("Invalid Event");
        break;
    }
      break;
  case STATE_CLOSING:
    switch (event)
    {
      case EVENT_CLOSING_DETECTED:
        moveEngine(DIRECTION_STOP_ENGINE);
        onLed(PIN_GREEN_LED, PIN_RED_LED);
        Serial.println("Close");
        state = STATE_CLOSED;
        break;
      case EVENT_OPENING_DETECTED:
        Serial.println("Check Open sensor failure");
        break;
      case EVENT_CHANGED_STATE:
        moveEngine(DIRECTION_STOP_ENGINE);
        Serial.println("Stop Closing");
        state = STATE_STOP_CLOSING;
        break;
      case EVENT_NOT_MODIFIED:
        moveEngine(DIRECTION_CLOSE_ENGINE);
        break;
      default:
        Serial.println("Invalid Event");
        break;
    }
    break;
  case STATE_STOP_CLOSING:
    switch (event)
    {
      case EVENT_CLOSING_DETECTED:
        Serial.println("Check Close sensor failure");
        break;
      case EVENT_OPENING_DETECTED:
        Serial.println("Check Open sensor failure");
        break;
      case EVENT_CHANGED_STATE:
        moveEngine(DIRECTION_OPEN_ENGINE);
        Serial.println("Opening..");
        state = STATE_OPENING;
        break;
      case EVENT_NOT_MODIFIED:
        // NOTHING TODO
        break;
      default:
        Serial.println("Invalid Event");
        break;
    }
    break;
  default:
    Serial.println("Invalid State");
    break;
  }
}

void setup()
{
  Serial.begin(DEFAULT_BAUND_RATE);
  initInputs();
  initOutputs();
  defineInitState();
}

void loop()
{
}