import { SerialPort } from 'serialport';
import crc from 'crc';

// const port = new SerialPort({ path: '/dev/null', baudRate: 115200 })
const port = new SerialPort({ path: '/dev/cu.usbserial-3', baudRate: 115200 })

const id = Buffer.from([0x7E, 0x00, 0xFE, 0xFA, 0x02, 0x0f, 0x7E]);

const rts = Buffer.from([0x7E, 0x00, 0x0f, 0xFA, 0x06, 0x0f, 0x7E]);

// const config = Buffer.from([0x7E, 0x1e, 0x0f, 0x0a, 0x2e, 0x0a, 0x00, 0x02, 0x00, 0x00, 0x15, 0x27, 0x10, 0xab, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x27, 0xff, 0xff, 0x10, 0xab, 0xd2, 0x7E]);
const config = Buffer.from([0x7e, 0xb, 0x0a, 0xbf, 0x2e, 0x0a, 0x00, 0x01, 0x10, 0x00, 0x00, 0x79, 0x7e]);
// 7E 0B 0A BF 2E 0A 00 01 10 00 00 39 7E
const clearToSend = Buffer.from([0x7e, 0x05, 0x0a, 0xbf, 0x06, 0x79, 0x7e]);

// const status = Buffer.from([0x7E, 0x1d, 0xff, 0xaf, 0x13, 0xF0, 0xF1, 0x60, 0x12, 0x23, 0xF2, 0x00, 0x00, 0x00, 0xF3, 0xF4, 0x03, 0x00, 0xF5, 0x03, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x0, 0x00, 0x00, 0x00, 0x0f, 0x7E]);
//                           7e 20 ff af 13 00 00 4a 13 08 00 00 01 00 05 0c 01 00 00 00 00 00 00 00 00 4a 00 00 02 78 00 00 a2 7e
const status = Buffer.from([0x7e, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x13, 0x08, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xa2, 0x7e]);
// const status = Buffer.from([0x7e, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x49, 0x11, 0x1b, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xbe, 0x7e]);
// const status = Buffer.from([0x7e, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x60, 0xd, 0x35, 0x01, 0x00, 0x60, 0x00, 0x08, 0xc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xf2, 0x7e]);

// Bad
// {"id":"ff","command":"13","crc":"7e","dir":"out","payload":"7e 20 ff af 13 00 00 4a 07 08 00 00 c1 50 df af 52 07 07 04 00 00 00 00 35 00 00 02 78 00 00 36 7e 7e ","diff":21471}

const status1b = Buffer.from([0x7e, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x07, 0x08, 0x00, 0x00, 0xc1, 0x50, 0xdf, 0xaf, 0x52, 0x07, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0x36, 0x7e, 0x7e]);
// Good 
// {"id":"ff","command":"13","crc":"8f","dir":"out","payload":"7e 20 ff af 13 00 00 4a 07 09 00 00 01 00 89 08 01 00 00 00 00 00 00 00 00 35 00 00 02 78 00 00 8f 7e ","diff":33055}
const status1g = Buffer.from([0x7e, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x07, 0x09, 0x00, 0x00, 0x01, 0x00, 0x89, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0x8f, 0x7e]);

// Bad

// {"id":"ff","command":"13","crc":"7e","dir":"out","payload":"7e 20 ff af 13 00 00 4a 16 04 00 30 55 fb 99 7c a2 22 00 00 00 00 00 08 4a 00 00 02 1e 00 00 6e 7e 7e ","diff":1400}
const status2b = Buffer.from([0x7e, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x16, 0x04, 0x00, 0x30, 0x55, 0xfb, 0x99, 0x7c, 0xa2, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x4a, 0x00, 0x00, 0x02, 0x1e, 0x00, 0x00, 0x6e, 0x7e, 0x7e]);

// Good
// {"id":"ff","command":"13","crc":"6e","dir":"out","payload":"7e 20 ff af 13 00 00 4a 16 04 00 00 01 00 85 0c 01 00 00 00 00 00 00 00 08 4a 00 00 02 1e 00 00 6e 7e ","diff":3958}
const status2g = Buffer.from([0x7e, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x16, 0x04, 0x00, 0x00, 0x01, 0x00, 0x85, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x4a, 0x00, 0x00, 0x02, 0x1e, 0x00, 0x00, 0x6e, 0x7e]);


