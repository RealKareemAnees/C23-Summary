# Chapter 14: Input, Output, and Text Processing

Source: 14-ioandtextprocessing.md

## 1. Exhaustive Concept Breakdown

### 1.1 Text Processing with Pointers

Text processing joins string functions, dynamic allocation, and pointers. A complete example parses lines of numbers:

```c
// Workflow:
// 1. Read line via fgetline() - detects truncation
// 2. Split line via numberline() - malloc array, parse via strtoull
// 3. Output via fprintnumbers() - format with snprintf safety
```

TAKEAWAY 14.1: The string strto... conversion functions are not const-safe (char\*\* parameter).

Key functions:

- **`strtoull(str, &next, base)`**: Converts string to unsigned long long; sets `next` to first non-digit.
- **`fgetline(size, buf, stream)`**: Reads line; null-terminates; detects truncation (missing newline).
- **`memchr(buf, byte, size)`**: Searches for byte in memory; returns pointer or null.

### 1.2 Formatted Input (scanf family)

The `scanf` family reads formatted input from streams/strings:

```c
int scanf(char const* format, ...);      // From stdin
int fscanf(FILE* stream, char const* format, ...);
int sscanf(char const* str, char const* format, ...);
```

**Format specifications**: `[flag][width][modifier]specifier`

| Specifier   | Type                      | Skip Space | Example Call                       |
| ----------- | ------------------------- | ---------- | ---------------------------------- |
| `d`         | `int*`                    | Yes        | `scanf("%d", &i)`                  |
| `i`         | `int*` (base auto)        | Yes        | `scanf("%i", &i)`                  |
| `u`         | `unsigned*`               | Yes        | `scanf("%u", &u)`                  |
| `x`         | `unsigned*` (hex)         | Yes        | `scanf("%x", &h)`                  |
| `b`         | `unsigned*` (binary, C23) | Yes        | `scanf("%b", &b)`                  |
| `f`/`e`/`g` | `double*`                 | Yes        | `scanf("%lg", &d)`                 |
| `c`         | `char*`                   | No         | `scanf("%c", &c)`                  |
| `s`         | `char*`                   | Yes        | `scanf("%s", buf)`                 |
| `[...]`     | `char*`                   | No         | `scanf("%[abc]", buf)`             |
| `p`         | `void**`                  | Yes        | `scanf("%p", &p)`                  |
| `n`         | `int*`                    | No         | `scanf("%n", &count)` (chars read) |

TAKEAWAY 14.2: The function interfaces for memchr and strchr are not const-safe.
TAKEAWAY 14.3: C23 type-generic interfaces for memchr/strchr search are const-safe.
TAKEAWAY 14.4: The strspn and strcspn functions are const-safe.

**Peculiarities**:

- Arguments are **pointers to target types**.
- Space in format matches any whitespace sequence (may be empty, multi-line).
- `%c` reads fixed count (default 1); `%s` reads non-whitespace and null-terminates.
- Type modifiers: `hh` (char), `h` (short), ``(default),`l`(long),`ll`(long long),`z`(size_t),`t` (ptrdiff_t).
- Assignment suppression: `%*d` skips an integer without assignment.

### 1.3 Buffer Operations: sprintf vs snprintf

**`sprintf(buf, format, ...)`**: Writes formatted output to string; no bounds checking.

TAKEAWAY 14.5: sprintf makes no provision against buffer overflow.

**`snprintf(buf, n, format, ...)`**: Bounded version; writes at most `n` bytes.

TAKEAWAY 14.6: Use snprintf when formatting output of unknown length.

Properties of `snprintf`:

- Buffer `buf` will not overflow.
- After successful call, `buf` is a null-terminated string.
- Calling with `n=0`, `buf=NULL` returns the length that would be written (without writing).

### 1.4 Extended Character Sets (Unicode, Multibyte Strings)

Standard `char` (8-bit) cannot represent all world scripts. C provides multibyte and wide-character support:

TAKEAWAY 14.7: Multibyte characters don't contain null bytes.
TAKEAWAY 14.8: Multibyte strings are null-terminated.

**Multibyte character (MBC)**: Sequence of bytes representing one character (UTF-8 example).
**Multibyte string (MBS)**: Null-terminated sequence of MBCs; compatible with standard C strings.
**Wide character (WC)**: Single `wchar_t` representing one Unicode code point.
**Wide character string (WCS)**: Null-terminated array of `wchar_t`.

**Unicode escape sequences** (in string/character literals):

- `\uXXXX` (4 hex digits): Unicode code point (U+0000 to U+FFFF).
- `\UXXXXXXXX` (8 hex digits): Extended code point (for > U+FFFF).

Example:

