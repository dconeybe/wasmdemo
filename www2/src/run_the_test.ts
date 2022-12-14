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
import { CancellationToken } from './cancellation_token';

/**
 * Runs the test.
 *
 * Replace the body of this function with the code you would like to execute
 * when the user clicks the "Run" button in the UI.
 *
 * @param cancellationToken a token that can be used to terminate early in the
 * case of a cancellation request.
 */
export async function runTheTest(
  cancellationToken: CancellationToken
): Promise<void> {
  log("This is the test starting");
  cancellationToken.throwIfCancelled();
  log("This is the test ending");
}
