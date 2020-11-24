# I5IOT - Remote Roomba
Project for IOT class, 5. semester 2020 by Malthe B. Nielsen

# Meeting the requirements
Sensor             - Ultra sonic sensor  
Actuator           - Servo motor  
Web API            - IFTTT  
Technical Platform - Photon & Arduino  

Datasheets and other documentation is uploaded to GitHub in the folder Documentation.

# About the Project  
A device to make a non Wireless Roomba, Wireless and make it possible to start the Roomba over the Internet  
The Project will use IFTTT as the web API, to start the Roomba if it is present at the charging station

## Interface analysis
For connection between user and System-To-Be, the IFTTT service is going to be used. Here a "Applet" with the button feature is going to be used, so when the user interact with the button on their smartphone, IFTTT will send a message / publish a message on the particle platform, which the Photon can subscribe to. When a message is published the Photon will react to that, and begin procedure.  
  
  
![IFTTT_communication](https://user-images.githubusercontent.com/54708397/95649891-54683680-0ae0-11eb-9a0c-62cb9440f765.png)
  
  
Because the Photon doesn't support 5V output on GPIO, and extra platform is needed. In this case Arduino have been choosen, because it supports 5V on output, on most of their units. That means, that the Arduino also can power the Photon, so only a single external power source is needed. To communicate with with the Photon, UART data communication have been choosen.  
![E5IOT - Interface Analysis](https://user-images.githubusercontent.com/54708397/95627436-8647b180-0a7c-11eb-9beb-b1848b5f4bd1.png)  
  

With User interaction and Hardware done a sequence diagram be made  
![Sequence Diagram of Remote Roomba](https://user-images.githubusercontent.com/54708397/95651421-9a29fc80-0aea-11eb-8c5f-9aca3cae6c9c.png)


## Status  
| Date      | Update |
| ----------- | ----------- |
| 05/10/2020 | Analysis on components have been done, and Interface Analysis have been made |
| 05/10/2020 | Message from IFTTT can now be catched by Photon       |
| 08/10/2020   | Test program to measure distance works |
| 08/10/2020   | Test program to activate servo motor works, and can turn from 0-180 degrees |
| 09/10/2020   | Interface Analysis have been completed, and Uart have been choosen over SPI |
| 10/10/2020   | Sequence Diagram have been made        |
| 29/10/2020   | Worked on UART communication between Photon and Arduino, new code will be updated soon        |
| 24/11/2020   | Uploaded finished implementation of Photon and Ardunino part        |
| Date   | Update        |

## Author
Malthe B. Nielsen
