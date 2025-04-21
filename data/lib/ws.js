////////////////////////////////
//    __          _______     //
//    \ \        / / ____|    //
//     \ \  /\  / / (___      //
//      \ \/  \/ / \___ \     //
//       \  /\  /  ____) |    //
//        \/  \/  |_____/     //
//                            //
////////////////////////////////

function waitForOpen(ws) {
  return new Promise((resolve, reject) => {
    if (ws.readyState === WebSocket.OPEN) {
      resolve();
      return;
    }
    const listener = (event) => {
      ws.removeEventListener(event, listener);
      console.log("Connected");
      resolve();
    };
    ws.addEventListener("open", listener);
  });
}

function waitForMsg(ws, timeout = 1000) {
  return new Promise((resolve, reject) => {
    const msgListener = (event) => {
      ws.removeEventListener(event, msgListener);
      ws.removeEventListener(event, closeListener);
      clearTimeout(timeoutHandler);
      resolve(event.data);
    };
    const closeListener = (event) => {
      ws.removeEventListener(event, msgListener);
      ws.removeEventListener(event, closeListener);
      clearTimeout(timeoutHandler);
      reject(close);
    };
    ws.addEventListener("message", msgListener);
    ws.addEventListener("close", closeListener);
    const timeoutHandler = setTimeout(() => {
      ws.removeEventListener("message", msgListener);
      ws.removeEventListener("close", closeListener);
      reject();
    }, timeout);
  });
}
