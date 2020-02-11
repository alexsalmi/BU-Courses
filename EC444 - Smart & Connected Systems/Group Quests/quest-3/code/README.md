# Code Readme

All of the code required to flash the sensor/adc functionality can be found in the /build and /main folder. The main C code for the functionality can be found in the udp_client.c file in the /main folder.
Simply navigate into this current directory (/code), run the idf.py build command, and then idf.py -p [PORT] flash, and the esp hould be ready to go. 

All of the code for the nodejs server can be found in the /node folder. Navigate into that folder, then node server.js to get the server up and running. 
