#ifndef COMPILER_SETTINGS_ENGINE_H
#define COMPILER_SETTINGS_ENGINE_H

#if defined(_MSC_VER)
#include <intrin.h>
#include <malloc.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#else
#error "Bad compiler (use MSCV, clang or GCC)"
#endif

// TODO: it's just a scalar realisation, need to do vectorisation
inline void scatterData(float* base, const __m256i& vindex, const __m256& values, int scale) {
#if defined(__AVX2__) || defined(_M_AVX2)
    alignas(32) uint32_t indices[8];
    alignas(32) float vals[8];

    _mm256_store_si256((__m256i*)indices, vindex);
    _mm256_store_ps(vals, values);

    for (int i = 0; i < 8; ++i) {
        base[indices[i]] = vals[i];
    }
#endif
}

#endif  // COMPILER_SETTINGS_ENGINE_H