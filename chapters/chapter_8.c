#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <stdckdint.h>
#include <stdbit.h>
#include <ctype.h>
#include <errno.h>

/* ============================================================ */
/* 1. ERROR HANDLING PATTERNS */
/* ============================================================ */

void demo_error_handling(void) {
    printf("=== Error Handling Strategies ===\n");

    /* Pattern 1: Null pointer return */
    FILE* f = fopen("/nonexistent", "r");
    if (!f) {
        perror("fopen failed:");
        printf("(This is expected; continuing...)\n");
    } else {
        fclose(f);
    }

    /* Pattern 2: Special error code (EOF) */
    const char* test_str = "hello";
    int result = puts(test_str);
    if (result == EOF) {
        perror("puts failed");
    } else {
        printf("(puts returned %d, success)\n", result);
    }
}

/* ============================================================ */
/* 2. PREPROCESSOR CONDITIONALS & COMPILE-TIME CHECKS */
/* ============================================================ */

void demo_preprocessor_checks(void) {
    printf("\n=== Preprocessor Conditionals ===\n");

    #ifdef __STDC_VERSION__
    printf("C Standard Version: ");
    if (__STDC_VERSION__ == 202311L) {
        printf("C23\n");
    } else if (__STDC_VERSION__ == 201710L) {
        printf("C17\n");
    } else {
        printf("Unknown (%ld)\n", __STDC_VERSION__);
    }
    #else
    printf("Non-standard C (no __STDC_VERSION__)\n");
    #endif

    /* Static assertion at compile time */
    static_assert(sizeof(long) >= sizeof(int),
        "long must be at least as large as int");
    printf("(Static assertions passed)\n");
}

/* ============================================================ */
/* 3. INTEGER ARITHMETIC FUNCTIONS */
/* ============================================================ */

void demo_integer_arithmetic(void) {
    printf("\n=== Integer Arithmetic Functions ===\n");

    /* abs / labs / llabs */
    int neg = -42;
    printf("abs(%d) = %d\n", neg, abs(neg));

    /* div: quotient and remainder */
    struct div_t d = div(17, 5);
    printf("div(17, 5): quotient=%d, remainder=%d\n", d.quot, d.rem);
}

/* ============================================================ */
/* 4. CHECKED INTEGER ARITHMETIC (C23) */
/* ============================================================ */

void demo_checked_arithmetic(void) {
    printf("\n=== Checked Integer Arithmetic (C23) ===\n");

    unsigned int a = UINT_MAX;
    unsigned int b = 5;
    unsigned int result = 0;

    bool overflow = ckd_add(&result, a, b);
    printf("ckd_add(%u, %u):\n", a, b);
    printf("  Overflow: %s\n", overflow ? "YES" : "NO");
    printf("  Result (modular): %u\n", result);
}

/* ============================================================ */
/* 5. BIT MANIPULATION (C23) */
/* ============================================================ */

void demo_bit_operations(void) {
    printf("\n=== Bit Operations (C23) ===\n");

    unsigned int value = 0b101010;  /* 42 in binary */

    printf("Value: %u (binary: 0b101010)\n", value);
    printf("  stdc_bit_width: %d bits needed\n",
           stdc_bit_width(value));
    printf("  stdc_leading_zeros: %d zeros\n",
           stdc_leading_zeros(value));
    printf("  stdc_trailing_zeros: %d zeros\n",
           stdc_trailing_zeros(value));
    printf("  stdc_popcount: %d one-bits\n",
           stdc_popcount(value));
    printf("  stdc_has_single_bit: %s (power of 2?)\n",
           stdc_has_single_bit(value) ? "YES" : "NO");
}

/* ============================================================ */
/* 6. MATHEMATICAL FUNCTIONS */
/* ============================================================ */

