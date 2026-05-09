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
