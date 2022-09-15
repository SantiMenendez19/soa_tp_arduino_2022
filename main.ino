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
#define PIN_POWER_METER 12

// END REGION PIN

// REGION STATE
#define STATE_INITIAL 0
#define STATE_OPENED 1
#define STATE_CLOSED 2
#define STATE_OPENING 3
#define STATE_CLOSING 4
#define STATE_STOP_OPENING 5
#define STATE_STOP_CLOSING 6
// END REGION STATE

// REGION EVENT
#define EVENT_AUTOMATIC_ACTION_CLOSE 1
#define EVENT_AUTOMATIC_ACTION_OPEN 2
#define EVENT_MANUAL_ACTION 3
#define EVENT_NOT_MODIFIED 4
// END REGION EVENT

// REGION DIRECTION ENGINE
#define DIRECTION_CLOSE_ENGINE -1
#define DIRECTION_STOP_ENGINE 0
#define DIRECTION_OPEN_ENGINE 1
// REGION DIRECTION ENGINE

// REGION OTHERS
#define DEFAULT_BAUND_RATE 9600
#define ARRAY_LENGTH 2; 
// END REGION OTHERS

// REGION VARIABLES
int power = 0;
int state = STATE_INITIAL;
int event = 0;
int previusEvent = LOW;
int button = LOW;
int finalOpen = LOW;
int finalClosed = HIGH;
//INDICES DE ARRAYS DE SENSORES DE FIN DE CARRERA 
int indexSwitchSensors[ARRAY_LENGTH]={0,1};
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

//INTERCAMBIA EL ORDEN DE LOS INDICES DE ARRAY DE SENSORES DE FIN DE CARRERA 
void reverseIndex()
{
	int aux = indexSwitchSensors[0];
    indexSwitchSensors[0] = indexSwitchSensors[1];
    indexSwitchSensors[1] = aux;
}

