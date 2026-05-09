# Chapter 5: Basic Values and Data

Source: 05-basicvaluesanddata.md

> Programs operate on values—abstract mathematical entities independent of representation. The C type system assigns types to values, determining representation, valid operations, and optimization opportunities. The abstract state machine model abstracts computation from platform details: binary representations (how values are encoded) vary by platform; operations are determined by value, type, and binary representation. Modern C distinguishes 18 base types organized into four classes: unsigned integers, signed integers, real floating-point, and complex floating-point. Type-semantic aliases (size_t, ptrdiff_t) provide portable abstractions. Literals specify compile-time values with implicit type deduction; explicit suffixes override defaults. Implicit type conversions reconcile mixed-type expressions within well-defined rules.

## 1. Exhaustive Concept Breakdown

### 1.1 The Abstract State Machine

A C program can be modeled as a state machine manipulating values. The program's state comprises:

- **Executable code** (the compiled program)
- **Current execution point** (instruction pointer)
- **Data** (values of all objects)
- **External input/output**

**Observable state**: The values of variables and program output at any execution moment. Optimization is valid only if observable states remain identical.

**TAKEAWAY 5.1**: C programs primarily reason about values, not their representation  
**TAKEAWAY 5.8**: Programs execute as if following the abstract state machine

### 1.2 Values: Abstract Entities

**TAKEAWAY 5.2**: All values are numbers or translate to numbers

A value is an abstract mathematical entity (0, 1, -5, 3.14, true, characters as numeric codes). Values exist independently of their representation. The number 12 has the same value whether written as decimal (12), hexadecimal (0xC), or binary (0b1100); the representation differs, not the value.

**Key principle**: Reason about values, not representations, for platform-independent code.

### 1.3 Types: Properties and Operations

Every value has a type, statically determined at compile-time.

**TAKEAWAY 5.3**: All values have a type that is statically determined  
**TAKEAWAY 5.4**: Possible operations on a value are determined by its type  
**TAKEAWAY 5.5**: A value's type determines the results of all operations

Arithmetic on `double` produces different results than identical arithmetic on `int`; the type determines semantics.

### 1.4 Binary Representations: Platform-Specific Encoding

**TAKEAWAY 5.6**: A type's binary representation determines the results of all operations  
**TAKEAWAY 5.7**: A type's binary representation is observable

The binary representation is how a given platform encodes values of a type. The C standard does not completely prescribe binary representations; it constrains them. For example:

- Precision of floating-point operations is implementation-defined
- Integer sizes depend on platform architecture
- Character encoding (ASCII, EBCDIC) is implementation-specific

**Model**: Binary representations are deterministic once chosen by the platform; the same operation on the same values always produces the same result.

### 1.5 Optimization and Abstract State

Modern compilers optimize aggressively, reordering computations as long as observable states are preserved.

**TAKEAWAY 5.9**: Type determines optimization opportunities

Example: `x = (x * 1.5) - y; printf("x is %g\n", x);`

Optimization possibilities:

1. If `x` is never used afterward, replace entire code with `printf("x is 4.5\n");`
2. If `x` is used later, reorder operations as long as observable effects remain identical
3. For signed/floating-point types, overflow may be an event that prevents certain optimizations (unsigned overflow is always safe)

### 1.6 Base Types: Four Main Classes

C has 18 base types organized into four classes:

**Class 1: Unsigned Integers** (unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long)

- Non-negative whole numbers [0, MAX]
- Arithmetic is modulo (SIZE_MAX + 1); overflow wraps predictably
- Most suitable for sizes, indices, counts

**Class 2: Signed Integers** (signed char, signed short, signed int, signed long, signed long long)

- Whole numbers including negatives
- Overflow behavior is implementation-defined (but well-defined in C99+)
- Used for quantities bearing sign or capable of negative values

**Class 3: Real Floating-Point** (float, double, long double)

- Fractional numbers; approximate representation
- IEEE 754 standard on most modern platforms
- Used for scientific computation, precision decimals

**Class 4: Complex Floating-Point** (float \_Complex, double \_Complex, long double \_Complex)

- Complex numbers (real + imaginary components)
- Requires `<complex.h>` header
- Support is optional (check `__STDC_NO_COMPLEX__`)

### 1.7 Type Ranks and Promotion

Types have **ranks** defining their relative sizes and precedence in arithmetic.

**TAKEAWAY 5.10**: Before arithmetic, narrow integers are promoted to signed int

Narrow types (bool, char, short) cannot be used directly in arithmetic; they're promoted to `signed int` before operations.

**TAKEAWAY 5.11**: Each of the four classes has three distinct unpromoted types

Type relationships within classes:

