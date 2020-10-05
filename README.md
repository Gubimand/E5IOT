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
Because the Photon doesn't support 5V output on GPIO, and extra platform is needed. In this case Arduino have been choosen, because it supports 5V on output, on most of their units. That means, that the Arduino also can power the Photon, so only a single external power source is needed. To communicate with with the Photon, SPI data communication have been choosen.  
![E5IOT-Interface_Analysis](https://user-images.githubusercontent.com/54708397/95123092-b9730380-0751-11eb-9ade-470a4a7e4caa.png)


## Status  
| Date      | Update |
| ----------- | ----------- |
| 05/10/2020 | Analysis on components have been done, and Interface Analysis have been made |
| 05/10/2020 | Message from IFTTT can now be catched by Photon       |
| Date   | Update        |

## Author
Malthe B. Nielsen
