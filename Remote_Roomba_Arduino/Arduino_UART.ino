////////////////////////////////////////////////////////////
//
// Source file   : Arduino_UART.ino
// Author        : Malthe B. Nielsen
// Date          : 05/10/2020
// Version       : 3.1
// 
// Description   :
// This is the main file for the RR project, which is a project
// To make wifi connect a non wifi roomba
////////////////////////////////////////////////////////////

//Includes
#include <Servo.h>

// Defines
#define echoPin 22 // attach pin D22 Arduino to pin Echo of HC-SR04
#define trigPin 23 //attach pin D23 Arduino to pin Trig of HC-SR04
#define servoPin 52  //attach pin D52 Arduino to pin

  // Constants
const size_t READ_BUF_SIZE = 64;

  // Forward declarations
void process_received();   // Process the received data from UART
int check_distance();      // Checks if the Roomba is present
void start_robot(int);     // Starts the Roomba if it's present (Press twice)
void uart_receive();       // UART Receiver.
void uart_send(int);       // UART send
//void ISR();              // Interrupt handler to wake arduino up from sleep 

// General variables
int distance; // variable to store return value from Check_Distance
int WaitOnStart; // Wait variable for runthrough of if else statements

// Variables for Serial communication
int wait; // Waiting to recieve on UART
int receivedValue;  // Recieved value on UART in integer
char read_buf[READ_BUF_SIZE]; // Recieved value on UART in Char
size_t readBufOffset = 0; // Buffer offset

// Variables for Range detector
long duration; // variable for the duration of sound wave travel
int measureddistance; // variable for the distance measurement
  
//Servo
Servo servo;  // Servo object
int angle = 0;  // base Angle

// Setup function
void setup() {
  // Serial TX (1) is connected to Photon RX
  // Serial RX (0) is connected to Photon TX
  // Ardiuno GND is connected to Photon GND
  Serial.begin(9600);
  // General variables
  wait = 1;
  distance = 0;
  WaitOnStart = 1;

  //Range detector pin setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  //Servo setup
  servo.attach(servoPin); // Servo pin setup
  servo.write(angle); // Set base angle for servo
  
}

void loop() {
  // Waits for new message / command from Photon
  while (wait == 1) {
    uart_receive(); // Calls uart_receive
  }
  // Checks if the Roomba is present returns 1 or 0
  distance = check_distance();

  // Run if the Roomba is present
  if (distance)
  {
    // Try and start the Roomba with 2 presses
    while (WaitOnStart == 1)
    {
      start_robot(2); // Starts the Roomba
    }
    // Sets variable back to 1 after start_robot was called
    WaitOnStart = 1;
    
    // Wait 10 seconds for the Roomba to move
    delay(10000);
    // Checks if the Roomba is still present
    distance = check_distance();

    // Run if the Roomba started & Send message 1.
    if (distance == 0)  uart_send(1);

    // If the Roomba is still present
    else if (distance == 1)
    {
      // Try and start the Roomba with 1 press
      while (WaitOnStart == 1)
      {
        start_robot(1);
      }
      // Sets variable back to 1 after start_robot was called
      WaitOnStart = 1;
      
      // Wait 10 seconds for the Roomba to move
      delay(10000);
      // Checks if the Roomba is still present
      distance = check_distance();

      // Run if the Roomba started & Send message 1.
      if (distance == 0) uart_send(1);

      // Run if the Roomba didn't start & Send message 2.
      else if (distance == 1) uart_send(2);

      // Run if an error was returned & Send message 4.
      else uart_send(4);
    }
    // Run if an error was returned & Send message 4.
    else uart_send(4);

  }
  // Run if the Roomba is not present.
  else if (check_distance() == 0) uart_send(3);

  // Run if an error was returned & Send message 4.
  else uart_send(4);

  // resets variables
  wait = 1;
  receivedValue = 0;
  measureddistance = 0;
  // Waits 5 seconds
  delay(5000);
}


// Checks if the Roomba is present, If Roomba is present, return 1 else 0.
int check_distance() {
  // resets variable to 0 
  measureddistance = 0;
  
  // Runs through check 10 times
  for (int x = 0; x < 10; x++) {
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    // Waits 2 microseconds 
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE)
    digitalWrite(trigPin, HIGH);
    // Waits 10 microseconds as mentioned in datasheet of the sensor
    delayMicroseconds(10);
    // Sets the trigPin LOW (INACTIVE)
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance from the formula givin in datasheet of the sensor
    measureddistance += duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    // Wait 0.1 seconds
    delay(100);
  }
  // Finds the average of the 10 measurements
  measureddistance = measureddistance / 10;
  
  // Checks if the roomba was present from the average distance
  // If the measureddistance is between 0 and 4 cm it is
  if(0< measureddistance && measureddistance <4)
  {
    // Yes
    return 1;
  }
  // If above 4cm it is not
  else if (measureddistance >4)
  {
    // No
    return 0;
  }
  // Error in case of sensor fault
  else
    return -1;
}



// Starts the Roomba if it's present (Press twice)
void start_robot(int presses) {

  // Run for amount of requested presses
  for (int x = 0; x < presses; x++)
  {
    servo.write(180); // Move Servo to angle 180
    delay(1000);      // Wait 1 sec
    servo.write(0); // Move Servo to angle 0
    delay(4000);      // Wait 4 sec
  }
  // Set WaitOnStart to 0 to get out of while loop in loop()
  WaitOnStart = 0;
}


/*  UART Receiver.
*   Recieved '1' - Start Roomba
*/
void uart_receive() {
  //wait 1 sec
  delay(1000);
  // Read data from serial when it is ready
  while (Serial.available()) {
    // Check if readBufOffset have exceed buffer size
    if (readBufOffset < READ_BUF_SIZE) {
      //If it haven't exceeded the buffer size
      //Read the serial port
      char c = Serial.read();
      //Check if newline was recieved
      if (c != '\n') {
        // Add character to buffer if it wasn't a newline
        read_buf[readBufOffset++] = c;
      }
      // If the char recieved was a newline 
      else {
        // End of line character found, process line
        read_buf[readBufOffset] = 0;
        // Call process_received function
        process_received();
        // Reset buffer counter
        readBufOffset = 0;
        // Set waiting to false / 0
        wait = 0;
      }
    }
    // If readBufOffset have exceed buffer size
    else {
      // Reset buffer counter
      readBufOffset = 0;
    }
  }
}



/*  UART send
*   Send '1' - Roomba was there and was started
*   Send '2' - Roomba was there but weren't started
*   Send '3' - Roomba wasn't there.
*   Send '4' - Error
*/
void uart_send(int msg) {
  // Switch statement to determine what 
  // value to return to the Photon
  switch (msg)
  {
  case 1:
    // send '1' + newline
    Serial.print(msg, DEC);
    Serial.print('\n');
    break;

  case 2:
    // Send '2' + newline
    Serial.print(msg, DEC);
    Serial.print('\n');
    break;

  case 3:
    // Send '3' + newline
    Serial.print(msg, DEC);
    Serial.print('\n');
    break;

  case 4:
    // Send '4' + newline
    Serial.print(msg, DEC);
    Serial.print('\n');
    break;

  default:
    break;

  }
}


//Process the received data from UART
void process_received() {
  // Convert received char to integer
  receivedValue = atoi(read_buf);
}
