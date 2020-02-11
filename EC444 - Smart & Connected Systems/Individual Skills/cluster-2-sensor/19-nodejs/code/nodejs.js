const SerialPort = require('serialport')

const port = new SerialPort('/dev/ttyUSB0', {
    baudRate: 115200
})

// Switches the port into "flowing mode"
port.on('data', function (data) {
    data = data.toString('utf-8')
    console.log('Data:', data)
})
