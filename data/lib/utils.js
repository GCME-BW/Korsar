////////////////////////////////////////////////
//     _    _ _______ _____ _       _____     //
//    | |  | |__   __|_   _| |     / ____|    //
//    | |  | |  | |    | | | |    | (___      //
//    | |  | |  | |    | | | |     \___ \     //
//    | |__| |  | |   _| |_| |____ ____) |    //
//     \____/   |_|  |_____|______|_____/     //
//                                            //
////////////////////////////////////////////////

let download = (function () {
  let dwnld = document.createElement("a");
  document.body.appendChild(dwnld);
  dwnld.style = "display: none";
  return function (data, name) {
    let blob = new Blob(data, { type: "octet/stream" }),
      url = window.URL.createObjectURL(blob);
    dwnld.href = url;
    dwnld.download = name;
    dwnld.click();
    window.URL.revokeObjectURL(url);
  };
})();

function parseInt16(str) {
  if (
    str.startsWith(
      "0x" ||
        str.startsWith("0d") ||
        str.startsWith("0o") ||
        str.startsWith("0b")
    )
  ) {
    return parseInt(str);
  }
  return parseInt(str, 16);
}
function getInputValue(id) {
  return document.getElementById(id).value;
}
function getInputNumber(id) {
  return parseInt(document.getElementById(id).value);
}
function getInputInt16Value(id) {
  return parseInt16(document.getElementById(id).value);
}
function getInputInt16ArrayValue(id) {
  return getInputValue(id)
    .split(" ")
    .filter((str) => str !== "")
    .map((str) => parseInt16(str));
}
function elementTextAppend(id, text) {
  document.getElementById(id).innerHTML += text;
}
function printText(text) {
  elementTextAppend("output", text);
}
function stringToHex(str) {
  return `0x${str.toString(16).toUpperCase()}`;
}
function validateNumber(number, min, max) {
  return !isNaN(number) && number >= min && number <= max;
}
function validateBytearray(arr) {
  return (
    !arr.includes(NaN) &&
    arr.length >= 0 &&
    arr.every((arr) => arr >= 0 || arr <= 255)
  );
}

function getI2CErrorText(error) {
  switch (error) {
    case "SUCCESS":
      return "";
    case "INCORRECT_INPUT":
      return "Internal API error";
    case "NO_ACKNOWLEDGE_ADDRESS":
      return "Device with this address is not available";
    case "NO_ACKNOWLEDGE_DATA":
      return "Error: device is not ready to process data at the moment";
    case "TIMEOUT":
    case "UNKNOWN":
    default:
      return "Unknown error";
  }
}

function getSPIErrorText(error) {
  switch (error) {
    case "SUCCESS":
      return "";
    case "INCORRECT_INPUT":
      return "Internal API error";
    case "FRAME_FORMAT":
    case "UNKNOWN":
    default:
      return "Unknown error";
  }
}
