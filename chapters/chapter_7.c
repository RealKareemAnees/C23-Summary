#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

/* ============================================================ */
/* 1. SIMPLE FUNCTIONS WITH PROTOTYPES */
/* ============================================================ */

/* Forward declarations (prototypes) */
int add(int a, int b);
int multiply(int a, int b);
void print_menu(void);

/* Function definitions */
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

void print_menu(void) {
    printf("=== Simple Functions Demo ===\n");
    printf("Sum: %d\n", add(5, 3));
    printf("Product: %d\n", multiply(5, 3));
}

/* ============================================================ */
/* 2. RECURSION: GCD (Greatest Common Divisor) */
/* ============================================================ */

/* Recursive implementation with preconditions */
inline size_t gcd2(size_t a, size_t b) {
    assert(a <= b);         /* Precondition: a must be ≤ b */
    if (!a) return b;       /* Base case */
    size_t r = b % a;
    return gcd2(r, a);      /* Recursive: (b%a, a) always satisfies condition */
}

/* Wrapper: enforces preconditions */
inline size_t gcd(size_t a, size_t b) {
    assert(a && b);         /* Both must be non-zero */
    if (a < b) return gcd2(a, b);
    else return gcd2(b, a);
}

void demo_gcd(void) {
    printf("\n=== GCD Recursion (Euclid's Algorithm) ===\n");
    printf("GCD(18, 30) = %zu\n", gcd(18, 30));
    printf("GCD(100, 35) = %zu\n", gcd(100, 35));
    printf("GCD(17, 19) = %zu (coprime)\n", gcd(17, 19));
}

/* ============================================================ */
/* 3. MULTIPLE RECURSION: EXPONENTIAL TIME (NAIVE) */
/* ============================================================ */

size_t fib_naive(size_t n) {
    if (n < 3) return 1;
    return fib_naive(n-1) + fib_naive(n-2);
}

/* ============================================================ */
/* 4. OPTIMIZED RECURSION: LINEAR TIME (MEMOIZATION) */
/* ============================================================ */

size_t fib_cache_rec(size_t n, size_t cache[static n]) {
    if (!cache[n-1]) {
        cache[n-1] = fib_cache_rec(n-1, cache) +
                     fib_cache_rec(n-2, cache);
    }
    return cache[n-1];
}

size_t fib_cache(size_t n) {
    if (n+1 <= 3) return 1;

    size_t cache[n];
    for (size_t i = 0; i < n; ++i) cache[i] = 0;

    cache[0] = 1;
    cache[1] = 1;

    return fib_cache_rec(n, cache);
}

/* ============================================================ */
/* 5. ITERATIVE ALTERNATIVE: TAIL RECURSION OPTIMIZATION */
/* ============================================================ */

void fib2_rec(size_t n, size_t buf[static 2]) {
    if (n > 2) {
        size_t res = buf[0] + buf[1];
        buf[1] = buf[0];
        buf[0] = res;
        fib2_rec(n-1, buf);
    }
}

size_t fib2(size_t n) {
    size_t buf[2] = { 1, 1 };
    fib2_rec(n, buf);
    return buf[0];
}

void demo_fibonacci(void) {
    printf("\n=== Fibonacci: Exponential vs Optimized ===\n");

    size_t n = 15;
    printf("Naïve fib(%zu) = %zu (very slow)\n", n, fib_naive(n));
    printf("Cached fib(%zu) = %zu (linear time)\n", n, fib_cache(n));
    printf("Tail-recursive fib(%zu) = %zu (constant space)\n", n, fib2(n));

    printf("\nSequence (tail-recursive):\n");
    for (size_t i = 1; i <= 10; ++i) {
        printf("fib(%zu) = %zu\n", i, fib2(i));
    }
}

/* ============================================================ */
/* 6. COMMAND-LINE ARGUMENT PROCESSING */
/* ============================================================ */

void process_arguments(int argc, char* argv[argc+1]) {
    printf("\n=== Command-Line Arguments ===\n");
    printf("Program name (argv[0]): %s\n", argv[0]);
    printf("Total arguments (argc): %d\n", argc);
    printf("Remaining arguments:\n");

    for (int i = 1; i < argc; ++i) {
        printf("  argv[%d] = \"%s\"\n", i, argv[i]);
    }

    /* Demonstrate argv[argc] == NULL */
    printf("argv[argc] == NULL: %s\n",
           (argv[argc] == NULL) ? "true" : "false");
}

/* ============================================================ */
/* 7. FUNCTION THAT RETURNS DIFFERENT PATHS */
/* ============================================================ */

int validate_age(int age) {
    if (age < 0) {
        fprintf(stderr, "Error: negative age\n");
        return -1;      /* Error path */
    }
    if (age < 18) {
        return 0;       /* Minor */
    }
    if (age < 65) {
        return 1;       /* Adult */
    }
    return 2;           /* Senior */
}

void demo_multi_return(void) {
    printf("\n=== Multiple Returns ===\n");

    int ages[] = { 5, 18, 35, 70, -1 };
    const char* status[] = { "Minor", "Adult", "Senior", "Error" };

    for (size_t i = 0; i < 5; ++i) {
        int result = validate_age(ages[i]);
        if (result < 0) {
            printf("Age %d: Invalid (error)\n", ages[i]);
        } else {
            printf("Age %d: %s\n", ages[i], status[result]);
        }
    }
}

/* ============================================================ */
/* 8. HELPER FUNCTIONS WITH PRECONDITIONS */
/* ============================================================ */

struct Fraction {
    int numerator;
    int denominator;
};

struct Fraction reduce_fraction(struct Fraction f) {
    /* Precondition: denominator ≠ 0 */
    assert(f.denominator != 0);

    size_t g = gcd(
        (size_t)((f.numerator < 0) ? -f.numerator : f.numerator),
        (size_t)f.denominator
    );

    return (struct Fraction) {
        .numerator = f.numerator / (int)g,
        .denominator = f.denominator / (int)g
    };
}

void demo_fractions(void) {
    printf("\n=== Fractions with Preconditions ===\n");

    struct Fraction fracs[] = {
        { 12, 8 },
        { 15, 25 },
        { 7, 1 }
    };

    for (size_t i = 0; i < 3; ++i) {
        struct Fraction reduced = reduce_fraction(fracs[i]);
        printf("%d/%d → %d/%d\n",
               fracs[i].numerator, fracs[i].denominator,
               reduced.numerator, reduced.denominator);
    }
}

/* ============================================================ */
/* MAIN: COORDINATING ALL DEMOS */
/* ============================================================ */

int main(int argc, char* argv[argc+1]) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║         Chapter 7: Functions - C23 Comprehensive Demo     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    print_menu();
    demo_gcd();
    demo_fibonacci();
    demo_multi_return();
    demo_fractions();

    /* Echo command-line arguments if provided */
    if (argc > 1) {
        process_arguments(argc, argv);
    } else {
        printf("\n(Run with arguments to see argc/argv demo: %s arg1 arg2 ...)\n", argv[0]);
    }

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    Demo Complete                           ║\n");
    printf("║         All functions executed successfully!              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;  /* TAKEAWAY 7.7: Explicit EXIT_SUCCESS */
}
