# Chapter 15: Program Failure

Source: 15-programfailure.md

## 1. Exhaustive Concept Breakdown

### 1.1 Wrongdoings: Direct Program Failures

Wrongdoings are direct actions causing failure, blameworthy and preventable.

#### 1.1.1 Arithmetic Violations

Operations with operands lacking mathematical results:

TAKEAWAY 15.1: The program execution should only perform arithmetic operations mathematically defined within the range of the underlying type.

**Integer arithmetic violations**:

- Division by zero, modulo by zero.
- Negation of `INT_MIN` (overflow).
- Bit-shift with negative or out-of-range operand.
- Bit-shift into sign bit of signed type.

**Floating-point arithmetic**:

- Division by zero: May yield infinity (if platform supports `INFINITY`).
- Exceptions tracked via `<fenv.h>`: `FE_DIVBYZERO`, `FE_INEXACT`, `FE_INVALID`, `FE_OVERFLOW`, `FE_UNDERFLOW`.

TAKEAWAY 15.2: The floating-point environment of the platform determines which floating-point operations result in program failure.

Query/clear floating-point exceptions:

```c
if (fetestexcept(FE_DIVBYZERO)) { /* handle */ }
feclearexcept(FE_ALL_EXCEPT);
```

#### 1.1.2 Invalid Conversions

Values don't have valid interpretation in target type:

- **Integer to signed**: `UINT_MAX` to signed is implementation-defined (not portable).
- **Float ↔ Integer**: Value out of range or precision loss.
- **Float ↔ Float**: Precision loss or out-of-range.
- **Pointer to integer**: Conversion narrower than pointer width fails (use `uintptr_t` if available).
- **Pointer to pointer**: Source pointer misaligned for target type.

**Pointer alignment violations**: Converting unaligned pointer to stricter-alignment type.

```c
unsigned char* unaligned = ...;
int* p = (int*)&unaligned[1];  // Misaligned! UB
```

#### 1.1.3 Value Violations

Invalid function arguments or unreachable results:

- Null pointers where non-null expected.
- Out-of-range sizes (zero for allocation functions).
- Floating-point comparison with NaN (always false).

#### 1.1.4 Type Violations

Accessing objects/functions with wrong type:

TAKEAWAY 15.5: Don't convert pointers unless you must.
TAKEAWAY 15.6: Always call a function with the prototype with which it is defined.

**Function pointer casting**: Calling function via incorrectly-typed pointer.

```c
int add(int a, int b) { return a + b; }
typedef int (*wrong_fn)(void);
wrong_fn wfn = (wrong_fn)&add;
int result = wfn();  // UNDEFINED: signature mismatch
```

TAKEAWAY 15.7: Call a function by its name.

#### 1.1.5 Access Violations

Pointer/array access errors—most common wrongdoing category:

- **Null pointer dereference**: `*nullptr` or `nullptr->member`.
- **Stale pointer**: Address of freed storage, local variable, or changed system object.
- **Out-of-bounds**: Array indexing beyond valid range.
- **Const violation**: Writing to const-qualified object or string literal.
- **Volatile mismatch**: Accessing volatile object through non-volatile lvalue.
- **Restrict violation**: Accessing through pointer not derived from restrict pointer.
- **Overlapping store**: Memcpy/scanf overwriting active object via unrelated pointer.

#### 1.1.6 Value Misinterpretation

Object accessed with uninitialized or invalid bit pattern:

- **Indeterminate value**: Uninitialized automatic variable or malloc storage.
- **Non-value representation**: Some types have more bits than representable values (e.g., `bool`).

TAKEAWAY 15.8: Don't store values other than 0 or 1 in a bool object.
TAKEAWAY 15.9: Don't change the representation bytes of objects directly.

#### 1.1.7 Explicit Invalidation (unreachable Macro)

C23's `unreachable()` asserts control path never taken:

```c
if (!p || !q) unreachable();
return p - q;  // Guarantees p, q non-null
```

