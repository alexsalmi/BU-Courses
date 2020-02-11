// QUEST 2: Sensor Central
// TEAM 17: Karle Erf, Thuc Nguyen, Alex Salmi

// Main code for our nodejs server. The data is recieved from serial, and is then sent to index.html using socket.io. 

var app = require('express')();
var http = require('http').createServer(app);
var io = require('socket.io')(http);

const SerialPort = require('serialport')
const port = new SerialPort('COM4', {
  baudRate: 115200
})

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

port.on('data', function (data) {
  data = data.toString('utf-8')

  io.emit('data', data);
  //console.log('Data:', data)
  })



