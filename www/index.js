"use strict";

const WASM_BASE64 = "REPLACE_WITH_BASE64";
const logElement = document.getElementById("pLogs");
const num1Element = document.getElementById("txtNum1");
const num2Element = document.getElementById("txtNum2");
const textToReverseElement = document.getElementById("txtReverseString");
const textToHashElement = document.getElementById("txtHashString");

const ERRNO_SUCCESS = 0;
const ERRNO_EACCES = 2;

const WASI_IMPORTS = Object.freeze({
  wasi_snapshot_preview1: {
    fd_close: function(fd) {
      log(`WARNING: fd_close(${fd}) unexpectedly invoked`);
      return ERRNO_EACCES;
    },
    fd_seek: function(fd, offset, whence, new_offset_ptr) {
      log(`WARNING: fd_seek(${fd}) unexpectedly invoked`);
      return ERRNO_EACCES;
    },
    fd_write: function(fd, iovs, iovs_len, new_offset_ptr) {
      log(`WARNING: fd_write(${fd}) unexpectedly invoked`);
      return ERRNO_EACCES;
    },
  }
});

function log(text) {
  logElement.appendChild(document.createTextNode(text));
  logElement.appendChild(document.createElement('br'));
  console.log(text);
}

function onClearClick() {
  console.log(`"Clear" button clicked`);
  logElement.innerHTML = "";
}

function WasmString(instance, ptr, size) {
  this.ptr = ptr;
  this.size = size;

  this.free = function() {
    instance.exports.free(ptr);
  }

  this.newUint8Array = function() {
    return new Uint8Array(instance.exports.memory.buffer, ptr, size);
  }

  this.toString = function() {
    const uint8Array = this.newUint8Array();
    return new TextDecoder("utf8").decode(uint8Array);
  }
}

function MyWebAssemblyInstance(instance) {
  this.malloc = function(size) {
    if (! Number.isInteger(size)) {
      throw new Error(`invalid size: ${size}`);
    } else if (size <= 0 || size >= 2147483648) {
      throw new Error(`size out of range: ${size}`);
    }
    return instance.exports.malloc(size);
  }

  this.free = function(ptr) {
    instance.exports.free(ptr);
  }

  this.echo = function(message) {
    const wasmMessage = this.newWasmString(message);
    try {
      instance.exports.echo(wasmMessage.ptr, wasmMessage.size);
    } finally {
      wasmMessage.free();
    }
  }

  this.echo_signed_unsigned = function(num) {
    instance.exports.echo_signed_unsigned(num);
  }

  this.add = function(num1, num2) {
    if (! Number.isInteger(num1)) {
      throw new Error(`invalid num1: ${num1}`);
    }
    if (! Number.isInteger(num2)) {
      throw new Error(`invalid num2: ${num2}`);
    }
    return instance.exports.add(num1, num2);
  }

  this.reverse = function(s) {
    const wasmString = this.newWasmString(s);
    try {
      instance.exports.reverse_string(wasmString.ptr, wasmString.size);
      return wasmString.toString();
    } finally {
      wasmString.free();
    }
  }

  this.hash = function(s) {
    const wasmString = this.newWasmString(s);
    let outputBufPtr;
    try {
      outputBufPtr = instance.exports.hash(wasmString.ptr, wasmString.size);
    } finally {
      wasmString.free();
    }
    return new Uint8Array(instance.exports.memory.buffer, outputBufPtr, 16);
  }

  this.newBloomFilter = function(bitmap, padding, hashCount) {
    const {memory, newBloomFilter} = instance.exports;
    const inputBuf = new Uint8Array(memory.buffer, bitmap.length);
    inputBuf.set(bitmap);
    return newBloomFilter(bitmap, bitmap.length, padding, hashCount);
  }

  this.mightContain = function(filterPointer, s) {
    const wasmString = this.newWasmString(s);
    let result;
    try {
      result = instance.exports.mightContain(filterPointer, wasmString.ptr, wasmString.size);
    } finally {
      wasmString.free();
    }

    return result;
  }

  this.deleteBloomFilter = function(filterPointer) {
    instance.exports.deleteBloomFilter(filterPointer);
  }

  this.newWasmString = function(value) {
    const valueStr = `${value}`;
    const mallocSize = valueStr.length * 4;
    const ptr = this.malloc(mallocSize);
    const uint8Array = new Uint8Array(instance.exports.memory.buffer, ptr, mallocSize);
    const { written: numBytes } = new TextEncoder("utf8").encodeInto(valueStr, uint8Array);
    return new WasmString(instance, ptr, numBytes);
  }
}

