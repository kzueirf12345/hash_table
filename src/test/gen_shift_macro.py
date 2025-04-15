import argparse
def generate_shr_file(filename):
    with open(filename, "w") as f:
        f.write("#define MM256_SHR(dst, src, n) \\\n")
        f.write("\tdo { \\\n")
        f.write("\t\t__m128i hi = _mm256_extracti128_si256(src, 1); \\\n")
        f.write("\t\tswitch(n) { \\\n")
        # case 0
        f.write("\t\t\tcase 0: dst = src; break; \\\n")
        
        # cases 1-15
        for i in range(1, 16):
            f.write(f"\t\t\tcase {i}: {{ \\\n")
            f.write("\t\t\t\tconst __m128i lo = _mm256_castsi256_si128(src); \\\n")
            f.write(f"\t\t\t\tdst = _mm256_set_m128i(_mm_srli_si128(hi, {i}), _mm_alignr_epi8(hi, lo, {i})); \\\n")
            f.write("\t\t\t\tbreak; \\\n")
            f.write("\t\t\t} \\\n")
        # case 16
        f.write("\t\t\t\tcase 16: dst = _mm256_set_m128i(_mm_setzero_si128(), hi); break;\\\n")
        # cases 16-31
        for i in range(17, 32):
            f.write(f"\t\t\tcase {i}: {{ \\\n")
            f.write(f"\t\t\t\tdst = _mm256_set_m128i(_mm_setzero_si128(), _mm_srli_si128(hi, {i} - 16)); \\\n")
            f.write("\t\t\t\tbreak; \\\n")
            f.write("\t\t\t} \\\n")
        
        # case >=32
        f.write("\t\t\tdefault: \\\n")
        f.write("\t\t\t\tdst = _mm256_setzero_si256(); \\\n")
        f.write("\t\t\t\tbreak; \\\n")
        
        f.write("\t\t} \\\n")
        f.write("\t} while (0)\n")

def generate_shl_file(filename):
    with open(filename, "w") as f:
        f.write("#define MM256_SHL(dst, src, n) \\\n")
        f.write("\tdo { \\\n")
        f.write("\t\t__m128i lo = _mm256_castsi256_si128(src); \\\n")
        f.write("\t\tswitch(n) { \\\n")
        
        # case 0
        f.write("\t\t\tcase 0: dst = src; break; \\\n")
        
        # cases 1-15
        for i in range(1, 16):
            f.write(f"\t\t\tcase {i}: {{ \\\n")
            f.write("\t\t\t\tconst __m128i hi = _mm256_extracti128_si256(src, 1); \\\n")
            f.write(f"\t\t\t\tdst = _mm256_set_m128i(_mm_alignr_epi8(hi, lo, 16 - {i}), _mm_slli_si128(lo, {i})); \\\n")
            f.write("\t\t\t\tbreak; \\\n")
            f.write("\t\t\t} \\\n")
        
        # case 16
        f.write("\t\t\tcase 16: dst = _mm256_set_m128i(lo, _mm_setzero_si128()); break; \\\n")
        
        # cases 17-31
        for i in range(17, 32):
            f.write(f"\t\t\tcase {i}: {{ \\\n")
            f.write(f"\t\t\t\tdst = _mm256_set_m128i(_mm_slli_si128(lo, {i} - 16), _mm_setzero_si128()); \\\n")
            f.write("\t\t\t\tbreak; \\\n")
            f.write("\t\t\t} \\\n")
        
        # case >= 32
        f.write("\t\t\tdefault: \\\n")
        f.write("\t\t\t\tdst = _mm256_setzero_si256(); \\\n")
        f.write("\t\t\t\tbreak; \\\n")
        
        f.write("\t\t} \\\n")
        f.write("\t} while (0)\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="generate macro for shift __mm256i type with veriable n")
    parser.add_argument("--shl", help="SHL filebame")
    parser.add_argument("--shr", help="SHR filebame")
    args = parser.parse_args()

    if args.shl:
        generate_shl_file(args.shl)
    if args.shr:
        generate_shr_file(args.shr)