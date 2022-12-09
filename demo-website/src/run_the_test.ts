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
import { testBloomFilter, BloomFilterType } from './test_bloom_filter';
// @ts-ignore
import { loadWebAssemblyModule } from './index.mjs';
import { getMd5HashValue } from '@firebase/firestore';


export async function runTheJSTest(): Promise<void> {
  await testBloomFilter(BloomFilterType.JSBloomFilter);
}

export async function runTheWasmTest(): Promise<void> {
  await testBloomFilter(BloomFilterType.WasmBloomFilter);
}

export async function runTheHashTest(): Promise<void> {
  const textToHashElement = document.getElementById(
    'txtHashString'
  ) as HTMLInputElement;
  let numOfHash = parseInt(textToHashElement.value) || 1;
  log(`MD5 Hash random strings for ${numOfHash} times.`);
  const Prefix = 'projects/project-1/databases/database-1/documents/coll/doc';
  const webAssemblyInstance = await loadWebAssemblyModule();
  let timeJS = 0,
    timeWasm = 0;

  while (numOfHash > 0) {
    const randomString = Prefix + Math.random().toString(36).slice(2, 7);
    const time1 = performance.now();
    getMd5HashValue(randomString);
    const time2 = performance.now();
    timeJS += time2 - time1;

    const time3 = performance.now();
    webAssemblyInstance.hash(randomString);
    const time4 = performance.now();
    timeWasm += time4 - time3;
    numOfHash--;
  }

  log(`JS MD5 Hash used: ${timeJS.toFixed(3)} milliseconds`);
  log(`Wasm MD5 Hash used: ${timeWasm.toFixed(3)} milliseconds`);
}
