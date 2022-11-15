"use strict";

const WASM_BASE64 = undefined;

function log(text) {
  const logElement = document.getElementById("pOut");
  logElement.appendChild(document.createTextNode(text));
  logElement.appendChild(document.createElement('br'));
  console.log(text);
}

async function onRunClick() {
  log("onRunClick() start");
  try {
    const num1 = parseInt(document.getElementById("txtNum1").value);
    const num2 = parseInt(document.getElementById("txtNum2").value);

    const wasm = Uint8Array.from(atob(WASM_BASE64), v => v.charCodeAt(0));
    const wasmModule = await WebAssembly.compile(wasm);
    const wasmInstance = new WebAssembly.Instance(wasmModule);
    const result = wasmInstance.exports.add(num1, num2);
    log(`add(${num1}, ${num2}) returned ${result}`);
  } catch (e) {
    log(`ERROR: ${e}`);
  }

  log("onRunClick() done");
}

document.getElementById("btnRun").onclick = onRunClick;
