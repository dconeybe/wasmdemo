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

import { runTheJSTest, runTheWasmTest, runTheHashTest } from './run_the_test';
import { log, resetStartTime, setLogFunction } from './logging';
import { clearLogs, log as browserLog } from './logging_browser';
// import { formatElapsedTime } from './util';

/**
 * Formats an elapsed time into a human-friendly string.
 *
 * @param startTime the start time.
 * @param endTime the end time.
 * @returns A human-friendly string that indicates the amount of time that has
 * elapsed between the given `startTime` and `endTime`.
 */
function formatElapsedTime(
  startTime: DOMHighResTimeStamp,
  endTime: DOMHighResTimeStamp
): string {
  const milliseconds = endTime - startTime;
  const minutes = Math.floor(milliseconds / (1000 * 60));
  const seconds = (milliseconds - minutes * 1000 * 60) / 1000;

  const formattedSeconds = seconds.toFixed(3) + 's';
  if (minutes == 0) {
    return formattedSeconds;
  } else {
    return `${minutes}m ${formattedSeconds}`;
  }
}

// Initialize the logging framework.
setLogFunction(browserLog);

/**
 * Callback invoked whenever the "Enable Debug Logging" checkbox's checked state
 * changes.
 *
 * Sets up the `Firestore` instance and invoke the `runTheTest()` function from
 * `run_the_test.ts`.
 */
async function runJSTest(this: GlobalEventHandlers, ev: MouseEvent) {
  const { btnRunTestJS } = getUiElements();
  const startTime: DOMHighResTimeStamp = performance.now();
  const title = (ev.currentTarget as HTMLElement).innerText;
  log('\n');
  log(`"${title}" started`, { className: 'startLine' });
  try {
    btnRunTestJS.disabled = true;
    await runTheJSTest();
  } catch (e) {
    if (e instanceof Error) {
      log(`ERROR: ${e.message}`, { alsoLogToConsole: false });
      console.log(e.stack);
    } else {
      log(`ERROR: ${e}`);
    }
  } finally {
    btnRunTestJS.disabled = false;
  }
  const endTime: DOMHighResTimeStamp = performance.now();
  const elapsedTimeStr = formatElapsedTime(startTime, endTime);
  log(`"${title}" completed in ${elapsedTimeStr}`);
  log(`----------------------------------------------------------------------`);
}

async function runWasmTest(this: GlobalEventHandlers, ev: MouseEvent) {
  const { btnRunTestWasm } = getUiElements();
  const startTime: DOMHighResTimeStamp = performance.now();
  const title = (ev.currentTarget as HTMLElement).innerText;

  log(`"${title}" started`, { className: 'startLine' });
  try {
    btnRunTestWasm.disabled = true;
    await runTheWasmTest();
  } catch (e) {
    if (e instanceof Error) {
      log(`ERROR: ${e.message}`, { alsoLogToConsole: false });
      console.log(e.stack);
    } else {
      log(`ERROR: ${e}`);
    }
  } finally {
    btnRunTestWasm.disabled = false;
  }
  const endTime: DOMHighResTimeStamp = performance.now();
  const elapsedTimeStr = formatElapsedTime(startTime, endTime);
  log(`"${title}" completed in ${elapsedTimeStr}`);
  log(`----------------------------------------------------------------------`);
}

async function runHashTest(this: GlobalEventHandlers, ev: MouseEvent) {
  const { btnHashString } = getUiElements();
  const startTime: DOMHighResTimeStamp = performance.now();
  const title = (ev.currentTarget as HTMLElement).innerText;

  log(`"${title}" started`, { className: 'startLine' });
  try {
    btnHashString.disabled = true;
    await runTheHashTest();
  } catch (e) {
    if (e instanceof Error) {
      log(`ERROR: ${e.message}`, { alsoLogToConsole: false });
      console.log(e.stack);
    } else {
      log(`ERROR: ${e}`);
    }
  } finally {
    btnHashString.disabled = false;
  }
  const endTime: DOMHighResTimeStamp = performance.now();
  const elapsedTimeStr = formatElapsedTime(startTime, endTime);
  log(`"${title}" completed in ${elapsedTimeStr}`);
  log(`----------------------------------------------------------------------`);
}

/**
 * Clears the logs from the UI and resets the log time back to zero.
 */
function clearLogsAndResetStartTime(): void {
  clearLogs();
  resetStartTime();
}

// The HTML elements in the UI with which this script interacts.
interface UiElements {
  btnRunTestJS: HTMLButtonElement;
  btnRunTestWasm: HTMLButtonElement;
  btnHashString: HTMLButtonElement;
  btnClearLogs: HTMLButtonElement;
}

/** Returns the HTML elements from the UI with which this script interacts. */
function getUiElements(): UiElements {
  return {
    btnRunTestJS: document.getElementById('btnRunTestJS') as HTMLButtonElement,
    btnRunTestWasm: document.getElementById(
      'btnRunTestWasm'
    ) as HTMLButtonElement,
    btnHashString: document.getElementById(
      'btnHashString'
    ) as HTMLButtonElement,
    btnClearLogs: document.getElementById('btnClearLogs') as HTMLButtonElement
  };
}

/** Registers callbacks and initializes state of the HTML UI. */
function initializeUi(): void {
  const { btnRunTestJS, btnRunTestWasm, btnHashString, btnClearLogs } =
    getUiElements();
  btnRunTestJS.onclick = runJSTest;
  btnRunTestWasm.onclick = runWasmTest;
  btnHashString.onclick = runHashTest;
  btnClearLogs.onclick = clearLogsAndResetStartTime;
  log(`Click "${btnRunTestJS.innerText}" to run the test`);
}

// Call initializeUi() to get everything wired up.
initializeUi();
