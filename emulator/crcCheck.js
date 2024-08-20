import { SerialPort } from 'serialport';
import crc from 'crc';

// const port = new SerialPort({ path: '/dev/null', baudRate: 115200 })

const id = Buffer.from([0x7E, 0x00, 0xFE, 0xFA, 0x02, 0x0f, 0x7E]);

const rts = Buffer.from([0x7E, 0x00, 0x0f, 0xFA, 0x06, 0x0f, 0x7E]);

// const config = Buffer.from([0x7E, 0x1e, 0x0f, 0x0a, 0x2e, 0x0a, 0x00, 0x02, 0x00, 0x00, 0x15, 0x27, 0x10, 0xab, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x27, 0xff, 0xff, 0x10, 0xab, 0xd2, 0x7E]);
const config = Buffer.from([0x7e, 0xb, 0x0f, 0xbf, 0x2e, 0xa, 0x00, 0x01, 0x10, 0x00, 0x00, 0x37, 0x7e]);


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


checkChecksum(status);
checkChecksum(status1b);
checkChecksum(status1g);
checkChecksum(status2b);
checkChecksum(status2g);

function checkChecksum(data) {
  console.log('data', data);

  console.log('origChecksum', data[data.length - 2], 'checksum', compute_checksum(data.slice(1, data.length - 2)));
  console.log();
}


function compute_checksum(bytes) {
  // console.log('compute_checksum', bytes, bytes.length);
  const checksum = crc.crc8(bytes, 0x02);
  return checksum ^ 0x02;
}