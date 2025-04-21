///////////////////////////////
//     _____ ___   _____     //
//    |_   _|__ \ / ____|    //
//      | |    ) | |         //
//      | |   / /| |         //
//     _| |_ / /_| |____     //
//    |_____|____|\_____|    //
//                           //
///////////////////////////////

let I2CReadWriteRequest;
let I2CReadWriteResponse;

function initI2C() {
  protobuf.load("/proto/i2c.proto", function (err, root) {
    I2CReadWriteRequest = root.lookupType("I2CReadWriteRequest");
    I2CReadWriteResponse = root.lookupType("I2CReadWriteResponse");
  });
}

async function scanI2C() {
  let result = await (await fetch("/api/v1/i2c/scan")).text();
  let devices = [];
  result.split("").forEach((v, i) => {
    if (v === "1") {
      devices.push(i);
    }
  });
  return devices;
}

async function probeI2C(address, freq = 100000) {
  if (address < 0 || address > 127 || (freq !== 100000 && freq !== 400000)) {
    throw new Error("Incorrect argument(s)");
  }
  let result = await (
    await fetch(`/api/v1/i2c/read_write`, {
      method: "POST",
      body: pdSerialize(I2CReadWriteRequest, {
        address: address,
        frequency: freq,
        byteNumber: 0,
        write: [],
      }),
    })
  ).arrayBuffer();
  return (
    pdDeserialize(I2CReadWriteResponse, new Uint8Array(result)).error ===
    "SUCCESS"
  );
}

async function writeI2C(address, bytes, freq = 100000) {
  if (
    address < 0 ||
    address > 127 ||
    (freq !== 100000 && freq !== 400000) ||
    !validateBytearray(bytes)
  ) {
    throw new Error("Incorrect argument(s)");
  }
  let result = await (
    await fetch(`/api/v1/i2c/read_write`, {
      method: "POST",
      body: pdSerialize(I2CReadWriteRequest, {
        address: address,
        frequency: freq,
        byteNumber: 0,
        write: bytes,
      }),
    })
  ).arrayBuffer();
  return pdDeserialize(I2CReadWriteResponse, new Uint8Array(result)).error;
}

async function readI2C(address, register, number, freq = 100000) {
  if (typeof register == "number") {
    register = [register];
  }
  if (
    address < 0 ||
    address > 127 ||
    !validateBytearray(register) ||
    number < 0 ||
    number > 4096 ||
    (freq !== 100000 && freq !== 400000)
  ) {
    throw new Error("Incorrect argument(s)");
  }
  let obj = pdDeserialize(
    I2CReadWriteResponse,
    new Uint8Array(
      await (
        await fetch(`/api/v1/i2c/read_write`, {
          method: "POST",
          body: pdSerialize(I2CReadWriteRequest, {
            address: address,
            frequency: freq,
            byteNumber: number,
            write: register,
          }),
        })
      ).arrayBuffer()
    )
  );
  return obj;
}
