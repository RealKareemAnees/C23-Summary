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

#if __has_c_attribute(unsequenced)
    printf("[[unsequenced]] attribute supported\n");
#else
    printf("[[unsequenced]] attribute NOT supported (fallback needed)\n");
#endif

#if __has_include(<stdatomic.h>)
    printf("<stdatomic.h> is available\n");
#else
    printf("<stdatomic.h> is NOT available\n");
#endif

#if __has_c_attribute(deprecated)
    printf("[[deprecated]] attribute supported\n");
#else
    printf("[[deprecated]] attribute NOT supported\n");
#endif
}

/* === 4.2 constexpr Fallback (C23) === */

#if __is_identifier(constexpr)
# define constexpr static const
#endif

constexpr int MAGIC_VALUE = 42;
constexpr double PI_APPROX = 3.141592653589793;

void demo_constexpr_fallback(void) {
    printf("\n--- constexpr Fallback ---\n");
    printf("MAGIC_VALUE = %d\n", MAGIC_VALUE);
    printf("PI_APPROX = %f\n", PI_APPROX);
}

/* === 4.3 Pure Function Attributes === */

#if __has_c_attribute(unsequenced)
    // C23 native attribute
    # define PURE_FUNC [[unsequenced]]
#elif defined(__GNUC__)
    // GCC attribute
    # define PURE_FUNC __attribute__((__const__))
#else
    // Fallback (no annotation)
    # define PURE_FUNC
#endif

PURE_FUNC int square(int x) {
    return x * x;
}

PURE_FUNC double absolute(double x) {
    return x < 0 ? -x : x;
}

void demo_pure_functions(void) {
    printf("\n--- Pure Function Attributes ---\n");
    printf("square(7) = %d\n", square(7));
    printf("absolute(-3.14) = %f\n", absolute(-3.14));
}

/* === 4.4 Digit Separators (C23) === */

void demo_digit_separators(void) {
    printf("\n--- Digit Separators (C23) ---\n");

    // Decimal with separators
    unsigned long million = 1'000'000;
    printf("1'000'000 = %lu\n", million);

    // Hexadecimal with separators
    unsigned int rgb = 0xFF'00'FF;  // Magenta
    printf("0xFF'00'FF (magenta) = 0x%X\n", rgb);

    // Binary with separators
    unsigned char bits = 0b1010'1100;
    printf("0b1010'1100 = 0x%02X\n", bits);
}

/* === 4.5 Binary Literals (C23) === */

void demo_binary_literals(void) {
    printf("\n--- Binary Literals (C23) ---\n");

    unsigned char flags = 0b1111'0000;
    printf("Flags: 0b1111'0000 = %d\n", flags);

    unsigned short mask = 0b1010'1010'1010'1010;
    printf("Mask: 0b1010'1010'1010'1010 = 0x%X\n", mask);
}

/* === 4.6 Header Availability Detection === */

void demo_header_detection(void) {
    printf("\n--- Header Availability Detection ---\n");

#ifdef __has_include
# if __has_include(<stdbit.h>)
    printf("<stdbit.h> (bit utilities) is available\n");
# else
    printf("<stdbit.h> is NOT available\n");
# endif

# if __has_include(<stdckdint.h>)
    printf("<stdckdint.h> (checked int) is available\n");
# else
    printf("<stdckdint.h> is NOT available\n");
# endif
#else
    printf("__has_include not supported\n");
#endif
}

/* === 4.7 Compiler Detection and Workarounds === */

void demo_compiler_detection(void) {
    printf("\n--- Compiler Detection ---\n");

#ifdef __GNUC__
    printf("Compiler: GCC (or compatible), version %d.%d\n", __GNUC__, __GNUC_MINOR__);
#endif

#ifdef __clang__
    printf("Compiler: Clang, version %d.%d\n", __clang_major__, __clang_minor__);
#endif

#if __STDC_VERSION__ >= 202311L
    printf("C23 fully supported\n");
#elif __STDC_VERSION__ >= 201710L
    printf("C17 supported; C23 fallback header needed\n");
#else
    printf("Pre-C17 compiler\n");
#endif
}

/* === 4.8 Platform and Environment Information === */

void demo_platform_info(void) {
    printf("\n--- Platform and Environment ---\n");

#ifdef _POSIX_VERSION
    printf("POSIX version: %ld\n", _POSIX_VERSION);
#endif

#ifdef __unix__
    printf("Running on Unix-like system\n");
#elif defined(_WIN32) || defined(_WIN64)
    printf("Running on Windows\n");
#else
    printf("Unknown platform\n");
#endif

#if defined(__LP64__) || defined(_WIN64)
    printf("64-bit platform\n");
#else
    printf("32-bit platform\n");
#endif

    printf("CHAR_BIT: %d bits per byte\n", CHAR_BIT);
}

/* === 4.9 Fallback Header Pattern === */

// Pattern for optional feature support:
#ifdef __STDC_NO_ATOMICS__
struct atomic_int {
    int value;
};
#define atomic_load(obj) ((obj)->value)
#define atomic_store(obj, val) ((obj)->value = (val))
typedef struct atomic_int _Atomic_int;
#else
#include <stdatomic.h>
typedef _Atomic(int) _Atomic_int;
#endif

void demo_optional_features(void) {
    printf("\n--- Optional Feature Handling ---\n");

#ifdef __STDC_NO_ATOMICS__
    printf("Atomics not supported; using fallback\n");
#else
    printf("Atomics supported natively\n");
#endif
}

/* === 4.10 Version and Feature Checklist === */

void print_c23_support_checklist(void) {
    printf("\n--- C23 Feature Support Checklist ---\n");

    printf("Language Features:\n");
    printf("  Digit separators: %s\n", "✓ (required)");
    printf("  Binary literals: %s\n", "✓ (required)");
    printf("  [[attribute]] syntax: %s\n", "✓ (required)");

    printf("\nOptional Extensions:\n");

#if __has_c_attribute(unsequenced)
    printf("  [[unsequenced]]: ✓ supported\n");
#else
    printf("  [[unsequenced]]: ✗ fallback to __attribute__\n");
#endif

#if __has_c_attribute(reproducible)
    printf("  [[reproducible]]: ✓ supported\n");
#else
    printf("  [[reproducible]]: ✗ fallback to __attribute__\n");
#endif

#if !__is_identifier(constexpr)
    printf("  constexpr: ✓ native support\n");
#else
    printf("  constexpr: ⊘ using static const fallback\n");
#endif

    printf("\nLibrary Headers:\n");

#ifdef __has_include
# if __has_include(<stdatomic.h>)
    printf("  <stdatomic.h>: ✓ available\n");
# else
    printf("  <stdatomic.h>: ✗ not available\n");
# endif

# if __has_include(<threads.h>)
    printf("  <threads.h>: ✓ available\n");
# else
    printf("  <threads.h>: ✗ not available\n");
# endif
#endif
}

int main(void) {
    printf("=== Chapter 22: Technical Annex – Compiler Support ===\n");

    demo_feature_tests();
    demo_constexpr_fallback();
    demo_pure_functions();
    demo_digit_separators();
    demo_binary_literals();
    demo_header_detection();
    demo_compiler_detection();
    demo_platform_info();
    demo_optional_features();
    print_c23_support_checklist();

    printf("\n=== End Chapter 22 ===\n");
    return 0;
}