- Unsigned: _bool_ ⊂ _unsigned char_ ⊂ _unsigned short_ ⊂ _unsigned int_ ⊂ _unsigned long_ ⊂ _unsigned long long_
- Signed: _signed char_ ⊂ _signed short_ ⊂ _signed int_ ⊂ _signed long_ ⊂ _signed long long_

### 1.8 Semantic Type Aliases

Beyond the 18 base types, the standard defines semantic types for specific use cases:

| Type        | Header       | Meaning                                             |
| ----------- | ------------ | --------------------------------------------------- |
| `size_t`    | `<stddef.h>` | Unsigned; suitable for array sizes, counts, indices |
| `ptrdiff_t` | `<stddef.h>` | Signed; suitable for pointer differences            |
| `uintmax_t` | `<stdint.h>` | Widest unsigned integer                             |
| `intmax_t`  | `<stdint.h>` | Widest signed integer                               |
| `time_t`    | `<time.h>`   | Seconds since epoch (system-dependent precision)    |
| `clock_t`   | `<time.h>`   | Processor clock ticks                               |

**TAKEAWAY 5.12**: Use size_t for sizes, cardinalities, or ordinal numbers  
**TAKEAWAY 5.13**: Use unsigned for small quantities that can't be negative  
**TAKEAWAY 5.14**: Use signed for small quantities that bear a sign  
**TAKEAWAY 5.15**: Use ptrdiff_t for large differences that bear a sign  
**TAKEAWAY 5.16**: Use double for floating-point calculations  
**TAKEAWAY 5.17**: Use double complex for complex calculations

### 1.9 Literals: Specifying Compile-Time Values

**Literals** are expressions whose values are determined at compile-time.

**Decimal Integer Literals** (e.g., 123, 45, 0): Written in decimal notation. Default type is the first signed type that fits the value.

**TAKEAWAY 5.20**: Decimal integer literals are signed  
**TAKEAWAY 5.21**: A decimal integer literal has the first of three signed types that fits

Example: On a platform with 32-bit int range [-2³¹, 2³¹-1]:

- `2147483647` fits in `signed int`
- `2147483648` does not fit in `signed int`; type is `signed long`

**Octal Integer Literals** (e.g., 077): Prefixed with `0`; digits 0-7. Historical use; rarely used except for `0`.

**Hexadecimal Integer Literals** (e.g., 0xFFFF, 0XBEAF): Prefixed with `0x` or `0X`; digits [0-9a-fA-F].

**Binary Integer Literals** (e.g., 0b1010, 0B0101): C23 feature; prefixed with `0b`/`0B`; digits 0-1.

**TAKEAWAY 5.23**: Don't use binary, octal, or hexadecimal literals for negative values  
**TAKEAWAY 5.24**: Use decimal literals for negative values

For non-decimal (0b, 0x, 0), if the value exceeds signed ranges, the type becomes unsigned.

**Floating-Point Literals** (e.g., 1.7E-13, 0x1.7aP-13):

- Decimal: mantissa with optional decimal point, optional exponent (E notation): mEe = m × 10ᵉ
- Hexadecimal: `0x` prefix, hex fraction `.`, hex exponent (P notation): 0XhPe = h × 2ᵉ

Hexadecimal floats precisely represent values that have exact binary representations; decimal floats are approximated.

**TAKEAWAY 5.26**: The effective value of a decimal floating-point literal may differ from its literal value

Example: `0.2` cannot be exactly represented in binary; computed value ≠ literal value.

**Character Literals** (e.g., 'a', '?', '\n'): Single characters in single quotes. Escape sequences like `\n`, `\t` represent special characters.

**String Literals** (e.g., "hello"): Sequences of characters in double quotes.

**TAKEAWAY 5.18**: Consecutive string literals are concatenated

```c
"hello "
"world"
```

is equivalent to `"hello world"`.

### 1.10 Literal Type Suffixes

Suffixes override default type deduction:

| Suffix       | Type Effect                               |
| ------------ | ----------------------------------------- |
| `L` or `l`   | At least `long` (for integers)            |
| `LL` or `ll` | `long long` (for integers)                |
| `U` or `u`   | Force `unsigned` (for integers)           |
| `F` or `f`   | `float` (for floats; default is `double`) |
| `L` or `l`   | `long double` (for floats)                |
| `WB` or `wb` | Bit-precise type (C23 feature)            |

Examples:

- `1U` → type `unsigned`, value 1
- `1L` → type `signed long`, value 1
- `1ULL` → type `unsigned long long`, value 1
- `0.5F` → type `float`, value 0.5
- `0.5L` → type `long double`, value 0.5

### 1.11 Complex Constants (Advanced)

Complex type support requires `<complex.h>`.

