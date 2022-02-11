#include <Adafruit_CircuitPlayground.h>

#define CAP_THRESHOLD   600
#define DEBOUNCE        200
#define FLASHLIGHT      0
#define TIMER           6
#define TIME_INCREMENT  10000


const int SHORT_PRESS_TIME = 1000; // 1000 milliseconds
const int LONG_PRESS_TIME  = 1000; // 1000 milliseconds

int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
bool isPressing = false;
bool isLongDetected = false;


boolean flashlightToggle = false;

int timerCounter = 0;
boolean timerToggled = false;
boolean timerStarting = false;
long int startTime;

boolean capButton(uint8_t pad) {
  
  Serial.println(CircuitPlayground.readCap(pad));
  if (CircuitPlayground.readCap(pad) > CAP_THRESHOLD) {
    return true;  
  } else {
    return false;
  }
}

void lightAll(int br) {
  
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, br,   br,   br);
    }
}

void updateTimerCountdown(){

    if(timerCounter != 0 && millis() - startTime >= TIME_INCREMENT){

            CircuitPlayground.setPixelColor(timerCounter-1, 0,   0,   0);
            timerCounter--;
            startTime+=TIME_INCREMENT;
          
        }
  
  }

 void resetTimer(){

    timerCounter = 0;
    lightAll(0);
  
  }



void setup() {
  CircuitPlayground.begin();
  pinMode(FLASHLIGHT, INPUT_PULLUP);
  pinMode(TIMER, INPUT_PULLUP);
  
  
}

void loop() {

  //Serial.println(digitalRead(TIMER));
  Serial.println(digitalRead(FLASHLIGHT));
  


  currentState = digitalRead(TIMER);


  if(lastState == LOW && currentState == HIGH) {        // button is pressed
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
  } else if(lastState == HIGH && currentState == LOW) { // button is released
    isPressing = false;
    releasedTime = millis();
  }

 long pressDuration = releasedTime - pressedTime;
  
 if (!digitalRead(TIMER)) {

      if(pressDuration < SHORT_PRESS_TIME){
            //Serial.println("A short press is detected");  
          if(timerCounter == 0){
              startTime = millis();
            }
    
          CircuitPlayground.setPixelColor(timerCounter, 255,   255,   255);
          timerCounter++;
    
          // Print message.
          //Serial.println("Timer Touched!");
        
        }

      if(isPressing == true && isLongDetected == false) {
          long pressDuration = millis() - pressedTime;
      
          if( pressDuration > LONG_PRESS_TIME ) {
            Serial.println("A long press is detected");
            isLongDetected = true;
            resetTimer();
          }
  }
  }
  else if (!digitalRead(FLASHLIGHT)) {

      // Print message.
      Serial.println("Flashlight touched!");

      if(flashlightToggle){
        lightAll(0);
        flashlightToggle = false;
        }
      else{
        lightAll(10);
        flashlightToggle = true;
        }
      
      // But not too often.
      delay(DEBOUNCE); 
  }

  updateTimerCountdown();

   // save the the last state
  lastState = currentState;
  
  //CircuitPlayground.setPixelColor(0, 0,   0,   0);
}
