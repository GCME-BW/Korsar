let I2CReadWriteRequest = undefined;
let I2CReadWriteResponse = undefined;

const openI2C = (busNumber = 1, options = undefined) => {
  if (!Number.isInteger(busNumber) || busNumber < 0) {
    throw new Error("Invalid I2C bus number " + busNumber);
  }

  if (I2CReadWriteRequest === undefined) {
    protobuf.load("/proto/i2c.proto", function (err, root) {
      I2CReadWriteRequest = root.lookupType("I2CReadWriteRequest");
      I2CReadWriteResponse = root.lookupType("I2CReadWriteResponse");
    });
  }

  return new Bus(busNumber, options);
};

async function openI2CPromisified(busNumber = 1, options = undefined) {
  return new Promise((resolve, reject) => {
    const bus = openI2C(busNumber, options);
    resolve(bus.promisifiedBus());
  });
}

class Bus {
  constructor(busNumber, options) {
    options = options || {};
    this._frequency = !!options.frequency || 100000;

    this._busNumber = busNumber;
    this._promisifiedBus = new PromisifiedBus(this);
  }

  static #checkAddress(addr) {
    if (!Number.isInteger(addr) || addr < 0 || addr > 0x7f) {
      throw new Error("Invalid I2C address " + addr);
    }
  }

  static #checkRegister(reg) {
    if (Number.isInteger(reg) && reg >= 0 && reg <= 255) {
      return [reg];
    }
    if (Array.isArray(reg)) {
      Bus.#checkByteArray(reg);
      return reg;
    }
    throw new Error("Invalid I2C register " + reg);
  }

  static #checkCallback(cb) {
    if (typeof cb !== "function") {
      throw new Error("Invalid callback " + cb);
    }
  }

  static #checkByteArray(array) {
    if (Array.isArray(array)) {
      array.forEach((value) => {
        if (!Number.isInteger(value) || value < 0 || value > 255) {
          throw new Error("Invalid byte array " + array);
        }
      });
      return;
    }
    throw new Error("Invalid buffer " + array);
  }

  promisifiedBus() {
    return this._promisifiedBus;
  }

  scan(cb) {
    Bus.#checkCallback(cb);

    fetch("/api/v1/i2c/scan")
      .then((response) => response.text())
      .then((result) => {
        let devices = [];
        result.split("").forEach((v, i) => {
          if (v === "1") {
            devices.push(i);
          }
        });
        return devices;
      })
      .then(cb);
  }

  probe(addr, frequency = this._frequency, cb) {
    Bus.#checkAddress(addr);
    Bus.#checkCallback(cb);

    fetch(`/api/v1/i2c/read_write`, {
      method: "POST",
      body: pdSerialize(I2CReadWriteRequest, {
        address: addr,
        frequency,
        byteNumber: 0,
        write: [],
      }),
    })
      .then((response) => response.arrayBuffer())
      .then(
        (response_buffer) =>
          pdDeserialize(I2CReadWriteResponse, new Uint8Array(response_buffer))
            .error === "SUCCESS"
      )
      .then(cb);
  }

  i2cReadReg(addr, reg, length, frequency = this._frequency, cb) {
    Bus.#checkAddress(addr);
    reg = Bus.#checkRegister(reg);
    Bus.#checkCallback(cb);

    fetch(`/api/v1/i2c/read_write`, {
      method: "POST",
      body: pdSerialize(I2CReadWriteRequest, {
        address: addr,
        frequency,
        byteNumber: length,
        write: reg,
      }),
    })
      .then((response) => response.arrayBuffer())
      .then((response_buffer) =>
        pdDeserialize(I2CReadWriteResponse, new Uint8Array(response_buffer))
      )
      .then(cb);
  }

  i2cWriteReg(addr, reg, arr, frequency = this._frequency, cb) {
    Bus.#checkAddress(addr);
    reg = Bus.#checkRegister(reg);
    Bus.#checkByteArray(arr);
    Bus.#checkCallback(cb);

    fetch(`/api/v1/i2c/read_write`, {
      method: "POST",
      body: pdSerialize(I2CReadWriteRequest, {
        address: addr,
        frequency,
        byteNumber: 0,
        write: [...reg, ...arr],
      }),
    })
      .then((result) => result.arrayBuffer())
      .then((response_buffer) => {
        return {
          error: pdDeserialize(
            I2CReadWriteResponse,
            new Uint8Array(response_buffer)
          ).error,
        };
      })
      .then(cb);
  }

  close(cb) {
    Bus.#checkCallback(cb);

    //...
  }
}

class PromisifiedBus {
  constructor(bus) {
    this._bus = bus;
  }

  bus() {
    return this._bus;
  }

  async close() {
    return new Promise((resolve, reject) =>
      this._bus.close((err) => (err ? reject(err) : resolve()))
    );
  }

  async i2cReadReg(addr, reg, length, freq) {
    return new Promise((resolve, reject) =>
      this._bus.i2cReadReg(addr, reg, length, freq, (response, err) =>
        err ? reject(err) : resolve(response)
      )
    );
  }

  async i2cWriteReg(addr, reg, arr, freq) {
    return new Promise((resolve, reject) =>
      this._bus.i2cWriteReg(addr, reg, arr, freq, (response, err) =>
        err ? reject(err) : resolve(response)
      )
    );
  }

  async scan() {
    return new Promise((resolve, reject) =>
      this._bus.scan((devices, err) =>
        err ? reject(err) : resolve(devices)
      )
    );
  }

  async probe(addr, freq) {
    return new Promise((resolve, reject) =>
      this._bus.probe(addr, freq, (presense, err) =>
        err ? reject(err) : resolve(presense)
      )
    );
  }
}
