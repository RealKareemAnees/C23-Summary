#include <stdio.h>
#include <stdlib.h>

/* === 4.1 Feature Test Macros === */

#ifndef __has_c_attribute
# define __has_c_attribute(X) 0
#endif

#ifndef __has_include
# define __has_include(X) 0
#endif

#ifndef __is_identifier
# define __is_identifier(X) 1
#endif

void demo_feature_tests(void) {
    printf("\n--- Feature Test Macros ---\n");

#if __STDC_VERSION__ >= 202311L
    printf("Native C23 support detected\n");
#elif __STDC_VERSION__ >= 201710L
    printf("C17 detected; using c23-fallback.h for C23 features\n");
#else
    printf("Pre-C17 compiler; many features unavailable\n");
#endif

#if __has_c_attribute(deprecated)
    printf("[[deprecated]] attribute supported\n");
#else
    printf("[[deprecated]] attribute NOT supported; use fallback\n");
#endif
}

/* === 4.2 Attribute Fallback Pattern === */

#if __has_c_attribute(unsequenced)
    #define PURE_FUNC [[unsequenced]]
#elif defined(__GNUC__)
    #define PURE_FUNC __attribute__((__const__))
#else
    #define PURE_FUNC /* No support */
#endif

PURE_FUNC int square_pure(int x) {
    return x * x;
}

void demo_attribute_fallback(void) {
    printf("\n--- Attribute Fallback Pattern ---\n");
    printf("square_pure(7) = %d (pure function)\n", square_pure(7));
}

/* === 4.3 constexpr Emulation (Clang Workaround) === */

#if __is_identifier(constexpr)
# define constexpr static const
#endif

constexpr int MAGIC_NUMBER = 42;
constexpr double PI_APPROX = 3.141592653589793;

void demo_constexpr_emulation(void) {
    printf("\n--- constexpr Emulation ---\n");
    printf("MAGIC_NUMBER = %d\n", MAGIC_NUMBER);
    printf("PI_APPROX = %.15f\n", PI_APPROX);
}

/* === 4.4 Digital Separators (C23 Native Only) === */

void demo_digit_separators(void) {
    printf("\n--- Digit Separators (Requires C23 Compiler) ---\n");

    unsigned million = 1'000'000;
    unsigned rgb = 0xFF'00'FF;       // Magenta
    unsigned short bits = 0b1010'1100;

    printf("Readable constants:\n");
    printf("  1'000'000 = %u\n", million);
    printf("  0xFF'00'FF = 0x%X\n", rgb);
    printf("  0b1010'1100 = 0x%X\n", bits);
}

/* === 4.5 Header Availability Detection === */

void demo_optional_headers(void) {
    printf("\n--- Optional Header Detection ---\n");

#ifdef __has_include
# if __has_include(<stdbit.h>)
    printf("<stdbit.h> (bit utilities, C23) is available\n");
# else
    printf("<stdbit.h> is NOT available\n");
# endif

# if __has_include(<stdckdint.h>)
    printf("<stdckdint.h> (checked arithmetic, C23) is available\n");
# else
    printf("<stdckdint.h> is NOT available\n");
# endif

# if __has_include(<stdatomic.h>)
    printf("<stdatomic.h> (atomics, C11+) is available\n");
# else
    printf("<stdatomic.h> is NOT available\n");
# endif
#else
    printf("__has_include not supported (pre-C99 compiler)\n");
#endif
}

/* === 4.6 Guard for Optional Features (e.g., Threads) === */

#ifdef __STDC_NO_THREADS__
    printf("Threads not supported on this platform\n");
    typedef int thrd_t;
    #define thrd_create(thr, func, arg) -1
#else
    #include <threads.h>
#endif

void demo_optional_features(void) {
    printf("\n--- Optional Feature Detection ---\n");

#ifdef __STDC_NO_THREADS__
    printf("Threading: NOT supported\n");
#else
    printf("Threading: supported\n");
#endif

#ifdef __STDC_NO_ATOMICS__
    printf("Atomics: NOT supported\n");
#else
    printf("Atomics: supported\n");
#endif

#ifdef __STDC_NO_COMPLEX__
    printf("Complex numbers: NOT supported\n");
#else
    printf("Complex numbers: supported\n");
#endif
}

/* === 4.7 Compiler Detection === */

void demo_compiler_detection(void) {
    printf("\n--- Compiler Detection ---\n");

#ifdef __GNUC__
    printf("Compiler: GCC v%d.%d (or compatible)\n", __GNUC__, __GNUC_MINOR__);
#endif

#ifdef __clang__
    printf("Compiler: Clang v%d.%d\n", __clang_major__, __clang_minor__);
#endif

#ifdef _MSC_VER
    printf("Compiler: MSVC v%d\n", _MSC_VER);
#endif

#if defined(__unix__) || defined(__APPLE__)
    printf("Platform: Unix-like\n");
#elif defined(_WIN32) || defined(_WIN64)
    printf("Platform: Windows\n");
#else
    printf("Platform: Unknown\n");
#endif
}

/* === 4.8 Transitional Header Pattern === */

// Pattern for safe, portable code with gradual C23 migration
#if __STDC_VERSION__ >= 202311L
    // Native C23 features available
    #define USE_C23_NATIVE 1
#else
    // Use fallback compatibility layer
    #define USE_C23_NATIVE 0
#endif

void demo_migration_strategy(void) {
    printf("\n--- Transitional Migration Strategy ---\n");

#if USE_C23_NATIVE
    printf("Stage 3: Removing fallback header; native C23 active\n");
#else
    printf("Stage 1/2: Using c23-fallback.h for compatibility\n");
#endif
}

/* === 4.9 Phased Adoption Checklist === */

void print_adoption_checklist(void) {
    printf("\n--- C23 Adoption Checklist ---\n");

    printf("Migration Phases:\n");
    printf("  ☐ Phase 1: Include c23-fallback.h; test on target platforms\n");
    printf("  ☐ Phase 2: Incrementally update to C23 native features\n");
    printf("  ☐ Phase 3: Remove c23-fallback.h when platforms support C23\n\n");

    printf("Compiler Support Status (as of 2025):\n");

#if defined(__GNUC__) && __GNUC__ >= 10
    printf("  ✓ GCC 10+: Reasonable C23 support\n");
#else
    printf("  ⊘ GCC <10: Limited C23 support\n");
#endif

#if defined(__clang__) && __clang_major__ >= 14
    printf("  ✓ Clang 14+: Reasonable C23 support\n");
#else
    printf("  ⊘ Clang <14: Limited C23 support\n");
#endif

    printf("\nFeature Checklist:\n");
    printf("  %s Digit separators (0xAB'CD required)\n",
           "✓");
    printf("  %s Binary literals (0b1010 required)\n",
           "✓");
    printf("  %s Attribute syntax ([[attr]] required)\n",
           "✓");
}

int main(void) {
    printf("=== Chapter 23: Appendix A – Transitional Code ===\n");

    demo_feature_tests();
    demo_attribute_fallback();
    demo_constexpr_emulation();
    demo_digit_separators();
    demo_optional_headers();
    demo_optional_features();
    demo_compiler_detection();
    demo_migration_strategy();
    print_adoption_checklist();

    printf("\n=== End Chapter 23 ===\n");
    return 0;
}