**CMPLX macro**: `CMPLX(real_part, imag_part)` → complex value

**I constant**: Represents imaginary unit (I² = -1). Can be used in expressions: `0.5 + 0.5*I` → complex value.

**TAKEAWAY 5.28**: I is reserved for the imaginary unit

### 1.12 Properties of Literals

**TAKEAWAY 5.19**: Numerical literals are never negative

The minus sign in `-34` is the unary negation operator applied to literal `34`, not part of the literal itself.

**TAKEAWAY 5.25**: Different literals can have the same value

Example: `0`, `0x0`, `'\0'` all represent zero.

**TAKEAWAY 5.27**: Literals have value, type, and binary representations

The C standard defines the semantics of literal values and their types precisely.

## 2. Syntax, Types & Functions Dictionary

| Entity                               | Type/Category                       | Signature                    | Parameters            | Return                                               |
| ------------------------------------ | ----------------------------------- | ---------------------------- | --------------------- | ---------------------------------------------------- |
| `size_t`                             | Semantic unsigned integer type      | Unsigned                     | —                     | Represents sizes, indices, counts                    |
| `ptrdiff_t`                          | Semantic signed integer type        | Signed                       | —                     | Represents signed differences of pointers            |
| `double`                             | Real floating-point base type       | 8 bytes (typically)          | —                     | Fractional numbers with ~15 decimal digits           |
| `int` / `signed int`                 | Signed integer base type            | 4 bytes (typically)          | —                     | Signed whole numbers                                 |
| `unsigned` / `unsigned int`          | Unsigned integer base type          | 4 bytes (typically)          | —                     | Unsigned whole numbers                               |
| `bool` / `_Bool`                     | Boolean type (C23 built-in)         | 1 byte                       | —                     | Stores true (1) or false (0)                         |
| `char`                               | Character base type                 | 1 byte                       | —                     | Stores character code; signedness varies by platform |
| `short` / `signed short`             | Short signed integer                | 2 bytes (typically)          | —                     | Signed; smaller than int                             |
| `long` / `signed long`               | Long signed integer                 | ≥ 4 bytes                    | —                     | Signed; ≥ sizeof(int)                                |
| `long long` / `signed long long`     | Long long signed integer            | ≥ 8 bytes                    | —                     | Signed; widest standard                              |
| `unsigned char`                      | Unsigned character type             | 1 byte                       | —                     | Unsigned; suitable for byte data                     |
| `unsigned short`                     | Short unsigned integer              | 2 bytes (typically)          | —                     | Unsigned; smaller than unsigned int                  |
| `unsigned long`                      | Long unsigned integer               | ≥ 4 bytes                    | —                     | Unsigned; ≥ sizeof(unsigned int)                     |
| `unsigned long long`                 | Long long unsigned integer          | ≥ 8 bytes                    | —                     | Unsigned; widest standard                            |
| `float`                              | Single-precision floating-point     | 4 bytes (typically)          | —                     | ~6 decimal digits precision                          |
| `long double`                        | Extended precision floating-point   | ≥ 10 bytes                   | —                     | Platform-specific extended precision                 |
| `float _Complex` (`float complex`)   | Single-precision complex            | 8 bytes (typically)          | —                     | Real + imaginary parts (float each)                  |
| `double _Complex` (`double complex`) | Double-precision complex            | 16 bytes (typically)         | —                     | Real + imaginary parts (double each)                 |
| `long double _Complex`               | Extended-precision complex          | ≥ 20 bytes                   | —                     | Real + imaginary parts (long double each)            |
| `123`                                | Decimal integer literal             | Signed                       | —                     | Value 123; type fits smallest signed                 |
| `0xFFFF`                             | Hexadecimal literal                 | Unsigned if > largest signed | —                     | 65535; type depends on value                         |
| `0b1010`                             | Binary literal (C23)                | Unsigned if > largest signed | —                     | 10; type depends on value                            |
| `3.14`                               | Decimal float literal               | `double`                     | —                     | Value ≈ 3.14 (binary representation)                 |
| `0x1.0P0`                            | Hexadecimal float literal           | `double`                     | —                     | Exactly 1.0 (binary-precise notation)                |
| `'a'`                                | Character literal                   | `int`                        | —                     | ASCII/encoding value of character                    |
| `"hello"`                            | String literal                      | `const char*`                | —                     | Pointer to zero-terminated string                    |
| `1U` / `1u`                          | Unsigned integer suffix             | `unsigned`                   | —                     | Forces type to `unsigned int`                        |
| `1L` / `1l`                          | Long integer suffix                 | `long`                       | —                     | Forces type to `signed long`                         |
| `1LL` / `1ll`                        | Long long suffix                    | `long long`                  | —                     | Forces type to `signed long long`                    |
| `0.5F` / `0.5f`                      | Float suffix                        | `float`                      | —                     | Forces type to `float`                               |
| `0.5L` / `0.5l`                      | Long double suffix                  | `long double`                | —                     | Forces type to `long double`                         |
| `CMPLX(r, i)`                        | Complex macro (from `<complex.h>`)  | Complex                      | Real, imaginary parts | `double complex` value                               |
| `I`                                  | Imaginary unit (from `<complex.h>`) | `float complex`              | —                     | I² = -1; basis for complex literals                  |
| `SIZE_MAX`                           | Maximum size_t value                | `size_t` constant            | —                     | Defined in `<stdint.h>`                              |

