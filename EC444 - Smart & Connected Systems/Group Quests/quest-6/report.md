# Roll Up
Authors: Karle Erf, Thuc Nguyen, Alex Salmi

2019-12-10

## Summary
For this quest, we integrated skills from our previous quests in order to modify the crawler. We added autonomous steering, 
a webcam live streaming from our raspberry pi to a node server, IR beacon reading, and ultrasonic 
distance sensing, along with the other skills included with the initial crawler quest. With all of this functionality we
plan to complete the course outlined in the quest brief. 


## Evaluation Criteria



## Solution Design
Pi USB Camera

Autonomous Steering

Node Server - Our node server and front end, hosted on the raspberry pi, is the control center for the crawler. The front end website has a video stream of the RPI webcam mounted on the crawler, which it accesses through mjpg webcam streaming. The website also includes a list of all our time split entries in the tingoDB database we set up, that should automatically update when the crawler reaches a new beacon. It does not actually update the list in our final product, since we are no longer able to connect our ESP32 to our router to communicate to the ESP. If we were able to connect, we have everything set up where the server would recieve the new time splits through UDP packets. The website also features controls for remote steering, but once again, since we cannot connect the ESP to the router, we cannot send the steering commands through UDP packets to the crawler to control it. All data flow between the server and the website is through socket.io.

MicroLidar Steering - We chose to use the microLidars as our side sensors, to allow the crawler to drive straight along a wall. We initially planned to have two microLidars on the left side of our crawler, so we could compare the distances between the two, making sure they are about the same, and if not, steer the crawler left or right to correct the discrepancy. However, part way through implementing this design, the wires to one of the microLidars broke, so we had to revert to a threshold strategy with only one microLidar. Basically, since we knew we had to be approximately 75 cm frm the wall, we simply checked if the microLidar read more or less than 75 cm, and steering slighty left or right respectively to correct itself. This ended up working very well for us, with only one collision on the mirrored wall during our demo.

Ultrasonic Distance

IR Beacon Read


## Sketches and Photos
<center><img src="./images/example.png" width="70%" /></center>  
<center> </center>


## Supporting Artifacts
- [Link to repo]()
- [Link to video demo]()


## References

-----

## Reminders

- Video recording in landscape not to exceed 90s
- Each team member appears in video
- Make sure video permission is set accessible to the instructors
- Repo is private