setInterval(() => {
  port.write(id, function (err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    console.log('ID Transmitted', id.toString('hex'))
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
  port.write(clearToSend, function (err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    console.log('clearToSend Transmitted', clearToSend)
  })
}, 1000);

setInterval(() => {
  const d = new Date();
  let hour = d.getHours();
  let minute = d.getMinutes();
  let second = d.getSeconds();


    status[8] = minute;
    status[9] = second;

  // 0xbe

  var checksum = compute_checksum(status.slice(1, status.length - 2));
  status[status.length - 2] = checksum;
  // console.log('status to Transmit', status)

  port.write(status, function (err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    console.log('status Transmitted', status)
  })
}, 300);

function sendConfig() {

  port.write(config, function (err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    console.log('Config Transmitted', config)
  })
}

function compute_checksum(bytes) {
  // console.log('compute_checksum', bytes, bytes.length);
  const checksum = crc.crc8(bytes, 0x02);
  return checksum ^ 0x02;
}

function concat(a, b) {
  const c = new Uint8Array(a.length + b.length);
  c.set(a);
  c.set(b, a.length);
  return c;
}


/*
00:00:12.877 V: [rs485]: Received: 10 - 7e 05 10 bf 06 5c 7e 
00:00:12.911 V: [rs485]: Received: 10 - 7e 07 10 bf 11 00 00 3e 7e 
00:00:12.939 V: [rs485]: Received: 10 - 7e 05 0a bf 06 79 7e 
00:00:12.964 V: [rs485]: Sent: 7e 05 0a bf 07 7e 7e 
00:00:12.983 V: [rs485]: Received: 10 - 7e 05 10 bf 06 5c 7e 
00:00:13.017 V: [rs485]: Received: 10 - 7e 07 10 bf 11 00 00 3e 7e 
00:00:13.048 V: [rs485]: Received: 10 - 7e 05 0a bf 06 79 7e 
00:00:13.067 V: [rs485]: Sent: 7e 05 0a bf 07 7e 7e 
00:00:13.092 V: [rs485]: Received: 10 - 7e 05 10 bf 06 5c 7e 
00:00:13.126 V: [rs485]: Received: 10 - 7e 07 10 bf 11 00 00 3e 7e 
00:00:13.160 V: [rs485]: Received: 10 - 7e 05 0a bf 06 79 7e 
00:00:13.185 V: [rs485]: Sent: 7e 05 0a bf 07 7e 7e 
00:00:13.214 V: [rs485]: Received: 10 - 7e 05 10 bf 06 5c 7e 
00:00:13.247 V: [rs485]: Received: 10 - 7e 07 10 bf 11 00 00 3e 7e 
00:00:13.282 V: [rs485]: Received: 10 - 7e 07 10 bf 11 00 00 3e 7e 
00:00:13.315 V: [rs485]: Received: 10 - 7e 05 0a bf 06 79 7e 
00:00:13.340 V: [rs485]: Sent: 7e 05 0a bf 07 7e 7e 
00:00:13.363 V: [rs485]: Received: 10 - 7e 05 10 bf 06 5c 7e 
00:00:13.392 V: [rs485]: Received: 10 - 7e 07 10 bf 11 00 00 3e 7e 
00:00:13.431 V: [rs485]: Received: 10 - 7e 05 0a bf 06 79 7e 
00:00:13.461 V: [rs485]: Sent: 7e 05 0a bf 07 7e 7e 
00:00:13.486 V: [rs485]: Received: 10 - 7e 05 10 bf 06 5c 7e 
00:00:13.524 V: [rs485]: Received: 10 - 7e 07 10 bf 11 00 00 3e 7e 
00:00:13.556 V: [rs485]: Received: 10 - 7e 05 0a bf 06 79 7e 
00:00:13.583 V: [rs485]: Sent: 7e 05 0a bf 07 7e 7e 
00:00:13.603 V: [rs485]: Received: 10 - 7e 05 10 bf 06 5c 7e 
00:00:13.640 V: [rs485]: Received: 10 - 7e 07 10 bf 11 00 00 3e 7e 
00:00:13.670 V: [rs485]: Received: 10 - 7e 05 0a bf 06 79 7e 
00:00:13.697 V: [rs485]: Sent: 7e 05 0a bf 07 7e 7e 
*/
