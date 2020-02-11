# Code Readme

All of the code required to flash the crawler functionality can be found in the /build and /main folder. The main C code for the functionality can be found in the pcnt_example_main.c file in the /main folder.
Simply navigate into this current directory (/code), run the idf.py build command, and then idf.py -p [PORT] flash, and the esp hould be ready to go. 

All of the code for the nodejs server can be found in the /node folder. Navigate into that folder, run npm install, then node server.js to get the server up and running. 