```c
#define COPYRIGHT "\u00A9"  // © symbol (U+00A9)
#define HBAR      "\u2500"  // ─ horizontal bar
```

**Setting locale** enables proper display:

```c
setlocale(LC_ALL, "");  // Switch to system locale
printf("Café: %s\n", "café");  // Works after setlocale
```

### 1.5 MBS/WCS Conversion Functions

`<wchar.h>` provides conversion utilities:

| Function                              | Header      | Purpose                          |
| ------------------------------------- | ----------- | -------------------------------- |
| `mbsrtowcs(wcs, mbs, len, state)`     | `<wchar.h>` | Convert MBS to WCS (restartable) |
| `mbrtowc(wc, mbs, len, state)`        | `<wchar.h>` | Convert single MBC to WC         |
| `wcsrtombs(mbs, wcs, len, state)`     | `<wchar.h>` | Convert WCS to MBS (restartable) |
| `mbsrtombs(mbs, mbs_src, len, state)` | `<wchar.h>` | MBS variant of mbsrtowcs         |

**`mbstate_t`**: Opaque state for parsing multibyte sequences; enables resumable parsing.

Return values:

- Size of output (excluding terminator) on success.
- `(size_t)-1` (mbinvalid) on invalid sequence; sets `errno = EILSEQ`.
- `(size_t)-2` (mbincomplete) when buffer too small for next MBC.

### 1.6 Binary Streams and Raw Data I/O

`<stdio.h>` provides binary stream functions:

| Function                           | Purpose                                 |
| ---------------------------------- | --------------------------------------- |
| `fread(buf, size, nmemb, stream)`  | Read `nmemb` objects of `size` bytes    |
| `fwrite(buf, size, nmemb, stream)` | Write `nmemb` objects of `size` bytes   |
| `fopen(filename, mode)`            | Open file; mode includes 'b' for binary |
| `fclose(stream)`                   | Close stream                            |
| `feof(stream)`                     | Test end-of-file condition              |
| `ferror(stream)`                   | Test error condition                    |

Text vs. binary modes (platform-dependent):

- **Text mode**: Newline conversion (`\n` ↔ CRLF on Windows).
- **Binary mode**: No conversion; byte-for-byte transfer.

Mode strings: `"r"` (read), `"w"` (write), `"a"` (append), `"+b"` for binary.

## 2. Syntax, Types, and Functions Dictionary

### Formatted I/O Functions

| Function                     | Header      | Purpose                          |
| ---------------------------- | ----------- | -------------------------------- |
| `scanf(fmt, ...)`            | `<stdio.h>` | Read formatted input from stdin  |
| `fscanf(stream, fmt, ...)`   | `<stdio.h>` | Read from stream                 |
| `sscanf(str, fmt, ...)`      | `<stdio.h>` | Read from string                 |
| `printf(fmt, ...)`           | `<stdio.h>` | Write formatted output to stdout |
| `fprintf(stream, fmt, ...)`  | `<stdio.h>` | Write to stream                  |
| `sprintf(buf, fmt, ...)`     | `<stdio.h>` | Write to string (UNSAFE)         |
| `snprintf(buf, n, fmt, ...)` | `<stdio.h>` | Write to string (bounded)        |

### String/Memory Functions

| Function                     | Header       | Purpose                              |
| ---------------------------- | ------------ | ------------------------------------ |
| `strtoull(str, &next, base)` | `<stdlib.h>` | Convert string to unsigned long long |
| `strtod(str, &next)`         | `<stdlib.h>` | Convert string to double             |
| `memchr(mem, byte, size)`    | `<string.h>` | Find byte in memory                  |
| `strchr(str, ch)`            | `<string.h>` | Find character in string             |
| `memcpy(dest, src, size)`    | `<string.h>` | Copy non-overlapping memory          |
| `memmove(dest, src, size)`   | `<string.h>` | Copy (may overlap)                   |
| `memset(mem, byte, size)`    | `<string.h>` | Fill memory with byte                |

### Multibyte/Wide Character Functions

| Function                          | Header       | Purpose                                 |
| --------------------------------- | ------------ | --------------------------------------- |
| `mbsrtowcs(wcs, mbs, len, state)` | `<wchar.h>`  | MBS → WCS (restartable)                 |
| `mbrtowc(wc, mbs, len, state)`    | `<wchar.h>`  | Single MBC → WC                         |
| `wcsrtombs(mbs, wcs, len, state)` | `<wchar.h>`  | WCS → MBS (restartable)                 |
| `wcstombs(mbs, wcs, len)`         | `<wchar.h>`  | WCS → MBS                               |
| `mbrlen(mbs, len, state)`         | `<wchar.h>`  | Length of first MBC                     |
| `setlocale(category, locale)`     | `<locale.h>` | Set locale (affects character handling) |

