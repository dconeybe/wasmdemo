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
  let numOfHash = parseInt(textToHashElement.value) || 1;
  log(`MD5 Hash random strings for ${numOfHash} times.`);

  const webAssemblyInstance = await loadWebAssemblyModule();
  let timeJS = 0,
    timeWasm = 0;

  while (numOfHash > 0) {
    let randomString = Math.random().toString(36).slice(2, 7);
    const time1 = performance.now();
    getMd5HashValue(randomString);
    const time2 = performance.now();
    timeJS += time2 - time1;

    const time3 = performance.now();
    webAssemblyInstance.hash(randomString);
    const time4 = performance.now();
    timeWasm += time4 - time3;
    numOfHash--;
    console.log(`Times left: ${numOfHash}`)
  }

  log('\n');
  log(`JS MD5 Hash used: ${timeJS} milliseconds`);
  log('\n');

  log(`Wasm MD5 Hash used: ${timeWasm} milliseconds`);
  log('\n');
}
