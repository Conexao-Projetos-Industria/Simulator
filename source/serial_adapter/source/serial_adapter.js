var activityManagerSocket = new WebSocket("ws://192.168.0.8:9091");

activityManagerSocket.onmessage = function (event) {
    console.log(event.data);
  }

activityManagerSocket.send("positionUpdate(&**&)2 0");
await sleep(2000);
activityManagerSocket.send("positionUpdate(&**&)2 2");
await sleep(2000);
activityManagerSocket.send("positionUpdate(&**&)2 0");