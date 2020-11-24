////////////////////////////////////////////////////////////
//
// Source file   : Remote_Roomba.ino
// Author        : Malthe B. Nielsen
// Date          : 05/10/2020
// Version       : 6.3
// 
// Description   :
// This is the main file for the RR project, which is a project
// To make wifi connect a non wifi roomba
////////////////////////////////////////////////////////////
// Imports

// Constants
const size_t READ_BUF_SIZE = 64;

// Forward declarations
void uart_receive();     // UART Receiver.
void uart_send(int);     // UART send
void process_received(); // Process the received data from UART

// handler for received publish message
void received_publish(const char *event, const char *data);
void publish_message(); // Message publisher

//Global variables
char readBuf[READ_BUF_SIZE];
size_t readBufOffset = 0;
int receivedValue;
int waiting;

void setup()
{
  // Serial for USB Debugging
  Serial.begin(9600);

  // Serial1 RX is connected to Arduino TX (1)
  // Serial2 TX is connected to Arduino RX (0)
  // Photon GND is connected to Arduino GND
  Serial1.begin(9600);

  //Subscribe to event
  Particle.subscribe("Remote_start", received_publish, ALL_DEVICES);

  waiting = 1;
}

void loop()
{
  Serial.printlnf("Waiting for message from Arduino\n");
  // Wait for respons from Arduino
  while (waiting == 1)
  {
    uart_receive();
  }
  Serial.printlnf("Message recieved from Arduino\n");
  //Publish recieved message from Arduino
  publish_message();
  waiting = 1;
}

// UART Receiver.
void uart_receive()
{
  delay(1000);
  // Read data from serial
  while (Serial1.available())
  {
    if (readBufOffset < READ_BUF_SIZE)
    {
      char c = Serial1.read();
      if (c != '\n')
      {
        // Add character to buffer
        readBuf[readBufOffset++] = c;
      }
      else
      {
        // End of line character found, process line
        readBuf[readBufOffset] = 0;
        process_received();
        readBufOffset = 0;
        waiting = 0;
      }
    }
    else
    {
      Serial.println("readBuf overflow, emptying buffer\n");
      readBufOffset = 0;
    }
  }
}

// UART send
void uart_send(int data)
{
  Serial.printlnf("Send Data; %d\n", data);
  // Send data over UART (Serial1) to Arduino.
  Serial1.printlnf("%d", data);
}

// handler for received publish message
void received_publish(const char *event, const char *data)
{
  Serial.printlnf("Catched published message from IFTTT\n");
  //Start Roomba
  uart_send(1);
}

/*  Message publisher
*   Send '1' - Roomba was there and was started
*   Send '2' - Roomba was there but weren't started
*   Send '3' - Roomba wasn't there.
*   Send '4' - Error
*   Particle.publish("Roomba_return","Roomba",PUBLIC);
*/
void publish_message()
{
  int x = receivedValue;
  Serial.printlnf("Recieved %d from Arduino", x);

  switch (x)
  {
  case 1:
    Particle.publish("Roomba", "Roomba deployed.", PUBLIC);
    break;

  case 2:
    Particle.publish("Roomba", "Roomba present but not deployed.", PUBLIC);
    break;

  case 3:
    Particle.publish("Roomba", "Roomba was not present.", PUBLIC);
    break;

  case 4:
    Particle.publish("Roomba", "ERROR: Something went wrong!", PUBLIC);
    break;

  default:
    break;
  }
}

//Process the received data from UART
void process_received()
{
  receivedValue = atoi(readBuf);
}
