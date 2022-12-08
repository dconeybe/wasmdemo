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

function MyWebAssemblyInstance(instance) {
  this.echo = function(num) {
    if (typeof(num) !== 'number') {
      throw new Error(`num is not a number: ${num}`);
    }
    instance.exports.echo(num);
  }

  this.add = function(num1, num2) {
    if (typeof(num1) !== 'number') {
      throw new Error(`num1 is not a number: ${num}`);
    }
    if (typeof(num2) !== 'number') {
      throw new Error(`num2 is not a number: ${num}`);
    }
    return instance.exports.add(num1, num2);
  }

  this.reverse = function(s) {
    const buf = new Uint8Array(instance.exports.memory.buffer)
    const { written: numBytes } = new TextEncoder("utf8").encodeInto(s, buf);
    instance.exports.reverse_string(buf, numBytes);
    return new TextDecoder("ascii").decode(buf.subarray(0, numBytes));
  }

  this.hash = function(s) {
    const {hash, memory} = instance.exports;
    let offset = 0;
    const inputBuf = new Uint8Array(memory.buffer, offset, s.length);
    const { written: numBytes } = new TextEncoder("utf8").encodeInto(s, inputBuf);

    const HASH_OUTPUT_SIZE = 16;
    offset += length * Uint8Array.BYTES_PER_ELEMENT;

    const outputBufPtr = hash(inputBuf, numBytes);

    const outputBuf = new Uint8Array(memory.buffer, outputBufPtr, 16);
    return [...outputBuf].map((x) => x.toString(16)).join(' ');
  }
}

async function loadWebAssemblyModule() {
  const wasm = Uint8Array.from(atob(WASM_BASE64), v => v.charCodeAt(0));
  const wasmInstantiateResult = await WebAssembly.instantiate(wasm, {
    base: {
      log: function(num) {
        log(`${num} logged from WebAssembly!`);
      }
    },
    ...WASI_IMPORTS
  });
  const { instance } = wasmInstantiateResult;
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
    console.log(e);
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

    webAssemblyInstance.echo(addResult);

    log(`Calling reverse("${textToReverse}")`);
    const reversedText = webAssemblyInstance.reverse(textToReverse);
    log(`Reversed string: ${reversedText}`);

    log(`Calling hash("${textToHash}")`);
    const hashedText = webAssemblyInstance.hash(textToHash);
    log (`Hashed string: ${hashedText}`);
  } catch (e) {
    log(`ERROR: ${e}`);
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

function initialize() {
  num1Element.onchange = onTextInputChange;
  num2Element.onchange = onTextInputChange;
  textToReverseElement.onchange = onTextInputChange;

  document.getElementById("btnRun").onclick = onRunClick;
  document.getElementById("btnHashTestWasm").onclick = onHashTestWasmClick;
  document.getElementById("btnClear").onclick = onClearClick;

  initializeInputElementValues();
}

initialize();
