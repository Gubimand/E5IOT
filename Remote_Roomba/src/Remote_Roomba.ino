////////////////////////////////////////////////////////////
//
// Source file   : Remote_Roomba.ino
// Author        : Malthe B. Nielsen
// Date          : 05/10/2020
// Version       : 0.1
// 
// Description   :
// This is the main file for the RR project, which is a project
// To make wifi connect a non wifi roomba
////////////////////////////////////////////////////////////

int LED = D7;
int i = 0;

// Handling all the events incoming to particle
void myHandler(const char *event, const char *data)
{
  i++;
  Log.info("%d: event=%s data=%s", i, event, (data ? data : "NULL"));
  digitalWrite(LED, HIGH);
  delay(1s);
}


// setup() runs once, when the device is first turned on.
void setup() {
  Particle.subscribe("Remote_start", myHandler, ALL_DEVICES);
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  digitalWrite(LED, LOW);
  delay(1s);

}