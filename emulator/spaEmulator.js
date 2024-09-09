import { SerialPort } from 'serialport';
import crc from 'crc';


// const port = new SerialPort({ path: '/dev/null', baudRate: 115200 })
const port = new SerialPort({ path: '/dev/cu.usbserial-3', baudRate: 115200 })
const id = Buffer.from([0x7E, 0x00, 0xFE, 0xFA, 0x02, 0x0f, 0x7E]);

const rts = Buffer.from([0x7E, 0x00, 0x0f, 0xFA, 0x06, 0x0f, 0x7E]);

// const config = Buffer.from([0x7E, 0x1e, 0x0f, 0x0a, 0x2e, 0x0a, 0x00, 0x02, 0x00, 0x00, 0x15, 0x27, 0x10, 0xab, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x27, 0xff, 0xff, 0x10, 0xab, 0xd2, 0x7E]);
const config = Buffer.from([0x7e, 0xb, 0x0a, 0xbf, 0x2e, 0x0a, 0x00, 0x01, 0x10, 0x00, 0x00, 0x39, 0x7e]);
// 7E 0B 0A BF 2E 0A 00 01 10 00 00 39 7E
const clearToSend = Buffer.from([0x7e, 0x05, 0x0a, 0xbf, 0x06, 0x79, 0x7e]);

// 7E 0F 0A BF 25 11 03 32 50 50 68 89 03 41 02 93 7E
const settings = Buffer.from([0x7E, 0x0F, 0x0A, 0xBF, 0x25, 0x11, 0x03, 0x32, 0x50, 0x50, 0x68, 0x89, 0x03, 0x41, 0x02, 0x93, 0x7E]);
// 7E 0D 0A BF 23 13 00 04 00 87 00 04 00 FA 7E
const filter = Buffer.from([0x7E, 0x0D, 0x0A, 0xBF, 0x23, 0x13, 0x00, 0x04, 0x00, 0x87, 0x00, 0x04, 0x00, 0xFA, 0x7E]);
// 7E 1A 0A BF 24 64 C9 2C 00 53 52 42 50 35 30 31 58 03 09 57 FA 83 01 06 02 00 1F 7E
const information = Buffer.from([0x7E, 0x1A, 0x0A, 0xBF, 0x24, 0x64, 0xC9, 0x2C, 0x00, 0x53, 0x52, 0x42, 0x50, 0x35, 0x30, 0x31, 0x58, 0x03, 0x09, 0x57, 0xFA, 0x83, 0x01, 0x06, 0x02, 0x00, 0x1F, 0x7E]);

// const status = Buffer.from([0x7E, 0x1d, 0xff, 0xaf, 0x13, 0xF0, 0xF1, 0x60, 0x12, 0x23, 0xF2, 0x00, 0x00, 0x00, 0xF3, 0xF4, 0x03, 0x00, 0xF5, 0x03, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x0, 0x00, 0x00, 0x00, 0x0f, 0x7E]);
//                           7e 20 ff af 13 00 00 4a 13 08 00 00 01 00 05 0c 01 00 00 00 00 00 00 00 00 4a 00 00 02 78 00 00 a2 7e
const status = Buffer.from([0x7e, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x13, 0x08, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xa2, 0x7e]);

const tooLong = Buffer.from([0x7e, 0x7f, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x13, 0x08, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xa2, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x13, 0x08, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xa2, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x13, 0x08, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xa2, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x13, 0x08, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xa2, 0x20, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x13, 0x08, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xa2, 0x7e]);

const shorter = Buffer.from([0x7e, 0x32, 0xff, 0xaf, 0x13, 0x00, 0x00, 0x4a, 0x13, 0x08, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x05, 0x0c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x02, 0x78, 0x00, 0x00, 0xa2, 0x7e]);

console.log(shorter.length);


// Open errors will be emitted as an error event
port.on('error', function (err) {
  console.log('Error: ', err.message)
})

