#define MM256_SHR(dst, src, n) \
	do { \
		__m128i hi = _mm256_extracti128_si256(src, 1); \
		switch(n) { \
			case 0: dst = src; break; \
			case 1: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 1), _mm_alignr_epi8(hi, lo, 1)); \
				break; \
			} \
			case 2: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 2), _mm_alignr_epi8(hi, lo, 2)); \
				break; \
			} \
			case 3: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 3), _mm_alignr_epi8(hi, lo, 3)); \
				break; \
			} \
			case 4: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 4), _mm_alignr_epi8(hi, lo, 4)); \
				break; \
			} \
			case 5: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 5), _mm_alignr_epi8(hi, lo, 5)); \
				break; \
			} \
			case 6: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 6), _mm_alignr_epi8(hi, lo, 6)); \
				break; \
			} \
			case 7: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 7), _mm_alignr_epi8(hi, lo, 7)); \
				break; \
			} \
			case 8: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 8), _mm_alignr_epi8(hi, lo, 8)); \
				break; \
			} \
			case 9: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 9), _mm_alignr_epi8(hi, lo, 9)); \
				break; \
			} \
			case 10: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 10), _mm_alignr_epi8(hi, lo, 10)); \
				break; \
			} \
			case 11: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 11), _mm_alignr_epi8(hi, lo, 11)); \
				break; \
			} \
			case 12: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 12), _mm_alignr_epi8(hi, lo, 12)); \
				break; \
			} \
			case 13: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 13), _mm_alignr_epi8(hi, lo, 13)); \
				break; \
			} \
			case 14: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 14), _mm_alignr_epi8(hi, lo, 14)); \
				break; \
			} \
			case 15: { \
				const __m128i lo = _mm256_castsi256_si128(src); \
				dst = _mm256_set_m128i(_mm_srli_si128(hi, 15), _mm_alignr_epi8(hi, lo, 15)); \
				break; \
			} \
				case 16: dst = _mm256_set_m128i(_mm_setzero_si128(), hi); break;\
			case 17: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 17 - 16)); \
				break; \
			} \
			case 18: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 18 - 16)); \
				break; \
			} \
			case 19: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 19 - 16)); \
				break; \
			} \
			case 20: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 20 - 16)); \
				break; \
			} \
			case 21: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 21 - 16)); \
				break; \
			} \
			case 22: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 22 - 16)); \
				break; \
			} \
			case 23: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 23 - 16)); \
				break; \
			} \
			case 24: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 24 - 16)); \
				break; \
			} \
			case 25: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 25 - 16)); \
				break; \
			} \
			case 26: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 26 - 16)); \
				break; \
			} \
			case 27: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 27 - 16)); \
				break; \
			} \
			case 28: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 28 - 16)); \
				break; \
			} \
			case 29: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 29 - 16)); \
				break; \
			} \
			case 30: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 30 - 16)); \
				break; \
			} \
			case 31: { \
				dst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, 31 - 16)); \
				break; \
			} \
			default: \
				dst = _mm256_setzero_si256(); \
				break; \
		} \
	} while (0)
