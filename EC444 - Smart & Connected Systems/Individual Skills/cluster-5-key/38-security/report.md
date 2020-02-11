#  Skill 38: Security

Author: Alex Salmi, 2019-11-22

## Summary
1. The main weakness with our Quest 3 solution was that we didn't inclde any kind of authentication, which would allow anybody to get into our server, if they have the IP. The other weakness was that we didn't encrypt our data in any way whatsoever. 

2. Ways that a bad guy can attack our system: 

    (a) They could access our server if they have the IP address.

    (b) They could intercept our data on communication, since none of it is encrypted

    (c) They could send false data if they intercept our data and send their own to the server

3. Some simple ways for us to mitigate these issues are to add some sort of authentication, so they can't access our server as easily. We could also even add some basic form of encryption to make it that much harder for the bad guys to intercept our data.

4. The example project in the esp-idf repo called "provisioning" provides some examples of different types of provisioning for the ESP32. This includes BLE provisioning, console provisioning, SoftAP provisioning, and SoftAP custom provisioning. 

## Sketches and Photos


## Modules, Tools, Source Used in Solution
severaal example projects in the esp-idf repo

whizzer.bu.edu

## Supporting Artifacts


-----

## Reminders
- Repo is private