TAKEAWAY 15.10: Only use `unreachable()` where you have proof.
TAKEAWAY 15.11: Don't use other operations than `unreachable()` to mark unreachable paths.

### 1.2 Program State Degradation

Failures from cumulative resource exhaustion, not single wrongdoing.

#### 1.2.1 Unbounded Recursion

Stack overflow from recursive calls without progress:

```c
int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);  // Must have base case
}
if (!n <= 0 || n > max) unreachable();  // Ensure termination
```

#### 1.2.2 Storage Exhaustion

**Stack overflow**: VLAs with unbounded size; deep recursion.

TAKEAWAY 15.12: VLAs (contrast to VM types) are optional in C23; tested via `__STDC_NO_VLA__`.

**Heap exhaustion**: `malloc`/`calloc`/etc. return null on failure.

```c
int* p = malloc(huge_size);
if (!p) {
    fprintf(stderr, "Allocation failed\n");
    return EXIT_FAILURE;
}
```

#### 1.2.3 Scarce Resource Exhaustion

Table 15.1 lists resources with reservation/release functions:

| Resource       | Reservation        | Release                       | Limit                  |
| -------------- | ------------------ | ----------------------------- | ---------------------- |
| Stream         | `fopen`, `tmpfile` | `fclose`, `remove`, `freopen` | `FOPEN_MAX`, `TMP_MAX` |
| File           | `fopen`            | same                          | OS-dependent           |
| Thread context | `thrd_create`      | `thrd_join`, `thrd_detach`    | OS-dependent           |
| Mutex          | `mtx_init`         | `mtx_destroy`                 | 0 (resource)           |

### 1.3 Unfortunate Incidents

Failures from distant events in time/space, difficult to trace.

#### 1.3.1 Escalating State Degradation

Continued execution after resource exhaustion corrupts program state:

```c
if (stack_exhausted) {
    abort();  // Stop immediately, not silently continue
}
```

#### 1.3.2 Collisions and Race Conditions

Unsequenced access to same object:

TAKEAWAY 15.13: Don't read and modify the same object within the same arithmetic expression.

```c
printf("%d\n", x++ + x);  // UB: unsequenced access
```

**Pointer aliasing race**: Pointers may refer to same object undetectably:

```c
printf("%d\n", (*p)++ + (*q));  // If p == q, race condition
```

**Signal handlers**: Access to objects from signal handler is unsequenced:

```c
volatile sig_atomic_t flag = 0;
void handler(int sig) { flag = 1; }
// Main code reads flag; unsequenced with handler write
```

#### 1.3.3 Library Call Restrictions

Some functions restricted to specific contexts:

- `signal()`: Not allowed in multithreaded program.
- `setjmp()`: Only in specific expression positions.

#### 1.3.4 Deadlocks

Cyclic dependencies preventing progress (multithreaded only):

```c
Thread1: acquires lock_A, waits for lock_B
Thread2: acquires lock_B, waits for lock_A
// DEADLOCK: neither can proceed
```

### 1.4 Series of Unfortunate Events

Execution looping over finite states with no observable progress:

TAKEAWAY 15.14: A program execution that loops over a finite set of states with no observable side effects has failed.

## 2. Syntax, Types, and Functions Dictionary

### Error Handling

| Function/Macro    | Header       | Purpose                                |
| ----------------- | ------------ | -------------------------------------- |
| `errno`           | `<errno.h>`  | Global error code (per thread C11+)    |
| `perror(prefix)`  | `<stdio.h>`  | Print error message and errno          |
| `strerror(errno)` | `<string.h>` | Convert errno to string                |
| `assert(expr)`    | `<assert.h>` | Assertion; abort if false (debug mode) |

### Floating-Point Environment