async function loadWebAssemblyModule() {
  const wasm = Uint8Array.from(atob(WASM_BASE64), v => v.charCodeAt(0));
  const instances = []
  const { instance } = await WebAssembly.instantiate(wasm, {
    base: {
      log: function(ptr, size) {
        const uint8Array = new Uint8Array(instances[0].exports.memory.buffer, ptr, size);
        const message = new TextDecoder("utf8").decode(uint8Array);
        log(`log(): ${message} (ptr=${ptr} size=${size})`);
      }
    },
    ...WASI_IMPORTS
  });
  instances.push(instance);
  return new MyWebAssemblyInstance(instance);
}

async function onHashTestWasmClick() {
  log("Hash Test Started");
  try {
    const webAssemblyInstance = await loadWebAssemblyModule();
    const numHashes = parseInt(num1Element.value);
    log(`Hashing ${numHashes} strings with WebAssembly MD5 algorithm`);
    let accumulatedMs = 0;

    for (let i=0; i<numHashes; i++) {
      const textToHash = `projects/my-cool-project/databases/(default)/` +
        `documents/collectionA/docA/collectionB/docB/collectionC/doc${i}`;
      const startTime = performance.now();
      webAssemblyInstance.hash(textToHash);
      const endTime = performance.now();
      accumulatedMs += endTime - startTime;
    }

    log(`Hashed ${numHashes} strings with WebAssembly MD5 algorithm ` +
      `in ${accumulatedMs.toFixed(3)} ms`);
  } catch (e) {
    log(`ERROR: ${e}`);
    console.log(e.stack);
  }

  log("Hash Test Completed");
}

async function onRunClick() {
  log("Run Started");
  try {
    const webAssemblyInstance = await loadWebAssemblyModule();

    const num1 = parseInt(num1Element.value);
    const num2 = parseInt(num2Element.value);
    const textToReverse = textToReverseElement.value;
    const textToHash = textToHashElement.value;

    const addResult = webAssemblyInstance.add(num1, num2);
    log(`add(${num1}, ${num2}) returned ${addResult}`);

    log(`Calling echo()`);
    webAssemblyInstance.echo(`This string was passed to echo()!`);
    log(`Calling echo_signed_unsigned()`);
    webAssemblyInstance.echo_signed_unsigned(num1);

    log(`Calling reverse("${textToReverse}")`);
    const reversedText = webAssemblyInstance.reverse(textToReverse);
    log(`Reversed string: ${reversedText}`);

    log(`Calling hash("${textToHash}")`);
    const hashedText = webAssemblyInstance.hash(textToHash);
    log (`Hashed string: ${hashedText}`);

    const mallocResult = webAssemblyInstance.malloc(8192);
    log(`malloc(8192) returned ${mallocResult}`);
    log(`Calling free(${mallocResult})`);
    webAssemblyInstance.free(mallocResult);
  } catch (e) {
    log(`ERROR: ${e}`);
    console.log(e.stack);
  }

  log("Run Completed");
}

function saveInputElementValue(element) {
  if (typeof Storage === 'undefined') {
    return;
  }
  window.sessionStorage.setItem(element.id, element.value);
}

function onTextInputChange(event) {
  saveInputElementValue(event.currentTarget);
}

function initializeInputElementValue(element) {
  if (typeof Storage === 'undefined') {
    return;
  }

  const value = window.sessionStorage.getItem(element.id);
  if (! value) {
    return;
  }

  element.value = value;
}

function initializeInputElementValues() {
  initializeInputElementValue(num1Element);
  initializeInputElementValue(num2Element);
  initializeInputElementValue(textToReverseElement);
}

function initializeWasmDemoUI() {
  num1Element.onchange = onTextInputChange;
  num2Element.onchange = onTextInputChange;
  textToReverseElement.onchange = onTextInputChange;

  document.getElementById("btnRun").onclick = onRunClick;
  document.getElementById("btnHashTestWasm").onclick = onHashTestWasmClick;
  document.getElementById("btnClear").onclick = onClearClick;

  initializeInputElementValues();
}
