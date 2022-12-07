/**
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import { log } from './logging';
import { testBloomFilter } from './test_bloom_filter';
// @ts-ignore
import { loadWebAssemblyModule } from './wasm/wasb';
import { getMd5HashValue } from '@firebase/firestore';

export async function runTheJSTest(): Promise<void> {
  testBloomFilter();
}

export async function runTheWasmTest(): Promise<void> {
  const webAssemblyInstance = await loadWebAssemblyModule();
  const addResult = webAssemblyInstance.add(1, 2);
  log(`add(${1}, ${2}) returned ${addResult}`);

  log(`Calling reverse("Hello World!")`);
  const reversedText = webAssemblyInstance.reverse('Hello World!');
  log(`Reversed string: ${reversedText}`);

  log(`Calling hash("abc")`);
  const hashedText = webAssemblyInstance.hash('abc');
  log(`Hashed string: ${hashedText}`);
}

export async function runTheHashTest(): Promise<void> {
  const textToHashElement = document.getElementById(
    'txtHashString'
  ) as HTMLInputElement;
  const textToHash = textToHashElement.value || '';

  log('Run MD5 hash with firestore JS function');
  const time1 = performance.now();
  const hashedTestJS = getMd5HashValue(textToHash);
  const time2 = performance.now();
  log(`Hashed string: ${hashedTestJS}`);
  log(`Time used is ${time2 - time1}`);
  log('\n');

  log('Run MD5 hash with webasembly function');
  const webAssemblyInstance = await loadWebAssemblyModule();
  const time3 = performance.now();

  const hashedTextWasm = webAssemblyInstance.hash(textToHash) as Uint8Array;
  const time4 = performance.now();

  log(`Hashed string: ${hashedTextWasm}`);
  log(`Time used is ${time4 - time3}`);
  log('\n');

  log(
    `The result is ${
      hashedTestJS.toString() === hashedTextWasm.toString() ? '' : ' NOT '
    } the same`
  );
}
