# Quest Name
Authors: Karle Erf, Thuc Nguyen, Alex Salmi

2019-09-20

## Summary



## Evaluation Criteria



## Solution Design
We decided to use global integers to keep track of the current time in 3 variables: hour, minute, and second.
We also had 2 global integer variables to keep track of the hour and minute that the alarm should go off.

We then accessed these global variables from our 5 tasks that were running concurrently. These 5 tasks were:
* Timer: This task incremented the 3 global time variables by using vTaskDelay to increment the second varible once every second. When the second variable hits 60, it resets to 0 and the minute variable increments by 1. Whe the minute variable hits 60, it resets to 0 and the hour variable increments by 1. When the hour variable hits 25, it resets to 1. (to make it a 24 hour clock)
* I2C: This task used the global hour and minute variables to index into an ASCII table to send the correct sequence of bits to each section of the 14-segment display, to display the current time.
* ServoMinute and servoSecond: These two tasks used the minute and second global variables respectively to set the servo to the correct angle. The range of each servo was set from 500 to 2500 so that it would rotate the full 180 degrees, and the max number of increments was set to 60, so that each servo would increment 60 times through the 180 degrees.
* Alarm: This task compared the global hour and minute variables with the global alarmHour and alarmMinute variables to see when they are all equal. When they are, 'ALARM' will be printed repeatedly to the command line.

Workflow: 
Inputs: hour, minute, alarmHour, alarmMinute

Outputs: Displaying the correct time in hours, minutes, and seconds using the 14-segment display and servos.


## Sketches and Photos
<center><img src="./images/example.png" width="70%" /></center>  
<center> </center>


## Supporting Artifacts
- [Link to repo](https://github.com/BU-EC444/Team17-Erf-Nguyen-Salmi)
- [Link to video demo](https://drive.google.com/file/d/14P7rOFaSkSDeDMd17vgasdkblE2QvSs_/view?usp=sharing)


## References

-----

## Reminders

- Video recording in landscape not to exceed 90s
- Each team member appears in video
- Make sure video permission is set accessible to the instructors
- Repo is private
