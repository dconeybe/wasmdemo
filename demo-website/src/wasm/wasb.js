'use strict';
import * as wasm from './wasm.json';
const WASM_BASE64 = wasm.code;

function MyWebAssemblyInstance(instance) {
  this.echo = function (num) {
    if (typeof num !== 'number') {
      throw new Error(`num is not a number: ${num}`);
    }
    instance.exports.echo(num);
  };

  this.add = function (num1, num2) {
    if (typeof num1 !== 'number') {
      throw new Error(`num1 is not a number: ${num}`);
    }
    if (typeof num2 !== 'number') {
      throw new Error(`num2 is not a number: ${num}`);
    }
    return instance.exports.add(num1, num2);
  };

  this.reverse = function (s) {
    const buf = new Uint8Array(instance.exports.memory.buffer);
    const { written: numBytes } = new TextEncoder('utf8').encodeInto(s, buf);
    instance.exports.reverse_string(buf, numBytes);
    return new TextDecoder('ascii').decode(buf.subarray(0, numBytes));
  };


  this.hash = function(s) {
    const {hash, memory} = instance.exports;
    let offset = 0;
    const inputBuf = new Uint8Array(memory.buffer, offset, s.length);
    const { written: numBytes } = new TextEncoder("utf8").encodeInto(s, inputBuf);

    const HASH_OUTPUT_SIZE = 16;
    offset += length * Uint8Array.BYTES_PER_ELEMENT;
    const outputBuf = new Uint8Array(memory.buffer, offset, HASH_OUTPUT_SIZE)

    hash(inputBuf, numBytes, outputBuf);
    return outputBuf;
  }
}

export async function loadWebAssemblyModule() {
  const wasm = Uint8Array.from(atob(WASM_BASE64), v => v.charCodeAt(0));
  const wasmInstantiateResult = await WebAssembly.instantiate(wasm, {
    base: {
      log: function (num) {
        log(`${num} logged from WebAssembly!`);
      }
    }
  });
  const { instance } = wasmInstantiateResult;
  return new MyWebAssemblyInstance(instance);
}