//DEFINE EVENTE SEGUN SENSORES DE FINALE DE CARRERA PERO ALTERNA EL ORDEN EN QUE LOS LEE EN CADA LOOP 
int alternateSwitchEvent()
{
    reverseIndex();
    int sensorResult[ARRAY_LENGTH] ={finalClosed,finalOpen};
    int sensorEvent[ARRAY_LENGTH = {EVENT_AUTOMATIC_ACTION_CLOSE,EVENT_AUTOMATIC_ACTION_OPEN};
    for(int i = 0; i<ARRAY_LENGTH; i++)
    {
        int index = indexSwitchSensors[i];
        if(sensorResult[index] == HIGH)
        {
           return sensorEvent[index];     
        }
    }
    return LOW;
}

void defineEvent()
{
    int oldManualAction = button;
    readSensors();
    //TODO: ver cuando este terminada la funcion de leer sensores
    if(button == HIGH && oldManualAction==LOW){
        event = EVENT_MANUAL_ACTION;
        return;
    }
    int sensorEvent = alternateSwitchEvent();
    if(sensorEvent > LOW)
    {
        event = sensorEvent;
        return;
    }
    event = EVENT_NOT_MODIFIED;
}

void turnOnLight(int pinLed) {
  	digitalWrite(pinLed, HIGH);
}

void turnOffLight(int pinLed) {
	digitalWrite(pinLed, LOW);
}

void moveEngine(int direction)
{

}

int readRemote()
{
  int value = LOW;
  if (IrReceiver.decode())
  {
    Serial.println(IrReceiver.decodedIRData.command);
    value = HIGH;
    IrReceiver.resume();
  }
  return value;
}

void readSensors() {
	  // Pulsador
  	//int buttonVal = digitalRead(PIN_BUTTON);
    //button = buttonVal;
    // Control Remoto
    int remoteVal = readRemote();
    button = remoteVal;
  	// Potenciometro
  	int powerMeterVal = analogRead(PIN_POWER_METER);
  	power = powerMeterVal / 4;
    // Fin de carrera cierre
    int closedSwitchVal = digitalRead(PIN_CLOSED_SWITCH);
    finalOpen = closedSwitchVal;
    // Fin de carrera apertura
    int openSwitchVal = digitalRead(PIN_OPEN_SWITCH);
    finalClosed = openSwitchVal;
}
void logAction(char message[])
{
    if(event != previusEvent)
    {
        Serial.println(message);
    }

}

void stateMachine()
{
    previusEvent = event;
    defineEvent();
    switch (state)
    {
        case STATE_INITIAL:
        switch(event)
            {
                case EVENT_AUTOMATIC_ACTION_CLOSE:
                    turnOffLight(PIN_RED_LED);
                    turnOnLight(PIN_GREEN_LED);
                    moveEngine(DIRECTION_STOP_ENGINE);
                    state = STATE_CLOSED;
                    logAction("Closed");
                    break;
                case EVENT_AUTOMATIC_ACTION_OPEN:
                    turnOffLight(PIN_GREEN_LED);
                    turnOnLight(PIN_RED_LED);
                    moveEngine(DIRECTION_STOP_ENGINE);
                    state = STATE_OPENED;
                    logAction("Opened");
                    break;
                case EVENT_MANUAL_ACTION:
                    moveEngine(DIRECTION_CLOSE_ENGINE);
                    turnOnLight(PIN_RED_LED);
                    turnOffLight(PIN_GREEN_LED);
                    logAction("Closing...");
                    state = STATE_CLOSING;
                    break;
                case EVENT_NOT_MODIFIED:
                    turnOffLight(PIN_GREEN_LED);
                    turnOnLight(PIN_RED_LED);
                    moveEngine(DIRECTION_STOP_ENGINE);
                    logAction("Stop Opening");
                    state = STATE_STOP_OPENING;
                    break;
                default:
                    logAction("Invalid Event");
                    break;
            }
            break;
        case STATE_CLOSED:
        switch (event)
        {
            case EVENT_AUTOMATIC_ACTION_CLOSE:
                // NOTHING TODO
                break;
            case EVENT_AUTOMATIC_ACTION_OPEN:
                logAction("Check Open sensor failure");
                break;
            case EVENT_MANUAL_ACTION:
                moveEngine(DIRECTION_OPEN_ENGINE);
                turnOnLight(PIN_RED_LED);
                turnOffLight(PIN_GREEN_LED);
                logAction("Opening..");
                state = STATE_OPENING;
                break;
            case EVENT_NOT_MODIFIED:
                logAction("Check Close sensor failure");
                break;
            default:
                logAction("Invalid Event");
                break;
        }
            break;
    case STATE_OPENED:
        switch (event)
        {
            case EVENT_AUTOMATIC_ACTION_CLOSE:
                logAction("Check Close sensor failure");
                break;
            case EVENT_AUTOMATIC_ACTION_OPEN:
                // NOTHING TODO
                break;
            case EVENT_MANUAL_ACTION:
                moveEngine(DIRECTION_CLOSE_ENGINE);
                logAction("Closing..");
                state = STATE_CLOSING;
                break;
            case EVENT_NOT_MODIFIED:
                logAction("Check Open sensor failure");
                break;
            default:
                logAction("Invalid Event");
                break;
        }
            break;
    case STATE_OPENING:
        switch (event)
        {
            case EVENT_AUTOMATIC_ACTION_CLOSE:
                logAction("Check Close sensor failure");
                break;
            case EVENT_AUTOMATIC_ACTION_OPEN:
                moveEngine(DIRECTION_STOP_ENGINE);
                logAction("Open");
                state = STATE_OPENED;
                break;
            case EVENT_MANUAL_ACTION:
                moveEngine(DIRECTION_STOP_ENGINE);
                logAction("Stop Opening");
                state = STATE_STOP_OPENING;
                break;
            case EVENT_NOT_MODIFIED:
                moveEngine(DIRECTION_OPEN_ENGINE);
                break;
            default:
                logAction("Invalid Event");
                break;
        }
            break;
        case STATE_STOP_OPENING:
        switch (event)
        {
            case EVENT_AUTOMATIC_ACTION_CLOSE:
            logAction("Check Close sensor failure");
            break;
            case EVENT_AUTOMATIC_ACTION_OPEN:
            logAction("Check Open sensor failure");
            break;
            case EVENT_MANUAL_ACTION:
            moveEngine(DIRECTION_CLOSE_ENGINE);
            logAction("Closing..");
            state = STATE_CLOSING;
            break;
            case EVENT_NOT_MODIFIED:
            // NOTHING TODO
            break;
        default:
            logAction("Invalid Event");
            break;
        }
            break;
    case STATE_CLOSING:
        switch (event)
        {
        case EVENT_AUTOMATIC_ACTION_CLOSE:
            moveEngine(DIRECTION_STOP_ENGINE);
            turnOnLight(PIN_GREEN_LED);
            turnOffLight(PIN_RED_LED);
            logAction("Close");
            state = STATE_CLOSED;
            break;
        case EVENT_AUTOMATIC_ACTION_OPEN:
            logAction("Check Open sensor failure");
            break;
        case EVENT_MANUAL_ACTION:
            moveEngine(DIRECTION_STOP_ENGINE);
            logAction("Stop Closing");
            state = STATE_STOP_CLOSING;
            break;
        case EVENT_NOT_MODIFIED:
            moveEngine(DIRECTION_CLOSE_ENGINE);
            break;
        default:
            logAction("Invalid Event");
            break;
        }
            break;
    case STATE_STOP_CLOSING:
        switch (event)
        {
            case EVENT_AUTOMATIC_ACTION_CLOSE:
                logAction("Check Close sensor failure");
                break;
            case EVENT_AUTOMATIC_ACTION_OPEN:
                logAction("Check Open sensor failure");
                break;
            case EVENT_MANUAL_ACTION:
                moveEngine(DIRECTION_OPEN_ENGINE);
                logAction("Opening..");
                state = STATE_OPENING;
                break;
            case EVENT_NOT_MODIFIED:
                // NOTHING TODO
                break;
            default:
                logAction("Invalid Event");
                break;
            }
            break;
        default:
            logAction("Invalid State");
            break;
    }
}

void setup()
{
  Serial.begin(DEFAULT_BAUND_RATE);
  initInputs();
  initOutputs();
}

void loop()
{
    stateMachine();
}