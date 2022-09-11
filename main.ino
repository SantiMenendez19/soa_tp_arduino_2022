//REGION PIN
#define PIN_OPEN_SWITCH 3
#define PIN_CLOSE_SWITCH 4
#define PIN_GREEN_LED 5
#define PIN_RED_LED 6
#define PIN_BUTTON 7
#define PIN_ENGINE_OPEN 8
#define PIN_ENGINE_CLOSE 9
#define PIN_ACTIVATE_ENGINE 10
#define PIN_REMOTE_CONTROL_IR 11
//END REGION PIN

//REGION STATE
#define STATE_OPENED 0 
#define STATE_CLOSED 1 
#define STATE_OPENING 2 
#define STATE_CLOSING 3 
#define STATE_STOP_OPENING 4 
#define STATE_STOP_CLOSING 5
//END REGION STATE

//REGION EVENT
#define EVENT_CLOSING_DETECTED 0 
#define EVENT_OPENING_DETECTED 1 
#define EVENT_CHANGED_STATE 2 
#define EVENT_NOT_MODIFIED 3
//END REGION EVENT

//REGION DIRECTION ENGINE
#define DIRECTION_CLOSE_ENGINE -1 
#define DIRECTION_STOP_ENGINE 0 
#define DIRECTION_OPEN_ENGINE 1
//REGION DIRECTION ENGINE

//REGION OTHERS
#define DEFAULT_BAUND_RATE 9600
//END REGION OTHERS

//REGION VARIABLES
int power=0;
int state=0;
int event=0;
//END REGION VARIABLES

void initInputs()
{
  
}
void initOutputs()
{
}
void defineInitState()
{
  readSensors();
  switch(event)
  {
    case EVENT_CLOSING_DETECTED:
    	state=STATE_CLOSED;
    	break;
    case EVENT_OPENING_DETECTED:
    	state= STATE_OPENED;
    	break;
    default:
    	state=STATE_CLOSING;
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