| Function/Macro                              | Header                    | Purpose                          |
| ------------------------------------------- | ------------------------- | -------------------------------- |
| `fetestexcept(excepts)`                     | `<fenv.h>`                | Test floating-point exceptions   |
| `feclearexcept(excepts)`                    | `<fenv.h>`                | Clear floating-point exceptions  |
| `FE_DIVBYZERO`, `FE_INVALID`, `FE_OVERFLOW` | `<fenv.h>`                | Exception flags                  |
| `INFINITY`                                  | `<float.h>` or `<math.h>` | Floating-point infinity constant |

### Control Flow for Errors

| Mechanism            | Header                  | Purpose                                       |
| -------------------- | ----------------------- | --------------------------------------------- |
| `return`             | (builtin)               | Return error code from function               |
| `exit(code)`         | `<stdlib.h>`            | Terminate program with exit code              |
| `abort()`            | `<stdlib.h>`            | Abnormally terminate (core dump if available) |
| `setjmp(buf)`        | `<setjmp.h>`            | Set jump point (see Chapter 19)               |
| `longjmp(buf, code)` | `<setjmp.h>`            | Jump to saved point with code                 |
| `unreachable()`      | `<stdnoreturn.h>` (C23) | Assert path never taken                       |

### Types and Constants

| Type/Constant  | Header          | Purpose                                             |
| -------------- | --------------- | --------------------------------------------------- |
| `sig_atomic_t` | `<signal.h>`    | Atomic type for signal handlers                     |
| `volatile`     | (builtin)       | Qualifier: prevent optimization, reload from memory |
| `restrict`     | (C99+, builtin) | Qualfier: hint that pointers don't alias            |
| `EILSEQ`       | `<errno.h>`     | Illegal byte sequence (encoding error)              |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Arithmetic Traps

**Pitfall**: Division by user input, shift by user input.

```c
int b = get_user_input();
int result = 100 / b;  // b could be 0!
int shifted = x >> b;  // b could be >= bit width!
```

**Fix**: Validate: `if (b == 0) return error; if (b >= 32) return error;`

### 3.2 Uninitialized Variables

**Pitfall**: Reading uninitialized automatic variable.

```c
int x;  // Uninitialized!
printf("%d\n", x);  // UB: indeterminate value
```

**Fix**: Initialize: `int x = 0;` or `int x = expression;`

### 3.3 Out-of-Bounds Access

**Pitfall**: Array indexing beyond bounds.

```c
int arr[10];
for (int i = 0; i <= 10; ++i) {  // Should be i < 10
    arr[i] = 0;  // arr[10] out-of-bounds
}
```

**Fix**: Use correct bounds: `i < size` not `i <= size`.

### 3.4 Stale Pointer to Local Variable

**Pitfall**: Returning address of local variable.

```c
int* get_value(void) {
    int x = 42;
    return &x;  // x destroyed on return!
}
int* p = get_value();
printf("%d\n", *p);  // CRASH: stale pointer
```

**Fix**: Return value, not address; use dynamic allocation if returning pointer.

### 3.5 Format String Vulnerabilities

**Pitfall**: User input as format string.

```c
char buf[256];
fgets(buf, sizeof buf, stdin);
printf(buf);  // DANGEROUS! User could supply %x to read stack
```

**Fix**: `printf("%s", buf);` with explicit format.

### 3.6 Double-Free from Exception

**Pitfall**: Exception path doesn't free acquired resources.

```c
int* p = malloc(size);
if (error_condition) return; // p leaked!
free(p);
```

**Fix**: Use error handling patterns; RAII-style cleanup.

## 4. Comprehensive C23 Working Example

```c
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
```

**Key Takeaways from Example**:

- Validate user input (divisions, shifts, array indices).
- Check allocation return values; null indicates failure.
- Avoid unsequenced access; separate statements for order guarantees.
- Use assertions in debug builds to catch invariant violations.
- Handle floating-point exceptions via `<fenv.h>` if needed.
- Set freed pointers to null; add safety checks before dereferencing.
- Convert errno to messages via `strerror()` for user-friendly errors.
- Maintain invariants (contracts) at function boundaries.
