#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* === 4.1 Compiler Detection === */

void demo_compiler_detection(void) {
    printf("\n--- Compiler Detection ---\n");

#ifdef __GNUC__
    printf("GCC detected: v%d.%d.%d\n",
           __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif

#ifdef __clang__
    printf("Clang detected: v%d.%d.%d\n",
           __clang_major__, __clang_minor__, __clang_patchlevel__);
#endif

#ifdef _MSC_VER
    printf("MSVC detected: v%d\n", _MSC_VER);
#endif

#if defined(__unix__) || defined(__APPLE__)
    printf("Platform: Unix-like system\n");
#elif defined(_WIN32)
    printf("Platform: Windows\n");
#endif
}

/* === 4.2 C Standard Version Detection === */

void demo_c_standard_support(void) {
    printf("\n--- C Standard Support ---\n");

#if __STDC_VERSION__ >= 202311L
    printf("C23 (ISO/IEC 9899:2024) supported\n");
#elif __STDC_VERSION__ >= 201710L
    printf("C17 (ISO/IEC 9899:2018) supported\n");
#elif __STDC_VERSION__ >= 201112L
    printf("C11 (ISO/IEC 9899:2011) supported\n");
#elif __STDC_VERSION__ >= 199901L
    printf("C99 (ISO/IEC 9899:1999) supported\n");
#else
    printf("C89/C90 (traditional)\n");
#endif

    printf("__STDC_VERSION__ = %ld\n", __STDC_VERSION__);
}

/* === 4.3 GCC/Clang Attribute Fallback === */

#if defined(__GNUC__) || defined(__clang__)
    // Use GCC/Clang extensions
    #define PURE_FUNCTION __attribute__((__const__))
    #define NO_SIDE_EFFECTS __attribute__((__pure__))
#else
    // Fallback for other compilers
    #define PURE_FUNCTION
    #define NO_SIDE_EFFECTS
#endif

PURE_FUNCTION int factorial_pure(int n) {
    return (n <= 1) ? 1 : n * factorial_pure(n - 1);
}

NO_SIDE_EFFECTS double sqrt_approx(double x) {
    // May read errno or other global state, but no side effects
    return x > 0 ? x / 2.0 : 0.0;
}

void demo_attribute_extensions(void) {
    printf("\n--- Compiler Extension Attributes ---\n");
    printf("Pure factorial(5) = %d\n", factorial_pure(5));
    printf("Approx sqrt(16.0) = %f\n", sqrt_approx(16.0));
}

/* === 4.4 constexpr Workaround (Clang Compatibility) === */

#if __is_identifier(constexpr)
    // constexpr not a keyword; safe to define
    #define constexpr static const
    #define USING_CONSTEXPR_FALLBACK 1
#else
    // Native constexpr support
    #define USING_CONSTEXPR_FALLBACK 0
#endif

constexpr int BUFFER_SIZE = 1024;
constexpr double GOLDEN_RATIO = 1.618033988749895;

void demo_constexpr_support(void) {
    printf("\n--- constexpr Support ---\n");

#if USING_CONSTEXPR_FALLBACK
    printf("Using fallback: constexpr → static const\n");
#else
    printf("Native constexpr support (C23)\n");
#endif

    printf("BUFFER_SIZE = %d\n", BUFFER_SIZE);
    printf("GOLDEN_RATIO = %f\n", GOLDEN_RATIO);
}

/* === 4.5 128-Bit Integer Assumptions === */

void demo_128bit_integer_support(void) {
    printf("\n--- 128-Bit Integer Support ---\n");

#if defined(__SIZEOF_INT128__)
    printf("128-bit integers available (__int128 extension)\n");
    unsigned __int128 big = 1;
    big <<= 100;
    printf("2^100 computed successfully\n");
#else
    printf("128-bit integers NOT available on this platform\n");
#endif

#ifdef __clang__
    printf("Clang version: %d.%d\n", __clang_major__, __clang_minor__);
    if (__clang_major__ < 18) {
        printf("⚠ WARNING: Clang <18 has ABI incompatibilities with GCC for 128-bit types\n");
    }
#endif
}

/* === 4.6 Feature Test Checklist === */

struct CompilerCapabilities {
    const char* compiler_name;
    int has_digit_separators;
    int has_binary_literals;
    int has_attribute_syntax;
    int has_constexpr;
    int has_128bit;
    int has_unsequenced;
};

void demo_compiler_capabilities(void) {
    printf("\n--- Compiler Capabilities (2025) ---\n");

    struct CompilerCapabilities caps = {
        .compiler_name = NULL,
        .has_digit_separators = 1,  // Most compilers support now
        .has_binary_literals = 1,
        .has_attribute_syntax = 1,
        .has_constexpr = 0,  // Clang lacks this
        .has_128bit = 0,     // Clang <18 unreliable
        .has_unsequenced = 0  // Missing in GCC 14
    };

#ifdef __clang__
    caps.compiler_name = "Clang";
    caps.has_constexpr = (__clang_major__ >= 19);
    caps.has_128bit = (__clang_major__ >= 18);
    caps.has_unsequenced = (__clang_major__ >= 19);
#endif

#ifdef __GNUC__
    if (!caps.compiler_name) {
        caps.compiler_name = "GCC";
        caps.has_constexpr = (__GNUC__ >= 10);
        caps.has_128bit = (__GNUC__ >= 10);
        caps.has_unsequenced = (__GNUC__ >= 14);  // Estimated
    }
#endif

    if (!caps.compiler_name) {
        caps.compiler_name = "Unknown";
    }

    printf("Compiler: %s\n", caps.compiler_name);
    printf("  ✓ Digit separators: %s\n", caps.has_digit_separators ? "Yes" : "No");
    printf("  ✓ Binary literals: %s\n", caps.has_binary_literals ? "Yes" : "No");
    printf("  ✓ Attribute syntax: %s\n", caps.has_attribute_syntax ? "Yes" : "No");
    printf("  ✓ constexpr: %s\n", caps.has_constexpr ? "Yes" : "No");
    printf("  ✓ 128-bit integers: %s\n", caps.has_128bit ? "Yes" : "No");
    printf("  ✓ [[unsequenced]]: %s\n", caps.has_unsequenced ? "Yes" : "No");
}

/* === 4.7 Compiler Version Recommendation === */

void print_compiler_recommendations(void) {
    printf("\n--- Compiler Version Recommendations (April 2026) ---\n");
    printf("For Full C23 Support:\n");
    printf("  • GCC: 10+ (reasonable C23); 14+ (better)\n");
    printf("  • Clang: 14+ (reasonable C23); 18+ (128-bit types); 19+ (attributes)\n");
    printf("  • MSVC: Limited C23 support; check current version\n\n");
    printf("For Maximum Compatibility:\n");
    printf("  • Use c23-fallback.h header\n");
    printf("  • Test on multiple compiler versions\n");
    printf("  • Document minimum version requirement\n");
}

int main(void) {
    printf("=== Chapter 24: Appendix B – C Compilers ===\n");

    demo_compiler_detection();
    demo_c_standard_support();
    demo_attribute_extensions();
    demo_constexpr_support();
    demo_128bit_integer_support();
    demo_compiler_capabilities();
    print_compiler_recommendations();

    printf("\n=== End Chapter 24 ===\n");
    return 0;
}
