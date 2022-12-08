import { BloomFilter as JSBloomFilter, ByteString } from '@firebase/firestore';

import * as TEST_DATA from './bloom_filter_golden_test_data';
import { log } from './logging';

const documentPrefix =
  'projects/project-1/databases/database-1/documents/coll/doc';

interface GoldenTestInput {
  bits: {
    bitmap: string;
    padding: number;
  };
  hashCount: number;
}

interface GoldenTestExpectedResult {
  membershipTestResults: string;
}

enum BloomFilter {
  JSBloomFilter,
  WasmBloomFilter
}

function testBloomFilterAgainstExpectedResult(
  bloomFilterInputs: GoldenTestInput,
  expectedResult: GoldenTestExpectedResult,
  bloomFilterType: BloomFilter
): void {
  const {
    bits: { bitmap, padding },
    hashCount
  } = bloomFilterInputs;
  const { membershipTestResults } = expectedResult;
  log(
    `BloomFilter size : ${
      bitmap.length * 8 - padding
    },  hashCount:  ${hashCount} `
  );
  const time1 = performance.now();
  const byteArray = ByteString.fromBase64String(bitmap).toUint8Array();
  const time2 = performance.now();
  log(
    `Time used for decoding from 64base: 
    ${time2 - time1} milliseconds`
  );

  let bloomFilter;
  if ((bloomFilterType = BloomFilter.JSBloomFilter)) {
    bloomFilter = new JSBloomFilter(byteArray, padding, hashCount);
  } else {
    bloomFilter = new JSBloomFilter(byteArray, padding, hashCount);
  }
  for (let i = 0; i < membershipTestResults.length; i++) {
    const expectedMembershipResult = membershipTestResults[i] === '1';
    const mightContain = bloomFilter.mightContain(documentPrefix + i);
    console.assert(
      mightContain === expectedMembershipResult,
      "MightContain result doesn't match the backend result."
    );
  }
  const time3 = performance.now();
  log(
    `Time used for running mighContain ${membershipTestResults.length} times:
    ${time3 - time2} milliseconds`
  );
  log(
    `On average, running one mightContain() takes ${
      (time3 - time2) / membershipTestResults.length
    } milliseconds`
  );
  log('\n');
}

export function testBloomFilter() {
  log('-------1 Doc--------');
  testBloomFilterAgainstExpectedResult(
    TEST_DATA.count1Rate0001TestData,
    TEST_DATA.count1Rate0001TestResult,
    BloomFilter.JSBloomFilter
  );
  log('-------500 Docs--------');
  testBloomFilterAgainstExpectedResult(
    TEST_DATA.count500Rate0001TestData,
    TEST_DATA.count500Rate0001TestResult,
    BloomFilter.JSBloomFilter
  );
  log('-------5000 Docs--------');
  testBloomFilterAgainstExpectedResult(
    TEST_DATA.count5000Rate0001TestData,
    TEST_DATA.count5000Rate0001TestResult,
    BloomFilter.JSBloomFilter
  );
  log('-------50000 Docs--------');
  testBloomFilterAgainstExpectedResult(
    TEST_DATA.count50000Rate01TestData,
    TEST_DATA.count50000Rate01TestResult,
    BloomFilter.JSBloomFilter
  );
  testBloomFilterAgainstExpectedResult(
    TEST_DATA.count50000Rate0001TestData,
    TEST_DATA.count50000Rate0001TestResult,
    BloomFilter.JSBloomFilter
  );
}