### Types and Constants

| Type/Macro  | Header       | Purpose                 |
| ----------- | ------------ | ----------------------- |
| `FILE`      | `<stdio.h>`  | Stream type             |
| `SIZE_MAX`  | `<stdint.h>` | Maximum size_t value    |
| `mbstate_t` | `<wchar.h>`  | Multibyte parsing state |
| `wchar_t`   | `<wchar.h>`  | Wide character type     |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 scanf Whitespace Ambiguity

**Pitfall**: Space in format string matches unlimited whitespace.

```c
scanf("%d %d", &a, &b);  // Space allows multiple lines between numbers
```

**Fix**: Be explicit; validate line structure; use `snprintf` for bounded reads.

### 3.2 String Overflow with scanf

**Pitfall**: `%s` reads unbounded input into fixed buffer.

```c
char buf[10];
scanf("%s", buf);  // Can overflow if input > 9 chars + null
```

**Fix**: Use bounded specifier: `scanf("%9s", buf)` (size-1 for null term).

### 3.3 Buffer Overflow with sprintf

**Pitfall**: sprintf assumes buffer is large enough.

```c
char buf[20];
sprintf(buf, "%s %s", very_long_string, another_string);  // CRASH
```

**Fix**: Use `snprintf(buf, sizeof buf, format, ...)`.

### 3.4 Invalid Multibyte Sequences

**Pitfall**: Malformed UTF-8 or encoding mismatch.

```c
char invalid[] = "\xFF\xFE";  // Invalid UTF-8
size_t result = mbsrtowcs(wcs, &invalid, 10, &state);
if (result == (size_t)-1) {
    // Conversion failed; errno == EILSEQ
}
```

**Fix**: Check return values; validate input encoding; use error modes.

### 3.5 Uninitialized Multibyte State

**Pitfall**: Using `mbstate_t` without initialization.

```c
mbstate_t state;  // Uninitialized!
mbrtowc(&wc, mbs, mblen, &state);  // UB
```

**Fix**: Zero-initialize: `mbstate_t state = {0};` or `calloc`.

## 4. Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>

/* === 4.1 Text Processing: Parse Numbers from Line === */

int parse_numbers(const char* line, int** nums_out) {
    printf("\n--- Parsing Numbers from Line ---\n");

    if (!line || !nums_out) return -1;

    // Allocate dynamic array
    int* nums = malloc(100 * sizeof *nums);
    if (!nums) return -1;

    int count = 0;
    char* copy = strdup(line);
    if (!copy) {
        free(nums);
        return -1;
    }

    // Parse space-separated integers
    char* saveptr = NULL;
    char* token = strtok_r(copy, " ", &saveptr);

    while (token && count < 100) {
        char* endp;
        long val = strtol(token, &endp, 10);
        if (endp != token) {  // Conversion succeeded
            nums[count++] = (int)val;
        }
        token = strtok_r(NULL, " ", &saveptr);
    }

    free(copy);
    *nums_out = nums;
    return count;
}

void demo_parse_numbers(void) {
    const char* line = "10 20 30 40 50";
    int* nums = NULL;
    int count = parse_numbers(line, &nums);

    printf("Parsed %d numbers: ", count);
    for (int i = 0; i < count; ++i) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    free(nums);
}

/* === 4.2 Formatted Input (scanf) === */

void demo_scanf(void) {
    printf("\n--- scanf (Formatted Input) ---\n");

    int id;
    double salary;
    char name[32];

    printf("Enter: ID (int) Salary (double) Name (string)\n");
    printf("> ");

    int result = scanf("%d %lg %31s", &id, &salary, name);

    if (result == 3) {
        printf("Parsed: ID=%d, Salary=$%.2f, Name=%s\n", id, salary, name);
    } else {
        printf("Parsing failed or incomplete (read %d fields)\n", result);
    }
}

/* === 4.3 Bounded String Formatting (snprintf) === */

void demo_snprintf(void) {
    printf("\n--- snprintf (Bounded Formatting) ---\n");

    char buf[20];
    const char* format = "Value: %d, Hex: %#x";

    // Query length needed
    int needed = snprintf(NULL, 0, format, 42, 255);
    printf("Space needed: %d bytes\n", needed);

    // Format with small buffer (truncates)
    int written = snprintf(buf, sizeof buf, format, 42, 255);
    printf("Buffer (size %zu): '%s'\n", sizeof buf, buf);
    printf("Actually needed: %d bytes\n", written);
}

/* === 4.4 Unicode and Multibyte Strings === */

