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

var db = new Engine.Db('../database/', {});
var users_collection = db.collection("users");
var access_collection = db.collection("accesses");

// Port and IP
var PORT = 8080;
var HOST = '192.168.1.49';

app.use(bodyParser.urlencoded({ extended: true })); 

// Points to index.html to serve webpage
app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
});

var ones = 'locked';
var twos = 'locked';
var threes = 'locked';

// User socket connection
io.on('connection', function(socket){
    console.log('a user connected');

    let userList = "";
    users_collection.find({authorized: "1"}).toArray(function(err, items){
        if(err)
            console.log(err);
        else{
            if(items.length > 0){
                items.forEach(item => {
                    userList = userList + item.fob_id + "|" + item.name + ",";
                });
            }
            else
                userList = "There are no saved users";
        }
        userList = userList.substring(0, userList.length - 1);
        io.emit('users', userList);
    })


    socket.on('disconnect', function(){
        console.log('user disconnected');
    });

    socket.on('newUser', function(data){
        let name = data.split("|")[0];
        let pin = data.split("|")[1];

        add_user(pin, name);
    })

    socket.on('searchAccesses', function(data){
        let result = "";

        access_collection.find({fob_id: data}).toArray(function(err,items){
            if(err){
                console.log(err);
            }
            else{
                if(items.length < 1){
                    result = "No Accesses for this ID ";
                }
                else{
                    if(data == '1111'){
                        result = result + "Status for fob with ID 1111: " + ones + ",";
                    }
                    if(data == '2222'){
                        result = result + "Status for fob with ID 2222: " + twos + ",";
                    }
                    if(data == '3333'){
                        result = result + "Status for fob with ID 3333: " + threes + ",";
                    }

                    items.forEach(item => {
                        result = result + "Time: " + item.time + "     \tLocation: " + item.location + "(Hub_id: " + item.hub_id + ")     \tFob_id: " + item.fob_id + "     \tFob User: " + item.person + "|";
                    });
                }
            }
            result = result.substring(0, result.length - 1);
    
            io.emit('searchResults', result);
        })
    })
});

// Listening on localhost:3000
http.listen(3000, function() {
    console.log('listening on *:3000');
});

// Function to add a new user to the users table
function add_user(fob_id, name){
    users_collection.insert([{
        fob_id: fob_id,
        name: name,
        authorized: "1"
    }],
    {w:1},
    function(err, result){});
}

function add_access(fob_id, hub_id, location){
    users_collection.find({fob_id: fob_id}).toArray(function(err,items){
        if(err)
            console.log(err);
        else{
            if(items.length > 0){
                item = items[0];
                var today = new Date();
                var date = today.getFullYear()+'-'+(today.getMonth()+1)+'-'+today.getDate();
                var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
                var dateTime = date+' '+time;
                access_collection.insert([{
                    fob_id: fob_id,
                    hub_id: hub_id,
                    person: item.name,
                    time: dateTime,
                    location: location
                }])
            }
            else
                console.log("No user available for this fob_id\n")
        }
    })
}

function get_accesses(fob_id){
    access_collection.find({fob_id: fob_id}).toArray(function(err,items){
        if(err){
            console.log(err);
        }
        else{
            if(items.length > 0){
                return items;
            }
            else{return "Nothing."}
        }
    })
}


// Create socket
var server = dgram.createSocket('udp4');

// Create server
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// On connection, print out received message
server.on('message', function (fob_id, remote) {
    console.log('From ' + remote.address + ':' + remote.port +' - "' + fob_id + '"');

    fob_id = fob_id.toString();

    let response = "0";

    users_collection.find({fob_id: fob_id}).toArray(function(err, items){
        if(err)
            console.log(err);
        else{
            if(items.length > 0){
                if(fob_id == '1111'){
                    if(ones == 'locked'){
                        ones = 'unlocked';
                    }
                    else{
                        ones = 'locked';
                    }
                }
                if(fob_id == '2222'){
                    if(twos == 'locked'){
                        twos = 'unlocked';
                    }
                    else{
                        twos = 'locked';
                    }
                }
                if(fob_id == '3333'){
                    if(threes == 'locked'){
                        threes = 'unlocked';
                    }
                    else{
                        threes = 'locked';
                    }
                }
                add_access(fob_id, "0000", "Main Hub");
                response = "1";
            }
            else{
                console.log("Invalid Fob");
                response = "0";
            }
        }

        // Send Ok acknowledgement
        server.send(response ,remote.port,remote.address,function(error){
            if(error){
            console.log('Could not send response');
            }
            else{
            console.log('Sent reponse ' + response);
            }
        });
    })


});

// Bind server to port and IP
server.bind(PORT, HOST);