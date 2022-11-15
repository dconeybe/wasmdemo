"use strict";

const WASM_BASE64 = "REPLACE_WITH_BASE64";

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
    const textToReverse = document.getElementById("txtReverseString").value;

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
    const { written: numBytes } = new TextEncoder().encodeInto(textToReverse, buf);
    log(`Calling reverse("${textToReverse}", ${numBytes})`);
    wasmInstance.exports.reverse_string(buf, numBytes);
    const reversedText = new TextDecoder().decode(buf.subarray(0, numBytes));
    log(`Reversed string: ${reversedText}`);
  } catch (e) {
    log(`ERROR: ${e}`);
  }

  log("onRunClick() done");
}

document.getElementById("btnRun").onclick = onRunClick;

log("wasmdemo compiled REPLACE_WITH_DATE");
log("wasmdemo size: REPLACE_WITH_SIZE (md5: REPLACE_WITH_MD5)");
