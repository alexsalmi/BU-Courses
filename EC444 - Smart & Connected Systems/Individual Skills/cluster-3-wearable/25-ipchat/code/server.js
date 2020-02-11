// Required module
var dgram = require('dgram');

// Port and IP
var PORT = 8080;
var HOST = '192.168.1.49';

// Create socket
var server = dgram.createSocket('udp4');

// Create server
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// On connection, print out received message
server.on('message', function (message, remote) {
    console.log('From ' + remote.address + ':' + remote.port +' - " ' + message + '"');

    // Send Ok acknowledgement
    server.send("Recieved",remote.port,remote.address,function(error){
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
