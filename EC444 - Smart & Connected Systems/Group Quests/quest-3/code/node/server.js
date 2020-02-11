// QUEST 3: Wearable
// TEAM 17: Karle Erf, Thuc Nguyen, Alex Salmi

// Main code for our nodejs server. The data is recieved from UDP, and is then sent to index.html using socket.io. 

// Required module
var dgram = require('dgram');
var app = require('express')();
var http = require('http').createServer(app);
var io = require('socket.io')(http);
var bodyParser = require('body-parser');

// Port and IP
var PORT = 8080;
var HOST = '192.168.1.49';

var findMyWatch = "t";

var toggle = "tsb";

app.use(bodyParser.urlencoded({ extended: true })); 

// Points to index.html to serve webpage
app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
  });

  // User socket connection
io.on('connection', function(socket){
    console.log('a user connected');
    socket.on('disconnect', function(){
      console.log('user disconnected');
    });
  });
  
  // Listening on localhost:3000
  http.listen(3000, function() {
    console.log('listening on *:3000');
  });

app.post('/myaction', function(req, res) {
  findMyWatch = "t";
  // res.sendFile(__dirname + '/index.html');
});

app.post('/toggle', function(req, res) {
  toggle = "";
  if(req.body.toggleTemp){
    toggle += "t";
  }
  if(req.body.toggleSteps){
    toggle += "s";
  }
  if(req.body.toggleBattery){
    toggle += "b";
  }
  res.sendFile(__dirname + '/index.html');
});

  //-----//

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

    message = message.toString();

    if(!toggle.includes("t")){
      var temp = "0|" + message.split("|")[1] + "|" + message.split("|")[2];
      message = temp;
    }
    if(!toggle.includes("s")){
      var temp = message.split("|")[0] + "|N/A|" + message.split("|")[2];
      message = temp;
    }
    if(!toggle.includes("b")){
      var temp = message.split("|")[0] + "|" + message.split("|")[1] + "|N/A";
      message = temp;
    }

    io.emit('data', message);

    // Send Ok acknowledgement
    server.send(findMyWatch + toggle ,remote.port,remote.address,function(error){
      if(error){
        console.log('Could not send response');
      }
      else{
        console.log('Sent reponse');
        findMyWatch = "false";
      }
    });

});

// Bind server to port and IP
server.bind(PORT, HOST);
