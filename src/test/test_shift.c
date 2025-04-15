#include <immintrin.h>
#include <stdio.h>
#include <stdint.h>

#include "test/test_shift.h"

#include "test/mm256_shl.inc"
#include "test/mm256_shr.inc"

static void print_m256i_(__m256i vec, const char* label) 
{
    uint8_t bytes[32];
    _mm256_storeu_si256((__m256i*)bytes, vec);
    
    printf("%s: ", label);
    for (int i = 0; i < 32; ++i) 
    {
        printf("%02x ", bytes[i]);
        if (i == 15) 
        {
            printf("| ");
        }
    }
    printf("\n");
}

void test_shr(int shift) 
{
    uint8_t data[32];
    for (uint8_t i = 0; i < 32; ++i) 
    {
        data[i] = i + 1;
    }
    
    __m256i src = _mm256_loadu_si256((__m256i*)data);
    __m256i dst;
    
    MM256_SHR(dst, src, shift);
    
    printf("\n=== TEST SHR %d ===\n", shift);
    print_m256i_(src, "Source ");
    print_m256i_(dst, "Result ");
}

void test_shl(int shift) 
{
    uint8_t data[32];
    for (uint8_t i = 0; i < 32; ++i) 
    {
        data[i] = i + 1; 
    }
    
    __m256i src = _mm256_loadu_si256((__m256i*)data);
    __m256i dst;
    
    MM256_SHL(dst, src, shift);
    
    printf("\n=== TEST SHL %d ===\n", shift);
    print_m256i_(src, "Source ");
    print_m256i_(dst, "Result ");
}