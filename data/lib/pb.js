///////////////////////////////////////////////
//     _____  _____   ____ _______ ____      //
//    |  __ \|  __ \ / __ \__   __/ __ \     //
//    | |__) | |__) | |  | | | | | |  | |    //
//    |  ___/|  _  /| |  | | | | | |  | |    //
//    | |    | | \ \| |__| | | | | |__| |    //
//    |_|    |_|  \_\\____/  |_|  \____/     //
//                                           //
///////////////////////////////////////////////




function pdSerialize(type, value) {
  return type.encode(type.create(value)).finish();
}

function pdDeserialize(type, value) {
  return type.toObject(type.decode(new Uint8Array(value)), {
    longs: String,
    enums: String,
    bytes: Uint8Array,
    defaults: true,
    arrays: true,
    objects: true,
    oneofs: true
  });
}
