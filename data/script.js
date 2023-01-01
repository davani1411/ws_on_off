var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

window.addEventListener('load',  onload);

function  onload(event) {
  initWebSocket();
  initButton2();
  initButton();
}

function initWebSocket() {
  console.log('Trying to open a WebSocket connection…');
  websocket = new WebSocket(gateway);
  websocket.onopen    = onOpen;
  websocket.onclose   = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log('Connection opened');
}

function onClose(event) {
  console.log('Connection closed');
  setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
  document.getElementById('state').innerHTML = event.data;
  console.log(event.data);
}

function initButton2() {
  document.getElementById('bON').addEventListener('click', toggleON2);
  document.getElementById('bOFF').addEventListener('click', toggleOFF2);
}

function toggleON2(event) {
  websocket.send('bON');
}
function toggleOFF2(event) {
  websocket.send('bOFF');
}


function initButton() {
  document.getElementById('b2ON').addEventListener('click', toggleON);
  document.getElementById('b2OFF').addEventListener('click', toggleOFF);
}

function toggleON(event) {
  websocket.send('b2ON');
}

function toggleOFF(event) {
  websocket.send('b2OFF');
}
