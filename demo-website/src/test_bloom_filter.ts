import { BloomFilter as JSBloomFilter, ByteString } from '@firebase/firestore';
// @ts-ignore
import { loadWebAssemblyModule } from './index.mjs';
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

export enum BloomFilterType {
  JSBloomFilter,
  WasmBloomFilter
}

async function testBloomFilterAgainstExpectedResult(
  bloomFilterInputs: GoldenTestInput,
  expectedResult: GoldenTestExpectedResult,
  bloomFilterType: BloomFilterType
): Promise<void> {
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
    ${(time2 - time1).toFixed(3)} milliseconds`
  );

  let bloomFilter;
  if (bloomFilterType === BloomFilterType.JSBloomFilter) {
    bloomFilter = new JSBloomFilter(byteArray, padding, hashCount);
  } else {
    bloomFilter = await loadWebAssemblyModule();
    bloomFilter.initBloom(byteArray, padding, hashCount);
  }
  const time3 = performance.now();

  for (let i = 0; i < membershipTestResults.length; i++) {
    const expectedMembershipResult = membershipTestResults[i] === '1';
    const mightContain = bloomFilter.mightContain(documentPrefix + i);
    console.assert(
      mightContain === expectedMembershipResult,
      "MightContain result doesn't match the backend result."
    );
  }
  const time4 = performance.now();
  log(
    `Time used for running mighContain ${membershipTestResults.length} times:
    ${(time4 - time3).toFixed(3)} milliseconds`
  );
  log(
    `On average, running one mightContain() takes ${(
      (time4 - time3) /
      membershipTestResults.length
    ).toFixed(3)} milliseconds`
  );
  log('\n');
}

export async function  testBloomFilter(bloomFilterType: BloomFilterType) {
  log('-------1 Doc--------');
  await testBloomFilterAgainstExpectedResult(
    TEST_DATA.count1Rate0001TestData,
    TEST_DATA.count1Rate0001TestResult,
    bloomFilterType
  );
  log('-------500 Docs--------');
  await testBloomFilterAgainstExpectedResult(
    TEST_DATA.count500Rate0001TestData,
    TEST_DATA.count500Rate0001TestResult,
    bloomFilterType
  );
  log('-------5000 Docs--------');
  await testBloomFilterAgainstExpectedResult(
    TEST_DATA.count5000Rate0001TestData,
    TEST_DATA.count5000Rate0001TestResult,
    bloomFilterType
  );
  log('-------50000 Docs--------');
  await testBloomFilterAgainstExpectedResult(
    TEST_DATA.count50000Rate01TestData,
    TEST_DATA.count50000Rate01TestResult,
    bloomFilterType
  );
  await testBloomFilterAgainstExpectedResult(
    TEST_DATA.count50000Rate0001TestData,
    TEST_DATA.count50000Rate0001TestResult,
    bloomFilterType
  );
}
