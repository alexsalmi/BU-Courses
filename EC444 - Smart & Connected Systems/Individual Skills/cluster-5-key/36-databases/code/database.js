var Engine = require('tingodb')();

const readline = require('readline');
const fs = require('fs');

var db = new Engine.Db('../database/', {});
var collection = db.collection("smoke_data");

// Function to load data from smoke.txt into the database
function read_file_to_db(){
    const readInterface = readline.createInterface({
        input: fs.createReadStream('./smoke.txt'),
        output: process.stdout,
        console: false
    });

    readInterface.on('line', function(line) {
        var words = line.split('\t');
        if(words[0] != "Time"){
            collection.insert([{
                ID: words[1],
                time: words[0],
                smoke: words[2],
                temp: words[3]
            }],
            {w:1},
            function(err, result){});
        }
    });
}

// This function call is commented out because the data is already in the database
// read_file_to_db();

const cons = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
  
cons.question('Enter an ID from 1 to 5 to get temp and smoke values over time: ', (answer) => {
    // TODO: Log the answer in a database
    collection.find({ID: answer}).toArray(function(err,items){
        if(err)
            console.log(err);
        else{
            if(items.length > 0){
                console.log("\nData for sensor with ID #" + answer + "\n");
                console.log("Time\tSmoke\tTemperature");
                items.forEach(item => {
                    console.log(item.time + "\t" + item.smoke + "\t" + item.temp);
                });
            }
            else{
                console.log("\nNo data available for this ID\n");
            }
        }
    });

    cons.close();
});
