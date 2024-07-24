import { SerialPort } from 'serialport';

const port = new SerialPort({ path: '/dev/cu.usbserial-3', baudRate: 115200 })

const id = Buffer.from([0x7E, 0x00, 0xFE, 0xFA, 0x02, 0x0f, 0x7E]);

const rts = Buffer.from([0x7E, 0x00, 0x0f, 0xFA, 0x06, 0x0f, 0x7E]);

const config = Buffer.from([0x7E, 0x1e, 0x0f, 0x0a, 0x2e, 0x94, 0x02, 0x02, 0x80, 0x00, 0x15, 0x27, 0x10, 0xab, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x27, 0xff, 0xff, 0x10, 0xab, 0xd2, 0x7E]);

const status = Buffer.from([0x7E, 0x1d, 0xff, 0xaf, 0x13, 0xF0, 0xF1, 0x60, 0x12, 0x23, 0xF2, 0x00, 0x00, 0x00, 0xF3, 0xF4, 0x03, 0x00, 0xF5, 0x03, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x0, 0x00, 0x00, 0x00, 0x0f, 0x7E]);

setInterval(() => {
  port.write(id, function (err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    // console.log('ID Transmitted', id.toString('hex'))
  })
}, 5000);

// Open errors will be emitted as an error event
port.on('error', function (err) {
  console.log('Error: ', err.message)
})

port.on('data', function (data) {

  if (data[3] === 0xBF && data[5] === 0x00 && data[6] === 0x00) {
    console.log('Get Config', data)
    sendConfig()
  } else if (data[3] === 0xBF && data[5] === 0x20 && data[6] === 0xff) {
    console.log('Get Fault log', data)
  } else if (data[3] === 0xBF && data[4] === 0x07) {
    // console.log('CTS', data)
  } else {
    console.log('Data:', data)
  }
})

setInterval(() => {
  port.write(rts, function (err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    // console.log('RTS Transmitted', rts)
  })
}, 1000);

setInterval(() => {
  const d = new Date();
  let hour = d.getHours();
  let minute = d.getMinutes();

  status[8] = hour;
  status[9] = minute;
  var checksum = 0;
  for(let i = 0; i < status.length - 2; i++) {
    checksum += status[i];
  }
  // console.log('Checksum:', checksum)
  status[status.length - 2] = checksum & 0xFF;
  port.write(status, function (err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    // console.log('status Transmitted', rts)
  })
}, 1000);

function sendConfig() {

  port.write(config, function (err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    console.log('Config Transmitted', config)
  })
}