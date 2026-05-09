# Chapter 25: Appendix C – C Libraries

Source: 25-appendixcclibraries.md

## 1. Exhaustive Concept Breakdown

### 1.1 Library Support Status for C23

**Current reality** (as of 2025): C library implementations lag behind compiler language features.

TAKEAWAY C.1: C library support for C23 is incomplete in most distributions.

**Situation**:

- glibc 2.39+ provides substantial C23 library support.
- Older glibc, musl, BSD libc have limited C23 support.
- Some platforms still use legacy C library implementations.

### 1.2 Functions Harmonized from POSIX

**Source**: POSIX and similar systems (Linux, macOS, BSD).

**Harmonized functions** (already available on Unix-like systems):

- `strftime()` – Format time to string
- `gmtime_r()` – Convert seconds to UTC time (reentrant)
- `localtime_r()` – Convert seconds to local time (reentrant)
- `memccpy()` – Copy memory until character found
- `strdup()` – Allocate and copy string
- `strndup()` – Allocate and copy string (limited length)
- `timegm()` – Convert broken-down time to seconds (non-standard but common)

**Note**: These functions likely already exist on target POSIX platforms; no adoption effort needed.

### 1.3 UTF-8 String Functions (C23)

**New functions** introduce UTF-8 support:

- `mbrtoc8()` – Convert multibyte sequence to UTF-8 character
- `c8rtomb()` – Convert UTF-8 character to multibyte sequence

**Equivalent to existing**: `mbrtoc32()` and `c32rtomb()` (but for UTF-8 instead of UTF-32).

**Challenge**: UTF-8 encoding is sophisticated; proper implementation requires deep understanding of Unicode and encoding schemes.

**Exercise opportunity**: Implementing these functions is valuable learning experience.

### 1.4 Bit Utilities (`<stdbit.h>`)

**Header**: New C23 addition.

**Purpose**: Type-generic interfaces for bit manipulation on unsigned integer types.

**Examples**:

- Bit counting: `popcount(x)` – Count set bits
- Bit rotation, shifting, scanning
- Type-generic wrappers hiding `_u`, `_ul`, `_ull` suffixes

**Compiler support**: GCC, Clang provide as built-ins; fallback header can expose.

**Complete support**: glibc 2.39+

### 1.5 Checked Integer Arithmetic (`<stdckdint.h>`)

**Header**: New C23 addition.

**Purpose**: Type-generic macros for safe integer operations detecting overflow.

**Examples**:

- `ckd_add(result, a, b)` – Checked addition
- `ckd_sub(result, a, b)` – Checked subtraction
- `ckd_mul(result, a, b)` – Checked multiplication

**Benefit**: Prevents silent wraparound; detects errors.

**Support**: glibc 2.39+; fallback headers available.

### 1.6 Formatted I/O Enhancements

**Printf/scanf extensions** (C23):

**New format specifiers**:

- `%w` – Width for extended integer types (e.g., `%w128` for 128-bit)
- `%wf` – Similar for floating-point
- `%b`, `%B` – Binary output (`0b1010`)

**Support**:

- glibc 2.39+: Supports standard integers; 128-bit support incomplete
- Compilers: May issue warnings with `%w128` specifiers (cosmetic but annoying)

**Limitation**: No support for 128-bit types in most libraries yet.

### 1.7 Mathematical Functions (`<math.h>`)

**New C23 functions** (many for specialized use):

**Key additions**:

- `exp2()`, `exp10()` – Powers of 2 and 10
- `log2()`, `log10()` – Logarithms
- `fma()` – Fused multiply-add
- "Pi functions" – Trigonometric with half-revolution units (π-based)

**CORE-MATH project**: Provides optimized implementations of missing math functions.

Reference: https://core-math.gitlabpages.inria.fr/

**Status**: Complete implementations will take time to propagate through distributions.

### 1.8 Reference Implementation: musl libc

**For Linux users**: musl libc patches (via build-musl script in book examples) provide:

- Full C23 `[u]int128_t` support (with proper alignment and calling conventions).
- Extended type support on architectures with GCC's `__int128` extension.

**Caveats**:

- Patches may be experimental; not all thoroughly tested.
- Integration into mainstream musl depends on review and acceptance.

**Access**: Via build-musl script in provided sample code directory.