## 3. The "Danger Zone": UB, Pitfalls, and Warnings

### 3.1 Floating-Point Precision and Approximate Values

**Problem**: Decimal floating-point literals cannot always be exactly represented.

**Example**: `0.2` has no exact binary representation; computed value ≈ 0.200...0001... (or similar).

**Consequence**: Comparisons `if (x == 0.2)` may fail due to precision drift; accumulating small floating-point errors produce wrong results.

**Mitigation**: Use hexadecimal float literals for exact values; use epsilon comparisons: `if (fabs(x - 0.2) < epsilon)`.

### 3.2 Integer Literal Overflow

**Problem**: Assuming a large decimal literal fits in a signed type, when it actually overflows.

**Example**: On 32-bit systems, `2147483648` exceeds `int` range; type becomes `signed long`.

**Consequence**: Expression type is unexpected; arithmetic with this value may be unexpected.

**Mitigation**: Use explicit suffixes to force intended types; understand literal deduction rules.

### 3.3 Negative Literal Confusion

**Problem**: `-1` is not a literal with value -1; it's the negation operator applied to literal `1`.

**Example**: Type of `-1` is `signed int`; type of `-1U` is `unsigned int`.

**Consequence**: Unexpected type conversions; non-portable behavior across platforms.

**Mitigation**: Use decimal literals for negative values; use explicit casts when platform-specific behavior matters.

### 3.4 Implicit Type Conversion and Data Loss

**Problem**: Assigning a value of one type to a variable of incompatible type, causing automatic (implicit) conversion.

**Example**: `unsigned char c = 1000;` truncates 1000 to 232 (1000 % 256).

**Consequence**: Data loss; logic errors.

**Mitigation**: Ensure type compatibility; use explicit casts when conversion is intentional; enable compiler warnings (`-Wconversion`).

### 3.5 Character Encoding Assumptions

**Problem**: Assuming character literals have specific ASCII values.

**Example**: `char c = 'A';` assumes `'A'` is 65 (ASCII).

**Consequence**: Non-portable code; fails on non-ASCII platforms (EBCDIC, etc.).

**Mitigation**: Use standard functions (`isalpha()`, `isdigit()`) instead of assuming raw values; document assumptions.

### 3.6 Complex Number Support

**Problem**: Complex type support is optional; code using complex may not compile on all platforms.

**Mitigation**: Check `__STDC_NO_COMPLEX__` macro; use conditional compilation; provide fallback code.

### 3.7 Mixing Unsigned and Signed in Comparisons

**Problem**: Comparing unsigned and signed values; implicit conversion rules may produce unexpected results.

**Example**: `-1 < 1U` compares `-1` (signed) with `1U` (unsigned); `-1` is implicitly converted to large unsigned value.

**Consequence**: Comparison yields unexpected result.

**Mitigation**: Avoid mixing signed/unsigned; explicitly cast to intended type.

### 3.8 String Literal Concatenation Pitfalls

**Problem**: Misunderstanding string concatenation; incomplete strings or incorrect formatting.

**Example**: `"hello" "world"` concatenates to `"helloworld"` (no space).

**Consequence**: Incorrect output; formatting errors.

**Mitigation**: Explicitly include spaces or separators in strings; understand concatenation semantics.

### 3.9 Escape Sequence Pitfalls

**Problem**: Mistyping escape sequences or using unsupported sequences.

**Example**: `"\d"` (invalid escape) behavior is implementation-defined.

**Consequence**: Compiler warnings or undefined characters in strings.

**Mitigation**: Use only standard escape sequences (`\n`, `\t`, `\\`, `\"`, `\'`, etc.); enable warnings.

### 3.10 Type size and Portability

**Problem**: Assuming specific sizes for types (e.g., `int` is always 32-bit).

**Consequence**: Non-portable code; fails on platforms with different type sizes.

**Mitigation**: Use `sizeof()` to query actual sizes; use semantic types (`size_t`, `uint32_t`) when sizes matter.

## 4. Comprehensive C23 Working Example

```c
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
```
