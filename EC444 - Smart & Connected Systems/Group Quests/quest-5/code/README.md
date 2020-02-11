# Code Readme

All of the code required to flash the fob functionality can be found in the /fobs/code/build and /fobs/code/main folder. The main C code for the functionality can be found in the uart_async_rxtxtasks_main.c file in the /fobs/code/main folder.
Simply navigate into the fobs directory (/code/fobs/), run the idf.py build command, and then idf.py -p [PORT] flash, and the esp hould be ready to go. 

All of the code required to flash the hub functionality can be found in the /hub/build and /hub/main folder. The main C code for the functionality can be found in the udp_client.c file in the /hub/main folder.
Simply navigate into the hub directory (/code/hub/), run the idf.py build command, and then idf.py -p [PORT] flash, and the esp hould be ready to go. 

All of the code for the nodejs server can be found in the /node folder. Navigate into that folder, install all required modules, then node server.js to get the server up and running. 