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

async function onRunClick() {
  log("Run Started");
  try {
    const num1 = parseInt(num1Element.value);
    const num2 = parseInt(num2Element.value);
    const textToReverse = textToReverseElement.value;

    const wasm = Uint8Array.from(atob(WASM_BASE64), v => v.charCodeAt(0));
    const wasmModule = await WebAssembly.compile(wasm);
    const wasmInstance = new WebAssembly.Instance(wasmModule, {
      base: {
        log: function(num) {
          log(`${num} logged from WebAssembly!`);
        }
      }
    });

    const addResult = wasmInstance.exports.add(num1, num2);
    log(`add(${num1}, ${num2}) returned ${addResult}`);

    const buf = new Uint8Array(wasmInstance.exports.memory.buffer)
    const { written: numBytes } = new TextEncoder("utf8").encodeInto(textToReverse, buf);
    log(`Calling reverse("${textToReverse}", ${numBytes})`);
    wasmInstance.exports.reverse_string(buf, numBytes);
    const reversedText = new TextDecoder("ascii").decode(buf.subarray(0, numBytes));
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
  log("wasmdemo size: REPLACE_WITH_SIZE (md5: REPLACE_WITH_MD5)", preambleElement);

  num1Element.onchange = onTextInputChange;
  num2Element.onchange = onTextInputChange;
  textToReverseElement.onchange = onTextInputChange;

  document.getElementById("btnRun").onclick = onRunClick;
  document.getElementById("btnClear").onclick = onClearClick;

  initializeInputElementValues();
}

initialize();
