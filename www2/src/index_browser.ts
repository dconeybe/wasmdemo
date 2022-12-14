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
import { log, resetStartTime, setLogFunction } from './logging';
import { clearLogs, log as browserLog } from './logging_browser';
import { formatElapsedTime } from './util';

// Initialize the logging framework.
setLogFunction(browserLog);

let currentCancellationTokenSource: CancellationTokenSource | null = null;

async function go(this: GlobalEventHandlers, ev: MouseEvent) {
  const { btnRun, btnCancel } = getUiElements();
  const startTime: DOMHighResTimeStamp = performance.now();
  const title = (ev.currentTarget as HTMLElement).innerText;

  if (currentCancellationTokenSource) {
    currentCancellationTokenSource.cancel();
  }
  const cancellationTokenSource = new CancellationTokenSource();
  currentCancellationTokenSource = cancellationTokenSource;

  log(`"${title}" started`);
  try {
    btnRun.disabled = true;
    btnCancel.disabled = false;
    btnCancel.onclick = (ev: MouseEvent) => {
      log(`"${(ev.currentTarget as HTMLElement).innerText}" clicked`);
      cancellationTokenSource.cancel();
    };
    await runTheTest(cancellationTokenSource.cancellationToken);
  } catch (e) {
    if (e instanceof Error) {
      log(`ERROR: ${e.message}`, { alsoLogToConsole: false });
      console.log(e.stack);
    } else {
      log(`ERROR: ${e}`);
    }
  } finally {
    btnRun.disabled = false;
  }
  const endTime: DOMHighResTimeStamp = performance.now();
  const elapsedTimeStr = formatElapsedTime(startTime, endTime);
  log(`"${title}" completed in ${elapsedTimeStr}`);
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
  btnRun: HTMLButtonElement;
  btnCancel: HTMLButtonElement;
  btnClearLogs: HTMLButtonElement;
}

/** Returns the HTML elements from the UI with which this script interacts. */
function getUiElements(): UiElements {
  return {
    btnRun: document.getElementById('btnRun') as HTMLButtonElement,
    btnCancel: document.getElementById(
      'btnCancel'
    ) as HTMLButtonElement,
    btnClearLogs: document.getElementById('btnClearLogs') as HTMLButtonElement,
  };
}

/** Registers callbacks and initializes state of the HTML UI. */
function initializeUi(): void {
  const { btnRun, btnCancel, btnClearLogs } = getUiElements();
  btnRun.onclick = go;
  btnCancel.disabled = true;
  btnClearLogs.onclick = clearLogsAndResetStartTime;

  log(`Click "${btnRun.innerText}" to run the test`);
}

// Call initializeUi() to get everything wired up.
initializeUi();
