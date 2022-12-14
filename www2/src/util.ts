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

/**
 * Generates and returns a random-ish number.
 *
 * The returned number will be 3 digits and will increase by roughly 10 each
 * second. This makes it a useful value for a document whose field is set
 * repeatedly because the value will increase monotonically, and it is easy for
 * a human to order the values.
 */
export function generateValue(): string {
  const value = `${Math.round(Date.now() / 250)}`;
  return value.substring(value.length - 3);
}

/**
 * Formats an elapsed time into a human-friendly string.
 *
 * @param startTime the start time.
 * @param endTime the end time.
 * @returns A human-friendly string that indicates the amount of time that has
 * elapsed between the given `startTime` and `endTime`.
 */
export function formatElapsedTime(
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