void demo_math_functions(void) {
    printf("\n=== Mathematical Functions (via <tgmath.h>) ===\n");

    /* Trigonometric */
    double angle = M_PI / 6;  /* 30 degrees */
    printf("sin(π/6) = %.4f\n", sin(angle));
    printf("cos(π/6) = %.4f\n", cos(angle));
    printf("tan(π/6) = %.4f\n", tan(angle));

    /* Exponential and logarithm */
    printf("\nexp(1) = %.4f (e)\n", exp(1.0));
    printf("log(e) = %.4f\n", log(M_E));
    printf("log10(100) = %.4f\n", log10(100.0));

    /* Power and root */
    printf("\nsqrt(16) = %.4f\n", sqrt(16.0));
    printf("pow(2, 8) = %.4f\n", pow(2.0, 8.0));
    printf("cbrt(27) = %.4f\n", cbrt(27.0));

    /* Classification */
    double special1 = 0.0 / 0.0;  /* NaN */
    double special2 = 1.0 / 0.0;  /* Infinity */
    printf("\n0/0 is NaN: %s\n", isnan(special1) ? "YES" : "NO");
    printf("1/0 is infinity: %s\n", isinf(special2) ? "YES" : "NO");
    printf("42.0 is finite: %s\n", isfinite(42.0) ? "YES" : "NO");
}

/* ============================================================ */
/* 7. TIME FUNCTIONS */
/* ============================================================ */

void demo_time_functions(void) {
    printf("\n=== Time Functions ===\n");

    /* Get current time */
    time_t now = time(NULL);
    struct tm* local_time = localtime(&now);

    printf("Current local time:\n");
    printf("  Year: %d\n", local_time->tm_year + 1900);
    printf("  Month: %d (0=January)\n", local_time->tm_mon);
    printf("  Day: %d\n", local_time->tm_mday);
    printf("  Hour: %d\n", local_time->tm_hour);
    printf("  Minute: %d\n", local_time->tm_min);
    printf("  Second: %d\n", local_time->tm_sec);

    /* Format as string */
    char buffer[256];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
    printf("  Formatted: %s\n", buffer);
}

/* ============================================================ */
/* 8. CHARACTER CLASSIFICATION */
/* ============================================================ */

void demo_char_classification(void) {
    printf("\n=== Character Classification (<ctype.h>) ===\n");

    const char* test = "Hello123!@";
    printf("String: \"%s\"\n\n", test);

    for (size_t i = 0; i < strlen(test); ++i) {
        char c = test[i];
        printf("'%c':", c);
        if (isalpha(c)) printf(" alphabetic");
        if (isdigit(c)) printf(" digit");
        if (isspace(c)) printf(" whitespace");
        if (isupper(c)) printf(" uppercase");
        if (islower(c)) printf(" lowercase");
        if (ispunct(c)) printf(" punctuation");
        printf("\n");
    }
}

/* ============================================================ */
/* 9. PROGRAM TERMINATION FUNCTIONS */
/* ============================================================ */

void demo_termination_functions(void) {
    printf("\n=== Program Termination Control ===\n");
    printf("(Not actually calling exit/abort; just demonstrating)\n");
    printf("Available: exit(), _Exit(), quick_exit(), abort()\n");
    printf("All are [[noreturn]]: never return to caller\n");
}

/* ============================================================ */
/* 10. ASSERTIONS FOR PRECONDITIONS */
/* ============================================================ */

int safe_divide(int a, int b) {
    assert(b != 0);  /* Precondition */
    return a / b;
}

void demo_assertions(void) {
    printf("\n=== Assertions for Preconditions ===\n");

    printf("safe_divide(10, 2) = %d\n", safe_divide(10, 2));

    printf("(Not calling safe_divide(10, 0) to avoid assertion failure)\n");
}

/* ============================================================ */
/* MAIN COORDINATION */
/* ============================================================ */

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Chapter 8: C Library Functions - C23 Comprehensive Demo  ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    demo_error_handling();
    demo_preprocessor_checks();
    demo_integer_arithmetic();
    demo_checked_arithmetic();
    demo_bit_operations();
    demo_math_functions();
    demo_time_functions();
    demo_char_classification();
    demo_termination_functions();
    demo_assertions();

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    Demo Complete                           ║\n");
    printf("║      All library functions executed successfully!         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;
}
