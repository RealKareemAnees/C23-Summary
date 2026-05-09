#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* === 4.1 POSIX-Harmonized String Functions === */

void demo_posix_strings(void) {
    printf("\n--- POSIX String Functions ---\n");

    // strdup: allocate and copy
    char* original = "Hello, World!";
    char* copy = strdup(original);
    printf("strdup: '%s'\n", copy);
    free(copy);

    // strndup: allocate and copy (limited length)
    char* partial = strndup("Hello, World!", 5);
    printf("strndup(5): '%s'\n", partial);
    free(partial);
}

/* === 4.2 POSIX Time Functions (Reentrant) === */

void demo_posix_time_functions(void) {
    printf("\n--- POSIX Time Functions (Reentrant) ---\n");

    time_t now = time(NULL);

    // gmtime_r: thread-safe UTC time conversion
    struct tm utc_time;
    gmtime_r(&now, &utc_time);

    // Format using strftime
    char buf[100];
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S UTC", &utc_time);
    printf("Current UTC: %s\n", buf);

    // localtime_r: thread-safe local time conversion
    struct tm local_time;
    localtime_r(&now, &local_time);

    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S %Z", &local_time);
    printf("Current local: %s\n", buf);
}

/* === 4.3 UTF-8 Character Functions (C23) === */

void demo_utf8_functions(void) {
    printf("\n--- UTF-8 Character Functions (C23) ---\n");

#ifdef __STDC_UTF_8__
    printf("UTF-8 support available\n");

    // Example: Encoding/decoding UTF-8 (implementation would use mbrtoc8/c8rtomb)
    const char* utf8_str = "Café";
    printf("UTF-8 string: %s\n", utf8_str);
#else
    printf("UTF-8 support not fully available on this platform\n");
#endif
}

/* === 4.4 Checked Integer Arithmetic (C23) === */

void demo_checked_arithmetic(void) {
    printf("\n--- Checked Integer Arithmetic (C23) ---\n");

#ifdef __STDC_NO_STDCKDINT__
    printf("Checked arithmetic not available; simulating...\n");
    unsigned x = 0xFFFFFFFFU;
    unsigned y = 2;
    unsigned result;
    if (x > UINT_MAX - y) {
        printf("Overflow detected: %u + %u\n", x, y);
    } else {
        result = x + y;
        printf("Safe result: %u\n", result);
    }
#else
    #include <stdckdint.h>
    unsigned result;
    if (ckd_add(&result, 0xFFFFFFFFU, 2)) {
        printf("Checked arithmetic: overflow detected\n");
    } else {
        printf("Checked arithmetic: result = %u\n", result);
    }
#endif
}

/* === 4.5 Bit Utilities (C23) === */

void demo_bit_utilities(void) {
    printf("\n--- Bit Utilities (C23) ---\n");

#ifdef __STDC_NO_STDBIT__
    printf("Bit utilities not available; using built-ins...\n");
    unsigned x = 0b10101100;
    int bits = __builtin_popcount(x);
    printf("popcount(0b10101100) = %d\n", bits);
#else
    #include <stdbit.h>
    unsigned x = 0b10101100;
    int bits = popcount(x);
    printf("Type-generic popcount(0b10101100) = %d\n", bits);
#endif
}

/* === 4.6 Extended Math Functions (C23) === */

void demo_extended_math(void) {
    printf("\n--- Extended Math Functions (C23) ---\n");

    printf("exp2(10) = %f (2^10)\n", exp2(10.0));
    printf("log2(1024) = %f\n", log2(1024.0));

    // Fused multiply-add: a*b+c (single rounding)
    double a = 1.5, b = 2.0, c = 0.5;
    double result = fma(a, b, c);
    printf("fma(1.5, 2.0, 0.5) = %f (1.5*2.0+0.5)\n", result);
}

/* === 4.7 128-Bit Integer Platform Check === */

void demo_128bit_availability(void) {
    printf("\n--- 128-Bit Integer Support ---\n");

#if defined(__SIZEOF_INT128__)
    printf("Platform supports 128-bit integers (__int128 available)\n");

    unsigned __int128 big = 1;
    big <<= 100;
    printf("2^100 = (very large number)\n");
#else
    printf("128-bit integers NOT available on this platform\n");
#endif

    printf("\nNote: Full C23 int128_t support requires:\n");
    printf("  • <stdint.h> types/macros\n");
    printf("  • <inttypes.h> format specifiers\n");
    printf("  • printf/scanf %w128 support\n");
    printf("  • Clang 18+ or GCC 10+ (consistent versions)\n");
}

/* === 4.8 Library Implementation Status === */

void print_library_status(void) {
    printf("\n--- C23 Library Implementation Status (2025) ---\n");

    printf("glibc:\n");
#ifdef __GLIBC__
    printf("  Version: %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
    if (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 39)) {
        printf("  ✓ Substantial C23 library support available\n");
    } else {
        printf("  ⊘ Limited C23 support; upgrade recommended\n");
    }
#else
    printf("  Not glibc (using %s or other)\n", "musl/BSD/other");
#endif

    printf("\nSupported features:\n");
    printf("  ✓ POSIX string functions (strdup, strndup, memccpy)\n");
    printf("  ✓ POSIX time functions (gmtime_r, localtime_r)\n");

#ifdef __STDC_NO_STDBIT__
    printf("  ⊘ Bit utilities (<stdbit.h>) not available\n");
#else
    printf("  ✓ Bit utilities (<stdbit.h>) available\n");
#endif

#ifdef __STDC_NO_STDCKDINT__
    printf("  ⊘ Checked arithmetic (<stdckdint.h>) not available\n");
#else
    printf("  ✓ Checked arithmetic (<stdckdint.h>) available\n");
#endif
}

/* === 4.9 Platform-Specific Library Selection === */

void demo_library_selection(void) {
    printf("\n--- Library Selection Strategy ---\n");

    printf("Recommended libraries for full C23 support:\n");
    printf("  1. glibc 2.39+ (Linux, current)\n");
    printf("  2. musl with C23 patches (musl/Linux, via build-musl script)\n");
    printf("  3. BSD libc (limited C23 support; check version)\n");
    printf("  4. MSVC CRT (limited C23 support; check version)\n");
}

int main(void) {
    printf("=== Chapter 25: Appendix C – C Libraries ===\n");

    demo_posix_strings();
    demo_posix_time_functions();
    demo_utf8_functions();
    demo_checked_arithmetic();
    demo_bit_utilities();
    demo_extended_math();
    demo_128bit_availability();
    print_library_status();
    demo_library_selection();

    printf("\n=== End Chapter 25 ===\n");
    return 0;
}
