////////////////////////////////
//      _____ _____ _____     //
//     / ____|  __ \_   _|    //
//    | (___ | |__) || |      //
//     \___ \|  ___/ | |      //
//     ____) | |    _| |_     //
//    |_____/|_|   |_____|    //
//                            //
////////////////////////////////

let spiWs;

let SPIReadWriteRequest;
let SPIReadWriteResponse;

let autoReconnect = false;
let onConnect;

function initSPI(onconnect = () => {}) {
  protobuf.load("/proto/spi.proto", function (err, root) {
    SPIReadWriteRequest = root.lookupType("SPIReadWriteRequest");
    SPIReadWriteResponse = root.lookupType("SPIReadWriteResponse");
  });
  autoReconnect = true;
  onConnect = onconnect;
  connectSpiWs();
  return waitForOpen(spiWs);
}

function connectSpiWs() {
  spiWs = new WebSocket("/api/v1/spi/ws");
  spiWs.binaryType = "arraybuffer";
  spiWs.onclose = (error) => {
    if (error.code !== 1001) {
      autoReconnectWs();
      console.log("ws error:", error);
    }
  };
  spiWs.onerror = (error) => {
    console.log("ws error:", error);
  };
  spiWs.addEventListener("open", onConnect);
}

function autoReconnectWs() {
  if (autoReconnect) {
    connectSpiWs();
    console.log("reconnected");
  }
}

function reconnectSPI() {
  return waitForOpen(spiWs);
}

async function writeSPI(data, speed = 2000000, bitOrder = 1, mode = 0) {
  if (
    speed < 100000 ||
    speed > 12500000 ||
    bitOrder < 0 ||
    bitOrder > 1 ||
    mode < 0 ||
    mode > 3 ||
    !validateBytearray(data)
  ) {
    throw new Error("Incorrect argument(s)");
  }
  if (spiWs.readyState !== WebSocket.OPEN) {
    await reconnectSPI();
  }
  spiWs.send(
    pdSerialize(SPIReadWriteRequest, {
      write: data,
      rnBytes: 0,
      rwByte: 0,
      speed: speed,
      bitOrder: bitOrder,
      mode: mode,
      noAnswer: false,
    })
  );
  return new Promise((resolve, reject) => {
    waitForMsg(spiWs)
      .then((msg) => {
        resolve(pdDeserialize(SPIReadWriteResponse, msg).error);
      })
      .catch((error) => {
        reject("NetworkError");
      });
  });
}

async function writeReadSPI(
  data,
  rnBytes,
  rwByte,
  speed = 2000000,
  bitOrder = 1,
  mode = 0
) {
  if (
    speed < 100000 ||
    speed > 12500000 ||
    rnBytes < 0 ||
    rnBytes > 4096 ||
    rwByte < 0 ||
    rwByte > 255 ||
    bitOrder < 0 ||
    bitOrder > 1 ||
    mode < 0 ||
    mode > 3 ||
    !validateBytearray(data)
  ) {
    throw new Error("Incorrect argument(s)");
  }
  if (spiWs.readyState !== WebSocket.OPEN) {
    await reconnectSPI();
  }
  spiWs.send(
    pdSerialize(SPIReadWriteRequest, {
      write: data,
      rnBytes: rnBytes,
      rwByte: rwByte,
      speed: speed,
      bitOrder: bitOrder,
      mode: mode,
      noAnswer: false,
    })
  );
  return new Promise((resolve, reject) => {
    waitForMsg(spiWs)
      .then((msg) => {
        resolve(pdDeserialize(SPIReadWriteResponse, msg));
      })
      .catch((error) => {
        reject("NetworkError");
      });
  });
}

async function writeSPINoCheck(
  data,
  waittime,
  speed = 2000000,
  bitOrder = 1,
  mode = 0
) {
  if (
    speed < 100000 ||
    speed > 12500000 ||
    bitOrder < 0 ||
    bitOrder > 1 ||
    mode < 0 ||
    mode > 3 ||
    !validateBytearray(data)
  ) {
    throw new Error("Incorrect argument(s)");
  }
  if (spiWs.readyState !== WebSocket.OPEN) {
    await reconnectSPI();
  }
  spiWs.send(
    pdSerialize(SPIReadWriteRequest, {
      write: data,
      rnBytes: 0,
      rwByte: 255,
      speed: speed,
      bitOrder: bitOrder,
      mode: mode,
      noAnswer: true,
    })
  );
  const delay = (delayInms) => {
    return new Promise((resolve) => setTimeout(resolve, delayInms));
  };
  await delay(waittime);
}
