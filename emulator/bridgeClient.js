
// import { console } from 'console';
import * as net from 'net';

const host = "spa-240AC4EC20DC.local.";
var isCurrentlyConnectedToSpa = false;
var socket = null;
var reconnecting = false;
var numberOfConnectionsSoFar = 0;


socket = get_socket(host);


socket?.on('data', (data) => {
  const bufView = new Uint8Array(data);
  console.log("Data received", bufView);
  // this.readAndActOnSocketContents(bufView);
});

function shutdownSpaConnection() {
  // Might already be disconnected, if we're in a repeat error situation.
  isCurrentlyConnectedToSpa = false;
  console.log("Shutting down Spa socket");
  /*
  if (this.faultCheckIntervalId) {
      clearInterval(this.faultCheckIntervalId);
      this.faultCheckIntervalId = undefined;
  }
  if (this.stateUpdateCheckIntervalId) {
      clearInterval(this.stateUpdateCheckIntervalId);
      this.stateUpdateCheckIntervalId = undefined;
  }
      */
  // Not sure I understand enough about these sockets to be sure
  // of best way to clean them up.
  if (socket != undefined) {
    socket.end();
    socket.destroy();
    socket = undefined;
  }
}

function reconnect(host) {
  if (!reconnecting) {
      reconnecting = true;
      setTimeout(() => {
          socket = get_socket(host);
          reconnecting = false;
      }, 20000);
  }
}

function get_socket(host) {
  if (isCurrentlyConnectedToSpa) {
    console.log("Already connected, should not be trying again.");
  }

  console.log("Connecting to Spa at", host, "on port 4257");
  socket = net.connect({
    port: 4257,
    host: host
  }, () => {
    numberOfConnectionsSoFar++;

    console.log('Successfully connected to Spa at', host,
      'on port 4257. This is connection number', numberOfConnectionsSoFar,
      'in days');
  });
  socket.on('end', () => {
    console.log("SpaClient: disconnected:");
  });
  // If we get an error, then retry
  socket.on('error', (error) => {
    console.log(error);
    console.log("Had error - closing old socket, retrying in 20s");

    shutdownSpaConnection();
    reconnect(host);
  });

  return socket;
}
