"use strict";

const WASM_BASE64 = "REPLACE_WITH_BASE64";
const logElement = document.getElementById("pLogs");
const preambleElement = document.getElementById("pPreamble");
const num1Element = document.getElementById("txtNum1");
const num2Element = document.getElementById("txtNum2");
const textToReverseElement = document.getElementById("txtReverseString");

function log(text, element) {
  if (! element) {
    element = logElement;
  }
  element.appendChild(document.createTextNode(text));
  element.appendChild(document.createElement('br'));
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
}

async function loadWebAssemblyModule() {
  const wasm = Uint8Array.from(atob(WASM_BASE64), v => v.charCodeAt(0));
  const wasmInstantiateResult = await WebAssembly.instantiate(wasm, {
    base: {
      log: function(num) {
        log(`${num} logged from WebAssembly!`);
      }
    }
  });
  const { instance } = wasmInstantiateResult;
  return new MyWebAssemblyInstance(instance);
}

async function onRunClick() {
  log("Run Started");
  try {
    const webAssemblyInstance = await loadWebAssemblyModule();

    const num1 = parseInt(num1Element.value);
    const num2 = parseInt(num2Element.value);
    const textToReverse = textToReverseElement.value;

    const addResult = webAssemblyInstance.add(num1, num2);
    log(`add(${num1}, ${num2}) returned ${addResult}`);

    webAssemblyInstance.echo(addResult);

    log(`Calling reverse("${textToReverse}")`);
    const reversedText = webAssemblyInstance.reverse(textToReverse);
    log(`Reversed string: ${reversedText}`);
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
  log("wasmdemo compiled REPLACE_WITH_DATE", preambleElement);
  log("wasmdemo size (in bytes): REPLACE_WITH_SIZE", preambleElement);

  num1Element.onchange = onTextInputChange;
  num2Element.onchange = onTextInputChange;
  textToReverseElement.onchange = onTextInputChange;

  document.getElementById("btnRun").onclick = onRunClick;
  document.getElementById("btnClear").onclick = onClearClick;

  initializeInputElementValues();
}

initialize();