void demo_unicode(void) {
    printf("\n--- Unicode Support ---\n");

    // Unicode escape sequences in string literals
    const char* symbols = "Copyright \u00A9 Trademark \u2122 Euros \u20AC";
    printf("Symbols: %s\n", symbols);

    // Direct Unicode characters in source (if system supports)
    const char* smiley = "Smile: 😊 (U+1F60A)";
    printf("Emoji: %s\n", smiley);
}

/* === 4.5 Multibyte to Wide Character Conversion === */

void demo_mbs_wcs(void) {
    printf("\n--- MBS/WCS Conversion ---\n");

    // Set locale for proper character handling
    setlocale(LC_ALL, "");

    const char* mbs = "Café";  // Multibyte string (UTF-8)
    wchar_t wcs[32];
    mbstate_t state = {0};

    size_t result = mbsrtowcs(wcs, &mbs, 32, &state);

    if (result != (size_t)-1) {
        printf("MBS: %s\n", mbs);  // Original invalid after conversion
        printf("WCS length: %zu characters\n", result);
        printf("First char code point: U+%04X\n", (unsigned)wcs[0]);
    } else {
        printf("Conversion failed\n");
    }
}

/* === 4.6 Memory Search (memchr, strchr) === */

void demo_memory_search(void) {
    printf("\n--- memchr and strchr ---\n");

    const char* str = "Hello, World!";

    // Find 'o' in string
    const char* pos = strchr(str, 'o');
    if (pos) {
        printf("Found 'o' at offset %td: '%s'\n", pos - str, pos);
    }

    // Find comma in memory
    const unsigned char* buf = (const unsigned char*)str;
    const unsigned char* comma_pos = (const unsigned char*)memchr(buf, ',', strlen(str));
    if (comma_pos) {
        printf("Found ',' at offset %td\n", comma_pos - buf);
    }
}

/* === 4.7 strtoull for Number Parsing === */

void demo_strtoull(void) {
    printf("\n--- strtoull (String to Number) ---\n");

    const char* inputs[] = { "0789", "1010b", "FF", "0" };
    int bases[] = { 0, 2, 16, 10 };  // base 0 = auto-detect

    for (size_t i = 0; i < sizeof inputs / sizeof *inputs; ++i) {
        char* endp;
        unsigned long long val = strtoull(inputs[i], &endp, bases[i]);
        printf("strtoull('%s', base %d) = %llu (parsed: %td chars)\n",
               inputs[i], bases[i], val, endp - inputs[i]);
    }
}

/* === 4.8 memcpy vs memmove === */

void demo_copy_functions(void) {
    printf("\n--- memcpy vs memmove ---\n");

    // Non-overlapping: either works
    int src[5] = { 1, 2, 3, 4, 5 };
    int dst[5];
    memcpy(dst, src, sizeof src);
    printf("memcpy (non-overlapping): ");
    for (int i = 0; i < 5; ++i) printf("%d ", dst[i]);
    printf("\n");

    // Overlapping: must use memmove
    int arr[10] = { 1, 2, 3, 4, 5, 0, 0, 0, 0, 0 };
    memmove(&arr[3], &arr[0], 5 * sizeof *arr);  // shift right
    printf("memmove (overlapping): ");
    for (int i = 0; i < 10; ++i) printf("%d ", arr[i]);
    printf("\n");
}

/* === 4.9 Character Classification (ctype) === */

void demo_char_classification(void) {
    printf("\n--- Character Classification ---\n");

    const char* test = "Hello123!";

    printf("Classification of '%s':\n", test);
    for (const char* p = test; *p; ++p) {
        printf("  '%c': alpha=%d digit=%d space=%d punct=%d\n",
               *p,
               isalpha((unsigned char)*p),
               isdigit((unsigned char)*p),
               isspace((unsigned char)*p),
               ispunct((unsigned char)*p));
    }
}

int main(void) {
    printf("=== Chapter 14: Input, Output, and Text Processing ===\n");

    demo_parse_numbers();
    // demo_scanf();  // Interactive; commented out
    demo_snprintf();
    demo_unicode();
    demo_mbs_wcs();
    demo_memory_search();
    demo_strtoull();
    demo_copy_functions();
    demo_char_classification();

    printf("\n=== End Chapter 14 ===\n");
    return 0;
}
```

**Key Takeaways from Example**:

- Dynamic parsing combines `malloc`, `strtol`/`strtoull`, and `strchr`/`memchr`.
- `snprintf` prevents overflow; query needed length with `NULL` buffer.
- `setlocale()` enables proper handling of multibyte/wide characters.
- `mbsrtowcs()` converts UTF-8 to wide chars; check for `(size_t)-1` errors.
- `memmove()` handles overlapping regions; `memcpy()` for non-overlapping only.
- Character classification (`isalpha`, `isdigit`, etc.) aids text parsing.
