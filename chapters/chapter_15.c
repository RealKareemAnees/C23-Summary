#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fenv.h>
#include <math.h>
#include <errno.h>

/* === 4.1 Arithmetic Violations and Detection === */

void demo_arithmetic_violations(void) {
    printf("\n--- Arithmetic Violations ---\n");

    // Division by zero (integer)
    int a = 10, b = 0;
    printf("Attempting 10 / 0: ");
    if (b == 0) {
        printf("BLOCKED (would crash)\n");
    } else {
        printf("%d\n", a / b);
    }

    // Right-shift with excessive operand
    int shift_val = 32;  // Too large for typical int
    printf("Attempting x >> 32: ");
    if (shift_val >= (int)sizeof(int) * 8) {
        printf("BLOCKED (out-of-range shift)\n");
    } else {
        printf("%d\n", a >> shift_val);
    }

    // Negation of INT_MIN
    int min_int = INT_MIN;
    printf("Attempting -INT_MIN: ");
    if (min_int == INT_MIN) {
        printf("BLOCKED (overflow on negation)\n");
    } else {
        printf("%d\n", -min_int);
    }
}

/* === 4.2 Floating-Point Exception Handling === */

void demo_fpu_exceptions(void) {
    printf("\n--- Floating-Point Exceptions ---\n");

    // Check if platform supports infinity
    #ifdef INFINITY
        printf("Platform supports INFINITY\n");
        double inf = 1.0 / 0.0;
        printf("1.0 / 0.0 = %g\n", inf);
    #else
        printf("Platform does NOT support INFINITY (division by 0 undefined)\n");
    #endif

    // Test exception flags
    feclearexcept(FE_ALL_EXCEPT);

    double x = 0.0;
    double y = 1.0 / x;  // May set FE_DIVBYZERO or FE_INVALID

    printf("After 1.0/0.0: ");
    if (fetestexcept(FE_DIVBYZERO)) printf("[DIVBYZERO] ");
    if (fetestexcept(FE_INVALID)) printf("[INVALID]   ");
    printf("\n");

    feclearexcept(FE_ALL_EXCEPT);
}

/* === 4.3 Pointer Arithmetic Violations === */

void demo_pointer_violations(void) {
    printf("\n--- Pointer Arithmetic Violations ---\n");

    int arr[5] = { 1, 2, 3, 4, 5 };
    int* p = arr;

    // Valid: within array
    printf("arr[2] via pointer: %d\n", *(p + 2));

    // Out-of-bounds: technically UB, but may not crash immediately
    printf("arr[10] via pointer: (attempted, but flagged)\n");
    // int val = *(p + 10);  // Undefined behavior; don't do this

    // Pointer subtraction: valid only within same array
    int* q = &arr[3];
    ptrdiff_t diff = q - p;
    printf("Pointer difference (q - p): %td elements\n", diff);
}

/* === 4.4 Type Violations and Casts === */

void demo_type_violations(void) {
    printf("\n--- Type Violations ---\n");

    // Correct: call by name
    int x = 42;
    printf("Calling add_two by name: %d\n", x + 2);

    // Dangerous: function pointer with wrong type
    typedef int (*wrong_fn)(void);
    int add(int a, int b) { return a + b; }

    printf("Function pointer with CORRECT signature works\n");
    typedef int (*right_fn)(int, int);
    right_fn rfn = &add;
    printf("Result: %d\n", rfn(10, 20));
}

int add(int a, int b) { return a + b; }

/* === 4.5 Unsequenced Access (Expression Evaluation) === */

void demo_unsequenced_access(void) {
    printf("\n--- Unsequenced Access ---\n");

    int x = 5;
    printf("x = %d\n", x);

    // DANGEROUS: unsequenced access
    printf("Attempting: printf(\"%%d\\n\", x++ + x);\n");
    printf("Result undefined (depends on evaluation order)\n");
    // printf("%d\n", x++ + x);  // Don't do this!

    // SAFE: separate statements
    int y = 5;
    y++;
    int result = y + y;
    printf("Safe equivalent: ++y; result = y + y = %d\n", result);
}

/* === 4.6 Resource Exhaustion Detection === */

void demo_resource_exhaustion(void) {
    printf("\n--- Resource Exhaustion Detection ---\n");

    // Simulate allocation attempts
    size_t attempt_size = 1024;
    for (int attempt = 0; attempt < 3; ++attempt) {
        void* p = malloc(attempt_size);

        if (p) {
            printf("Allocation %d: %zu bytes - SUCCESS\n", attempt, attempt_size);
            free(p);
            attempt_size *= 2;
        } else {
            printf("Allocation %d: %zu bytes - FAILED (heap exhaustion)\n",
                   attempt, attempt_size);
            break;
        }
    }
}

/* === 4.7 Null Pointer Checks === */

void demo_null_pointer_safety(void) {
    printf("\n--- Null Pointer Safety ---\n");

    int* p = malloc(sizeof *p);

    if (!p) {
        printf("malloc failed: NULL pointer\n");
        return;
    }

    *p = 42;
    printf("Allocated value: %d\n", *p);

    free(p);
    p = NULL;  // Best practice

    // Later: safe check before dereference
    if (p) {
        printf("Value: %d\n", *p);
    } else {
        printf("Pointer is NULL; safe to skip dereference\n");
    }
}

/* === 4.8 Error Codes and errno === */

void demo_error_codes(void) {
    printf("\n--- Error Codes and errno ---\n");

    FILE* fp = fopen("/nonexistent/file.txt", "r");

    if (!fp) {
        printf("fopen failed: %s\n", strerror(errno));
    }

    // String to number with error detection
    const char* invalid_num = "123abc";
    char* endp;
    long val = strtol(invalid_num, &endp, 10);

    if (*endp != '\0') {
        printf("Partial parse: converted %ld, stopped at '%s'\n", val, endp);
    }
}

/* === 4.9 Assertions and Debugging === */

void demo_assertions(void) {
    printf("\n--- Assertions (Debug Aid) ---\n");

    int x = 100;
    assert(x > 0);  // Checks in debug builds
    printf("Assertion passed: x > 0\n");

    // In release builds, assertions are disabled
    #ifdef NDEBUG
        printf("Assertions disabled (NDEBUG defined)\n");
    #else
        printf("Assertions enabled (debug mode)\n");
    #endif
}

/* === 4.10 ISI (Invariant, Sequencing, Interaction) Analysis === */

void demo_invariants(void) {
    printf("\n--- Invariants and State ---\n");

    struct Account {
        double balance;
        int transaction_count;
    } acc = { 1000.0, 0 };

    // Invariant: balance >= 0, count >= 0
    printf("Account balance: $%.2f (transactions: %d)\n",
           acc.balance, acc.transaction_count);

    // Safe transaction
    double withdrawal = 100.0;
    if (withdrawal <= acc.balance) {
        acc.balance -= withdrawal;
        acc.transaction_count++;
        printf("After withdrawal: $%.2f (transactions: %d)\n",
               acc.balance, acc.transaction_count);
    } else {
        printf("Withdrawal blocked: insufficient funds\n");
    }

    // Invariant maintained
    assert(acc.balance >= 0);
    assert(acc.transaction_count >= 0);
}

int main(void) {
    printf("=== Chapter 15: Program Failure ===\n");

    demo_arithmetic_violations();
    demo_fpu_exceptions();
    demo_pointer_violations();
    demo_type_violations();
    demo_unsequenced_access();
    demo_resource_exhaustion();
    demo_null_pointer_safety();
    demo_error_codes();
    demo_assertions();
    demo_invariants();

    printf("\n=== End Chapter 15 ===\n");
    return 0;
}
