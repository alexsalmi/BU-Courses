#  Skill 12 - RTOS

Author: Alex Salmi, 2019-09-20

## Summary
To complete this skill I created a global variable to keep track of the current counting direction and 3 tasks that run simultaneously:
* A interrupt handler task that switches the direction of the global variable whenever the button is pressed
* A counting tasks that will either count up or down between 0 and 15, based on the global variable, and display the current number in the count using the LEDs
* A task to display the current counting direction to the 14-segment display


## Sketches and Photos
Example Video:

[![Example Video](https://img.youtube.com/vi/dPJCmxhj848/0.jpg)](https://www.youtube.com/watch?v=dPJCmxhj848)


## Modules, Tools, Source Used in Solution
I used my programs from the past 5 skills as a template for this program

## Supporting Artifacts


-----

## Reminders
- Repo is private
