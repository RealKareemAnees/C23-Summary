#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h>

// ============================================================
// Comprehensive Example: Types, Values, Literals, and Conversions
// ============================================================
// Demonstrates:
// 1. Base types (signed, unsigned, floating-point, bool, complex)
// 2. Semantic type aliases (size_t, ptrdiff_t)
// 3. Literal specifications and type deduction
// 4. Literal suffixes (U, L, F, etc.)
// 5. Implicit type conversions
// 6. Binary representations and sizes
// 7. Complex numbers
// 8. Abstract state machine principles

int main(void) {
    // ========================================
    // Section 1: Unsigned Integer Types
    // ========================================

    printf("=== Unsigned Integer Types ===\n");

    unsigned char uc = 255;      // Max for unsigned char
    unsigned short us = 65535;   // Max for unsigned short (typically)
    unsigned int ui = 4294967295UL;  // Max for unsigned int (typically, explicit suffix)
    size_t sz = sizeof(size_t) == 8 ? UINT64_MAX : UINT32_MAX;  // Platform-dependent max

    printf("unsigned char:  %hhu (size: %zu bytes)\n", uc, sizeof(unsigned char));
    printf("unsigned short: %hu (size: %zu bytes)\n", us, sizeof(unsigned short));
    printf("unsigned int:   %u (size: %zu bytes)\n", ui, sizeof(unsigned int));
    printf("size_t:         %zu (size: %zu bytes) SIZE_MAX=%zu\n",
           sz, sizeof(size_t), (size_t)-1);

    // ========================================
    // Section 2: Signed Integer Types
    // ========================================

    printf("\n=== Signed Integer Types ===\n");

    signed char sc = -128;         // Min for signed char
    signed short ss = -32768;      // Min for signed short (typically)
    signed int si = -2147483648LL; // Min for signed int (typically, literal with suffix)
    ptrdiff_t pd = -999;           // Signed difference type

    printf("signed char:  %hhd (range: %d to %d)\n", sc, -128, 127);
    printf("signed short: %hd (size: %zu bytes)\n", ss, sizeof(signed short));
    printf("signed int:   %d (size: %zu bytes)\n", si, sizeof(signed int));
    printf("ptrdiff_t:    %td (size: %zu bytes)\n", pd, sizeof(ptrdiff_t));

    // ========================================
    // Section 3: Floating-Point Types
    // ========================================

    printf("\n=== Floating-Point Types ===\n");

    float f = 3.14F;          // Float suffix 'F'
    double d = 3.14;          // Default double
    long double ld = 3.14L;   // Long double suffix 'L'

    printf("float:       %.6f (size: %zu bytes)\n", f, sizeof(float));
    printf("double:      %.15f (size: %zu bytes)\n", d, sizeof(double));
    printf("long double: %.18Lf (size: %zu bytes)\n", ld, sizeof(long double));

    // Decimal float ≠ exact binary representation
    printf("\nDecimal literal 0.2: %.20f\n", 0.2);    // Not exactly 0.2
    // Hexadecimal exact representation
    printf("Hex literal 0x1.0P0: %.1f (exactly 1.0)\n", 0x1.0P0);

    // ========================================
    // Section 4: Boolean Type
    // ========================================

    printf("\n=== Boolean Type ===\n");

    bool b_false = false;
    bool b_true = true;

    printf("false: %d (size: %zu bytes)\n", b_false, sizeof(bool));
    printf("true:  %d (size: %zu bytes)\n", b_true, sizeof(bool));

    // Bool from scalar value (0 = false, non-zero = true)
    bool b_from_int = 42;  // true
    printf("bool from 42: %d\n", b_from_int);

    // ========================================
    // Section 5: Character Type
    // ========================================

    printf("\n==== Character Type ===\n");

    char ch_a = 'a';
    char ch_newline = '\n';  // Escape sequence
    char ch_null = '\0';     // Null terminator

    printf("'a': %c (code: %d)\n", ch_a, ch_a);
    printf("'\\n': (newline, code: %d)\n", ch_newline);
    printf("'\\0': (null, code: %d)\n", ch_null);

    // ========================================
    // Section 6: Literal Specifications
    // ========================================

    printf("\n=== Literal Specifications and Type Deduction ===\n");

    // Integer literals with suffixes
    int lit_decimal = 123;           // Decimal, type: signed int
    unsigned ui_suffix = 123U;       // Unsigned suffix
    long li_suffix = 123L;           // Long suffix
    unsigned long ul_suffix = 123UL; // Unsigned long

    printf("123:   type size depends on platform (assume signed int)\n");
    printf("123U:  type unsigned int\n");
    printf("123L:  type signed long\n");
    printf("123UL: type unsigned long\n");

    // Hexadecimal literal deduction
    unsigned int hex_small = 0x7F;      // Fits in signed, but we use unsigned
    unsigned int hex_large = 0xFFFFFFFFU; // Exceeds signed range; suffix needed

    printf("\n0x7F:       value=%u, type=%s\n", hex_small, "unsigned");
    printf("0xFFFFFFFFU: value=%u, type=%s\n", hex_large, "unsigned int");

    // Binary literals (C23)
    unsigned int bin = 0b1010;
    printf("0b1010: value=%u\n", bin);

    // Floating-point literals
    double d1 = 1.5;          // Decimal float
    double d2 = 0x1.8P0;      // Hexadecimal (exactly 1.5 = 1 + 0.5)
    float f1 = 1.5F;          // Float suffix

    printf("\n1.5 (decimal):    %.10f\n", d1);
    printf("0x1.8P0 (hex):    %.10f (exactly 1.5 in binary)\n", d2);
    printf("1.5F (float):     %.6f\n", f1);

    // ========================================
    // Section 7: Complex Numbers
    // ========================================

    printf("\n=== Complex Numbers ===\n");

    double complex c1 = 3.0 + 4.0*I;  // Using imaginary unit I
    double complex c2 = CMPLX(3.0, 4.0);  // Using CMPLX macro

    printf("3 + 4i: real=%.1f, imag=%.1f\n", creal(c1), cimag(c1));
    printf("CMPLX(3, 4): real=%.1f, imag=%.1f\n", creal(c2), cimag(c2));

    double complex c_sum = c1 + c2;  // Complex arithmetic
    printf("(3+4i) + (3+4i) = %.1f + %.1fi\n", creal(c_sum), cimag(c_sum));

    // ========================================
    // Section 8: String Literal Concatenation
    // ========================================

    printf("\n=== String Literal Concatenation ===\n");

    const char* str1 = "Hello " "world";  // Concatenated by compiler
    const char* str2 = "Multi-line "
                       "string literal";   // Concatenated across lines

    printf("Concatenated: %s\n", str1);
    printf("Multi-line: %s\n", str2);

    // ========================================
    // Section 9: Type Sizes and Binary Representation
    // ========================================

    printf("\n=== Type Sizes and Binary Representation ===\n");

    printf("Type sizes (in bytes):\n");
    printf("  bool:              %zu\n", sizeof(bool));
    printf("  char:              %zu\n", sizeof(char));
    printf("  short:             %zu\n", sizeof(short));
    printf("  int:               %zu\n", sizeof(int));
    printf("  long:              %zu\n", sizeof(long));
    printf("  long long:         %zu\n", sizeof(long long));
    printf("  float:             %zu\n", sizeof(float));
    printf("  double:            %zu\n", sizeof(double));
    printf("  size_t:            %zu\n", sizeof(size_t));
    printf("  ptrdiff_t:         %zu\n", sizeof(ptrdiff_t));
    printf("  double complex:    %zu\n", sizeof(double complex));

    // ========================================
    // Section 10: Implicit Type Conversion
    // ========================================

    printf("\n=== Implicit Type Conversions ===\n");

    int signed_val = -1;
    unsigned uns_val = 1;

    // Signed -1 converted to unsigned (large positive number)
    printf("Signed -1 converted to unsigned: %u\n", (unsigned)signed_val);

    // Comparison: -1 vs 1U (implicit conversion of -1)
    if (signed_val < uns_val) {
        printf("-1 < 1U: comparison after implicit conversion\n");
    }

    // Float to int conversion (truncation, not rounding)
    double d_val = 3.9;
    int i_val = d_val;  // Truncates to 3
    printf("3.9 converted to int: %d\n", i_val);

    // ========================================
    // Section 11: Abstract State Machine
    // ========================================

    printf("\n=== Abstract State Machine Principle ===\n");

    // The program's state at any moment is determined by:
    // 1. Executable code (same)
    // 2. Execution point (here: main function)
    // 3. Data (values of all variables)
    // 4. External I/O (output to terminal)

    double x = 5.0;
    x = (x * 1.5) - 3.0;  // Computation
    printf("x after (x*1.5) - 3.0: %.1f\n", x);

    // Compiler may optimize this to: printf("x: 4.5\n");
    // ...if x is not used afterward (observable state is identical)

    printf("\n=== Program completed successfully ===\n");
    return EXIT_SUCCESS;
}

// ============================================================
// Compilation:
// ============================================================
// gcc -std=c23 -Wall -O0 -o types_and_values types_and_values.c -lm
// clang -std=c23 -Wall -O0 -o types_and_values types_and_values.c -lm
//
// Execution:
// ./types_and_values
//
// Key Concepts Demonstrated:
// ============================================================
// 1. All 18 base types (unsigned, signed, float, complex, bool, char)
// 2. Semantic type aliases (size_t, ptrdiff_t)
// 3. Literal specification and type deduction
// 4. Literal suffixes (U, L, UL, F, etc.)
// 5. Floating-point precision limitations
// 6. Complex number constants and arithmetic
// 7. String concatenation at compile-time
// 8. Type sizes and binary representation
// 9. Implicit type conversions
// 10. Abstract state machine (values, types, representation)
// 11. Optimization constraints (observable state preservation)
