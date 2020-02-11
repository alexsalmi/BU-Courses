var app = require('express')();
var http = require('http').createServer(app);
var io = require('socket.io')(http);
var fs = require('fs');

// Points to index.html to serve webpage
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

// User socket connection
io.on('connection', function(socket){
  console.log('a user connected');
  var content = fs.readFileSync("data.tsv", "utf8").split('\n').slice(1,-1);
  var dataArray = Array(5);

    content.forEach((line) => {
        var data = line.split('\t');
        if(dataArray[data[1]-1] == undefined){
            dataArray[data[1]-1] = Array();
        }
        dataArray[data[1]-1].push({x: +data[0], y: +data[3]});
    })
  io.emit('data', dataArray);
  socket.on('disconnect', function(){
    console.log('user disconnected');
  });
});


// Listening on localhost:3000
http.listen(3000, function() {
  console.log('listening on *:3000');
});
