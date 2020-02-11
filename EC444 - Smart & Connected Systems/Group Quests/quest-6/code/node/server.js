// QUEST 5: Security Key
// TEAM 17: Karle Erf, Thuc Nguyen, Alex Salmi

// Main code for our nodejs server. The code is recieved from UDP packets from the hub, whenever a key is trying to gain access. This code is then checked against the database,
// and if it is a valid key, the server will respond to the hub with a USP packet with a success response. Otherwise, it will send back a failed response
// The server also communicates with the front end using socket.io. It will search the database for fob access over time, and whether a fob is currently in a room ('unlocked'),
// whenever the front end requests this info

var Engine = require('tingodb')();

var dgram = require('dgram');
var app = require('express')();
var http = require('http').createServer(app);
var io = require('socket.io')(http);
var bodyParser = require('body-parser');
var Jimp = require("jimp");
var fs = require('fs');
var QrCode = require('qrcode-reader');


setInterval(() => {
    var buffer = fs.readFileSync('http://192.168.1.36:8080/?action=snapshot');
    Jimp.read(buffer, function(err, image) {
        if (err) {
            console.error(err);
            // TODO handle error
        }
        var qr = new QrCode();
        qr.callback = function(err, value) {
            if (err) {
                console.error(err);
                // TODO handle error
            }
            else{
                console.log('READ QR CODE: ');
                console.log(value.result);
            }
        };
        qr.decode(image.bitmap);
    });
}, 2000)

var db = new Engine.Db('../database/', {});
var splits_collection = db.collection("splits");

// Port and IP
var PORT = 8080;
var HOST = '192.168.1.49';

app.use(bodyParser.urlencoded({ extended: true })); 

// Points to index.html to serve webpage
app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
});

// User socket connection
io.on('connection', function(socket){
    console.log('a user connected');
    get_splits()

    socket.on('disconnect', function(){
        console.log('user disconnected');
    });

    socket.on('direction', function(dir){
        console.log('NEW DIRECTION: ' + dir)
    })
    socket.on('toggleRemote', function(dir){
        console.log('TOGGLE: ' + dir)
    })
});

// Listening on localhost:3000
http.listen(3000, function() {
    console.log('listening on *:3000');
});

function get_splits(){
  let splitList = "";
  splits_collection.find({id: "0"}).toArray(function(err, items){
      if(err)
          console.log(err);
      else{
          if(items.length > 0){
              items.forEach(item => {
                  splitList = splitList + item.split + "|";
              });
          }
          else
              splitList = "There are no saved splits yet";
      }
      splitList = splitList.substring(0, splitList.length - 1);
      io.emit('splits', splitList);
  })
}

// Function to add a new user to the users table
function add_split(split){
    splits_collection.insert([{
        id: "0",
        split: split
    }],
    {w:1},
    function(err, result){});

    get_splits();
}

// Create socket
var server = dgram.createSocket('udp4');

// Create server
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// On connection, print out received message
server.on('message', function (split, remote) {
    console.log('From ' + remote.address + ':' + remote.port +' - "' + fob_id + '"');

    add_split(split);


    let response = "0";

    // Send Ok acknowledgement
    server.send(response ,remote.port,remote.address,function(error){
        if(error){
        console.log('Could not send response');
        }
        else{
        console.log('Sent reponse ' + response);
        }
    });
});

// Bind server to port and IP
server.bind(PORT, HOST);