## 2. Syntax, Types, and Functions Dictionary

### POSIX-Harmonized Functions

| Function        | Header       | Purpose                                |
| --------------- | ------------ | -------------------------------------- |
| `strftime()`    | `<time.h>`   | Format time to string (POSIX standard) |
| `gmtime_r()`    | `<time.h>`   | Time to UTC (reentrant, POSIX)         |
| `localtime_r()` | `<time.h>`   | Time to local (reentrant, POSIX)       |
| `memccpy()`     | `<string.h>` | Copy until character (POSIX)           |
| `strdup()`      | `<string.h>` | Allocate and copy string (POSIX)       |
| `strndup()`     | `<string.h>` | Allocate and copy (limited, POSIX)     |
| `timegm()`      | `<time.h>`   | Time to seconds (POSIX-like, non-std)  |

### New C23 Functions

| Function                                 | Header          | Purpose                         |
| ---------------------------------------- | --------------- | ------------------------------- |
| `mbrtoc8()`                              | `<uchar.h>`     | Multibyte to UTF-8 character    |
| `c8rtomb()`                              | `<uchar.h>`     | UTF-8 character to multibyte    |
| `popcount()` (type-generic)              | `<stdbit.h>`    | Count set bits                  |
| `ckd_add()`                              | `<stdckdint.h>` | Checked addition                |
| `ckd_sub()`                              | `<stdckdint.h>` | Checked subtraction             |
| `ckd_mul()`                              | `<stdckdint.h>` | Checked multiplication          |
| `exp2()`, `exp10()`, `log2()`, `log10()` | `<math.h>`      | Exponential/logarithm functions |
| `fma()`                                  | `<math.h>`      | Fused multiply-add              |

### New Headers (C23)

| Header          | Purpose                    | Status      |
| --------------- | -------------------------- | ----------- |
| `<stdbit.h>`    | Bit utilities              | glibc 2.39+ |
| `<stdckdint.h>` | Checked integer arithmetic | glibc 2.39+ |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Assuming Universal UTF-8 Function Availability

**Pitfall**: Using `mbrtoc8()` and `c8rtomb()` assuming all platforms support them.

```c
// May fail on older glibc or non-POSIX systems
#include <uchar.h>
mbrtoc8(pc8, mbstr, n, pstate);
```

**Fix**: Check platform; provide fallback or encoding alternative.

### 3.2 Relying on Unsupported Printf Format Specifiers

**Pitfall**: Using `%w128` in format string expecting compiler/library support.

```c
int128_t big = 0;
printf("%w128d\n", big);  // May fail or issue warnings (2025)
```

**Fix**: Cast to supported type; use fallback formatting for 128-bit values.

### 3.3 Missing Implementations in Older glibc

**Pitfall**: Calling new C23 functions on systems with glibc <2.39.

```c
// __builtin_popcount exists, but <stdbit.h> may not
#include <stdbit.h>
unsigned count = popcount(x);  // Link error or unavailable
```

**Fix**: Check glibc version; provide static implementation or fallback.

### 3.4 Incomplete 128-Bit Support in Printf/Scanf

**Pitfall**: Using `%w128` expecting full support; compilers warn, libraries don't support.

```c
uint128_t val = ...;
printf("%w128u\n", val);  // Compiler warning; library may not format correctly
```

**Fix**: Serialize manually or wait for glibc/compiler updates.

### 3.5 ABI and Calling Convention Issues

**Pitfall**: Mixing code compiled with different 128-bit type handling.

```bash
gcc prog.c -o prog     # Uses GCC 128-bit conventions
clang prog.c -o prog   # Uses Clang 128-bit conventions (pre-18: incompatible)
```

**Result**: Crashes or data corruption.

**Fix**: Use consistent compiler; upgrade Clang to 18+.

## 4. Comprehensive C23 Working Example

```c
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
```

**Key Takeaways**:

- C library support for C23 lags compiler adoption; glibc 2.39+ provides substantial coverage.
- POSIX-harmonized functions already exist on Unix-like systems; minimal adoption effort needed.
- UTF-8, bit utilities, and checked arithmetic are new C23 features; implementations still propagating.
- Printf/scanf format extensions (`%w128`) have incomplete support across libraries and compilers.
- For bleeding-edge C23 library support on Linux, use musl with C23 patches.
