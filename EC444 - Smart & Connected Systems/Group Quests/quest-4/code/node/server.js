// QUEST 4: Autonomous Crawler
// TEAM 17: Karle Erf, Thuc Nguyen, Alex Salmi

// Main code for our nodejs server. The server waits for input in the console using readline, and updates the 'on' variable when the user inputs 't'
// Server will then send the current value of 'on' to the crawler through UDP responses every second, and the crawler will turn on/off based on this variable. 

// Required module
var dgram = require('dgram');
const readline = require('readline');

// Port and IP
var PORT = 8080;
var HOST = '192.168.1.49';

// Create socket
var server = dgram.createSocket('udp4');

var on = "f";

readline.emitKeypressEvents(process.stdin);
process.stdin.setRawMode(true);

process.stdin.on('keypress', (key, data) => {
  if (data.ctrl && data.name === 't') {
    process.exit();
  } else if(data.name == 't'){
    if(on == "f"){
      on = "t";
    }
    else{
      on = "f";
    }
  }
});
console.log('Press a key');

// Create server
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// On connection, print out received message
server.on('message', function (message, remote) {
    console.log('From ' + remote.address + ':' + remote.port +' - " ' + message + '"');

    // Send Ok acknowledgement
    server.send(on,remote.port,remote.address,function(error){
      if(error){
        console.log('Could not send response');
      }
      else{
        console.log('Sent reponse');
      }
    });

});

// Bind server to port and IP
server.bind(PORT, HOST);