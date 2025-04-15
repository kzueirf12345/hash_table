#define MM256_SHL(dst, src, n) \
	do { \
		__m128i lo = _mm256_castsi256_si128(src); \
		switch(n) { \
			case 0: dst = src; break; \
			case 1: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 1), _mm_slli_si128(lo, 1)); \
				break; \
			} \
			case 2: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 2), _mm_slli_si128(lo, 2)); \
				break; \
			} \
			case 3: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 3), _mm_slli_si128(lo, 3)); \
				break; \
			} \
			case 4: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 4), _mm_slli_si128(lo, 4)); \
				break; \
			} \
			case 5: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 5), _mm_slli_si128(lo, 5)); \
				break; \
			} \
			case 6: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 6), _mm_slli_si128(lo, 6)); \
				break; \
			} \
			case 7: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 7), _mm_slli_si128(lo, 7)); \
				break; \
			} \
			case 8: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 8), _mm_slli_si128(lo, 8)); \
				break; \
			} \
			case 9: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 9), _mm_slli_si128(lo, 9)); \
				break; \
			} \
			case 10: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 10), _mm_slli_si128(lo, 10)); \
				break; \
			} \
			case 11: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 11), _mm_slli_si128(lo, 11)); \
				break; \
			} \
			case 12: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 12), _mm_slli_si128(lo, 12)); \
				break; \
			} \
			case 13: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 13), _mm_slli_si128(lo, 13)); \
				break; \
			} \
			case 14: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 14), _mm_slli_si128(lo, 14)); \
				break; \
			} \
			case 15: { \
				const __m128i hi = _mm256_extracti128_si256(src, 1); \
				dst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - 15), _mm_slli_si128(lo, 15)); \
				break; \
			} \
			case 16: dst = _mm256_set_m128i(lo, _mm_setzero_si128()); break; \
			case 17: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 17 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 18: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 18 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 19: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 19 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 20: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 20 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 21: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 21 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 22: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 22 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 23: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 23 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 24: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 24 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 25: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 25 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 26: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 26 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 27: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 27 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 28: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 28 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 29: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 29 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 30: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 30 - 16), _mm_setzero_si128()); \
				break; \
			} \
			case 31: { \
				dst = _mm256_set_m128i(_mm_slli_si128(lo, 31 - 16), _mm_setzero_si128()); \
				break; \
			} \
			default: \
				dst = _mm256_setzero_si256(); \
				break; \
		} \
	} while (0)
