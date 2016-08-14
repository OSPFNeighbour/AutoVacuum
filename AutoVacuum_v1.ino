#include <IRremote.h> // use the library
#include <virtuabotixRTC.h> 


// Creation of the Real Time Clock Object
virtuabotixRTC myRTC(5, 6, 7); 

const int buttonPin = 9; // Button pin. Pulled up. Triggered when connected to ground.
const int buttonLED = 10;
const int onboardLED = 13;


int runToday = 0;

//Time that it will trigger
const int hour = 13;    //1pm
const int minutes = 15; //single 0 for on the hr
//

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
long previousClockSeconds = 0;
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

unsigned int autoCommand[68] = {4500, 4400, 550, 1650, 550, 550, 550, 550, 600, 550, 550, 550, 550, 550, 550, 550, 550, 1650, 550, 1650, 550, 550, 550, 550, 550, 550, 600, 550, 550, 550, 550, 550, 550, 1650, 550, 550, 550, 1650, 550, 550, 550, 550, 550, 550, 600, 550, 550, 550, 550, 550, 550, 1650, 550, 550, 550, 1650, 550, 1650, 550, 1650, 600, 1650, 550, 1650, 550, 1650, 550};



IRsend irsend;


void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonLED, OUTPUT);
  pinMode(onboardLED, OUTPUT);

  digitalWrite(buttonLED, HIGH);
  Serial.println("Ready..");

  myRTC.updateTime();
  Serial.print("Current Date / Time: ");                                                                 //| 
  Serial.print(myRTC.dayofmonth);                                                                        //| 
  Serial.print("/");                                                                                     //| 
  Serial.print(myRTC.month);                                                                             //| 
  Serial.print("/");                                                                                     //| 
  Serial.print(myRTC.year);                                                                              //| 
  Serial.print("  ");                                                                                    //| 
  Serial.print(myRTC.hours);                                                                             //| 
  Serial.print(":");                                                                                     //| 
  Serial.print(myRTC.minutes);                                                                           //| 
  Serial.print(":");                                                                                     //| 
  Serial.println(myRTC.seconds);       

}

void loop() {
  myRTC.updateTime();


  if (digitalRead(buttonPin) == LOW)
  {
    Serial.println("Sending");
    // @param1 Raw data
    // @param2 length
    // @param3 frequency, (most devices use 38khz)
    irsend.sendRaw(autoCommand,68,38);
  }

  if ((myRTC.hours == hour) & (myRTC.minutes == minutes))
  {
    if (runToday == 0)
    {
      Serial.println("Sending");
      runToday = 1; //flag so we only run once a day 
      // @param1 Raw data
      // @param2 length
      // @param3 frequency, (most devices use 38khz)
      irsend.sendRaw(autoCommand,68,38);
    }
  } 
  else
    if (runToday == 1)
    {
      Serial.println("Resetting run clock");
      runToday = 0; //flag so we only run once a day
    }

  unsigned long currentMillis = millis();

  if((currentMillis - previousMillis > interval) & (previousClockSeconds != myRTC.seconds))  {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    previousClockSeconds = myRTC.seconds;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(onboardLED, ledState);

  }   



}


