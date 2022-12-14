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

import { runTheTest } from './run_the_test';
import { CancellationTokenSource } from './cancellation_token';
import { log, setLogFunction } from './logging';
import { log as nodeLog } from './logging_node';
import { formatElapsedTime } from './util';

// Initialize the logging framework.
setLogFunction(nodeLog);

async function go() {
  // Since there is no way to cancel when running in Node.js, just use a
  // CancellationToken that will never be cancelled.
  const cancellationToken = new CancellationTokenSource().cancellationToken;

  const startTime: DOMHighResTimeStamp = performance.now();
  log(`Test Started`);
  try {
    await runTheTest(cancellationToken);
  } catch (e) {
    if (e instanceof Error) {
      log(`ERROR: ${e.message}`, { alsoLogToConsole: false });
      console.log(e.stack);
    } else {
      log(`ERROR: ${e}`);
    }
  }
  const endTime: DOMHighResTimeStamp = performance.now();
  const elapsedTimeStr = formatElapsedTime(startTime, endTime);
  log(`Test completed in ${elapsedTimeStr}`);
}

// Run the program!
go();
