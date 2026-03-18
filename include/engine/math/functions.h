#ifndef MATH_FUNCTIONS_ENGINE_H
#define MATH_FUNCTIONS_ENGINE_H

#include "engine/core/CompilerSettings.h"

namespace engine::math {

[[nodiscard]] inline float lengthFromPos(float x1, float x2, float y1, float y2) noexcept {
    const float dx = x2 - x1;
    const float dy = y2 - y1;
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(dx * dx + dy * dy)));
}

[[nodiscard]] inline float lengthFromD(float dx, float dy) noexcept {
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(dx * dx + dy * dy)));
}

inline __m256 avxLengthFromPos(__m256 x1, __m256 x2, __m256 y1, __m256 y2) {
    const __m256 dx         = _mm256_sub_ps(x2, x1);
    const __m256 dy         = _mm256_sub_ps(y2, y1);
    const __m256 sq_sum     = _mm256_add_ps(_mm256_mul_ps(dx, dx), _mm256_mul_ps(dy, dy));
    const __m256 inv_length = _mm256_rsqrt_ps(sq_sum);

    return _mm256_div_ps(
        _mm256_set1_ps(1.0f),
        _mm256_mul_ps(inv_length,
                      _mm256_sub_ps(_mm256_set1_ps(1.5f), _mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f), sq_sum),
                                                                        _mm256_mul_ps(inv_length, inv_length)))));
}

inline __m256 avxLengthFromD(__m256 dx, __m256 dy) {
    const __m256 sq_sum     = _mm256_add_ps(_mm256_mul_ps(dx, dx), _mm256_mul_ps(dy, dy));
    const __m256 inv_length = _mm256_rsqrt_ps(sq_sum);

    return _mm256_div_ps(
        _mm256_set1_ps(1.0f),
        _mm256_mul_ps(inv_length,
                      _mm256_sub_ps(_mm256_set1_ps(1.5f), _mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f), sq_sum),
                                                                        _mm256_mul_ps(inv_length, inv_length)))));
}

}  // namespace engine::math

#endif  // MATH_FUNCTIONS_ENGINE_H