// fragment 8 a bf 22 0 0 1 58
// fragment 8 a bf 22 4 0 0 f4
// fragment 8 a bf 22 1 0 0 34
// fragment 8 a bf 22 2 0 0 89

port.on('data', async function (data) {
  parseMessageFromBinaryArray(data).forEach(async (fragment, index) => {
    console.log(`Fragment ${index}: ${fragment.map(byte => byte.toString(16)).join(' ')}`);
    if (fragment[3] === 0x22 && fragment[4] === 0x00) {
      console.log('Get Config', fragment.map(byte => byte.toString(16)).join(' '))
      await sendData(config)
    } else if (fragment[3] === 0x22 && fragment[4] === 0x04) {
      console.log('Get Settings 0x04', fragment.map(byte => byte.toString(16)).join(' '));
      await sendData(settings)
    } else if (fragment[3] === 0x22 && fragment[4] === 0x01) {
      console.log('Get Filter', fragment.map(byte => byte.toString(16)).join(' '))
      await sendData(filter)
    } else if (fragment[3] === 0x22 && fragment[4] === 0x02) {
      console.log('Get Information', fragment.map(byte => byte.toString(16)).join(' '))
      await sendData(information)
    } else {
      console.log('Data:', data)
    }

  })
})


setInterval(async () => {
  await sendData(tooLong);
}, 20000);

setInterval(async () => {
  await sendData(shorter);
}, 19000);

setInterval(async () => {
  await sendData(clearToSend);
}, 1000);

setInterval(async () => {
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

  await sendData(status);
}, 300);


async function sendData(data) {
  return new Promise((resolve, reject) => {
    port.write(data, async function (err) {
      await sleep(50);
      if (err) {
        console.log('Error on write:', err.message);
        reject(err);
      } else {
        console.log('Transmitted', data);
        resolve();
      }
    });
  });
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

function parseMessageFromBinaryArray(binaryArray) {
  // Split the message by '7e'
  const delimiter = 0x7e;
  const fragments = [];
  let currentFragment = [];

  // Iterate over the binary array
  for (let i = 0; i < binaryArray.length; i++) {
    if (binaryArray[i] === delimiter) {
      // If currentFragment has data, push it to the fragments array
      if (currentFragment.length > 0) {
        fragments.push(currentFragment);
        currentFragment = [];
      }
    } else {
      currentFragment.push(binaryArray[i]);
    }
  }

  // Check if there's one last fragment
  if (currentFragment.length > 0) {
    fragments.push(currentFragment);
  }

  // Process each fragment based on its second byte (length byte)
  return fragments.map(fragment => {
    if (fragment.length < 2) {
      return null; // Invalid fragment
    }

    const length = fragment[1]; // Second byte is the length
    const actualFragment = fragment.slice(0, length + 1); // Get the fragment based on length byte
    return actualFragment;
  }).filter(frag => frag !== null); // Filter out invalid fragments
}

async function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

// Example usage:
// const binaryArray = [0x7e, 0x08, 0x0a, 0xbf, 0x22, 0x00, 0x00, 0x01, 0x58, 0x7e, 0x7e, 0x08, 0x0a, 0xbf, 0x22, 0x04, 0x00, 0x00, 0xf4, 0x7e, 0x7e, 0x08, 0x0a, 0xbf, 0x22, 0x01, 0x00, 0x00, 0x34, 0x7e, 0x7e, 0x08, 0x0a, 0xbf, 0x22, 0x02, 0x00, 0x00, 0x89, 0x7e];

// const result = parseMessageFromBinaryArray(binaryArray);
// console.log(result);



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


// 00:04:54.525 V: [rs485]: spaMessage 7e 00 fe fa 02 0f 7e 7e 05 0a bf 06 79 7e 7e 20 ff af 13 00 00 4a 20 04 00 00 01 00 05 0c 01 00 00 00 00 00 00 00 00 4a 00 00 02 78 00 00 43 7e 7e 05 