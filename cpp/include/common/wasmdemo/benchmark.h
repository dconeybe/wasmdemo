// This file is an excerpt from
// https://github.com/google/benchmark/blob/e67028c510196783b4cb8143d62f81f570fd828b/include/benchmark/benchmark.h#L431-L449

#define BENCHMARK_ALWAYS_INLINE __attribute__((always_inline))

// The DoNotOptimize(...) function can be used to prevent a value or
// expression from being optimized away by the compiler. This function is
// intended to add little to no overhead.
// See: https://youtu.be/nXaxk27zwlk?t=2441
template <class Tp>
inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp const& value) {
  asm volatile("" : : "r,m"(value) : "memory");
}

template <class Tp>
inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp& value) {
  asm volatile("" : "+r,m"(value) : : "memory");
}
