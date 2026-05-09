# Chapter 19: Variations in Control Flow

Source: 19-variationscontrolflow.md

## 1. Exhaustive Concept Breakdown

### 1.1 Sequencing and Basic Blocks

TAKEAWAY 19.1: Sequencing is about the order in which statements are evaluated.

Statements execute sequentially by default; control flow keywords redirect this order.

**Sequence points** (C89): Guarantee that prior volatile/side effects complete before next point.

C11/C23 terminology: **Unsequenced** (no order guaranteed), **Indeterminately sequenced** (one before other, unknown which), **Sequentially consistent** (order defined).

TAKEAWAY 19.2: Unstructured control flow via goto should be used only as a last resort.

### 1.2 goto Statement

**Syntax**:

```c
goto label;
// ...
label: statement;
```

**Legitimate uses**:

- Error recovery/cleanup (exception-like pattern).
- Breaking from nested loops.
- State machine transitions.

**Anti-patterns**:

- Spaghetti code (arbitrary jumps).
- Jumping into blocks from outside (undefined behavior).
- Forward jumps over declarations without initialization.

### 1.3 setjmp/longjmp (Non-Local Jumps)

**Exception-like mechanism** (though not true exceptions):

```c
#include <setjmp.h>
jmp_buf env;
if (setjmp(env) == 0) {
    // Normal execution path
    if (error_condition) longjmp(env, 1);
} else {
    // Exception handler (longjmp returned here)
}
```

**Semantics**:

- `setjmp()` saves execution state; returns 0 on first call.
- `longjmp(env, value)` restores state; `setjmp()` appears to return `value`.
- Skips destructors; manual cleanup required (C++ would call destructors on unwinding).

TAKEAWAY 19.3: The longjmp function shall not be used after a failed attempt to restore the state.

**Limitations**:

- No automatic cleanup.
- Volatile accesses unpredictable across longjmp.
- Stack corruption if buffer exited.

### 1.4 Signal Handlers

Asynchronous events (SIGINT, SIGSEGV, etc.) invoke handler functions.

**`<signal.h>` API**:

```c
typedef void (*sig_handler_t)(int);
sig_handler_t signal(int sig, sig_handler_t handler);
```

**Signal safety**: handler can only safely call certain async-signal-safe functions (e.g., `write()`, not `printf()`).

**Limitations**:

- Handler runs asynchronously; no control over timing.
- Preempts current execution (if accessing same data → race condition).
- Cannot reliably use malloc/free/stdio.

TAKEAWAY 19.4: A signal handler can only call functions from the list of async-signal-safe functions.

TAKEAWAY 19.5: Frequently-used signal handlers use a flag to report occurrence; the flag is global and volatile.

**Safe pattern**:

```c
static volatile sig_atomic_t signal_received = 0;
void handler(int sig) { signal_received = 1; }
// In main: if (signal_received) { handle_signal(); signal_received = 0; }
```

### 1.5 Control Flow in Practice

**loops and ifs**: Structured, predictable.

**For breaking nested loops**, use `goto` (or refactor into function).

**For complex state machines**, explicit state enums + switch statements preferred over `goto`.

## 2. Syntax, Types, and Functions Dictionary

### Control Flow Statements

| Statement                              | Purpose                            |
| -------------------------------------- | ---------------------------------- |
| `if (condition) { } else { }`          | Conditional execution              |
| `switch (expr) { case v: ... break; }` | Multi-way branch                   |
| `for (init; cond; inc) { }`            | Loop with init/condition/increment |
| `while (cond) { }`                     | Loop while condition true          |
| `do { } while (cond)`                  | Loop with trailing condition       |
| `break`                                | Exit switch/loop                   |
| `continue`                             | Jump to loop increment             |
| `goto label`                           | Unconditional jump (use sparingly) |
| `return expr`                          | Exit function, return value        |

### setjmp/longjmp (`<setjmp.h>`)

| Function                               | Purpose                                               |
| -------------------------------------- | ----------------------------------------------------- |
| `int setjmp(jmp_buf env)`              | Save state; return 0 (initial), or value from longjmp |
| `void longjmp(jmp_buf env, int value)` | Restore state; setjmp appears to return value         |

### Signals (`<signal.h>`)

| Function/Type                                          | Purpose                       |
| ------------------------------------------------------ | ----------------------------- |
| `sig_handler_t signal(int sig, sig_handler_t handler)` | Register signal handler       |
| `int raise(int sig)`                                   | Send signal to self           |
| `SIGABRT`, `SIGINT`, `SIGSEGV`, etc.                   | Signal numbers                |
| `SIG_DFL`                                              | Default handler               |
| `SIG_IGN`                                              | Ignore signal                 |
| `sig_atomic_t`                                         | Type safe for signal handlers |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 goto into Blocks

**Pitfall**: Jumping into a block from outside skips initialization.

```c
if (error) goto recovery;
{
    int x = 10;  // Skipped by goto
recovery:
    printf("%d\n", x);  // UB: x uninitialized
}
```

**Fix**: Jump to block start or refactor with nested blocks.

### 3.2 longjmp Across Stack Frame Loss

**Pitfall**: Longjmping past function that allocated resources.

```c
jmp_buf env;
void cleanup_needed(void) {
    int* p = malloc(100);
    if (error) longjmp(env, 1);  // Memory leaked!
}
```

**Fix**: Ensure cleanup before longjmp; use flags pattern for recovery.

### 3.3 Volatile and setjmp Surprises

**Pitfall**: Non-volatile locals may have stale values after longjmp.

```c
int count = 0;
if (setjmp(env) == 0) {
    count = 5;
    longjmp(env, 1);
} else {
    printf("%d\n", count);  // May not be 5; undefined in C89
}
```

**Fix**: Mark `count` volatile or set after setjmp branch.

### 3.4 Signal Handler Race Conditions

**Pitfall**: Handler and main code access same variable.

```c
int flag = 0;
void handler(int sig) { flag = 1; }  // Race condition
// In main: flag = 0; ...
```

**Fix**: Use `volatile sig_atomic_t` for flag; minimize shared state.

### 3.5 Non-Async-Signal-Safe Functions in Handlers

**Pitfall**: Calling unsafe function in handler.

```c
void handler(int sig) {
    printf("Signal!\n");  // printf is NOT async-signal-safe
}
```

**Fix**: Call only async-signal-safe functions (see POSIX list); prefer simple flag-setting.

## 4. Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>

/* === 4.1 goto for Error Recovery === */

void demo_goto_recovery(void) {
    printf("\n--- goto for Error Recovery ---\n");

    FILE* f1 = NULL;
    FILE* f2 = NULL;

    // Simulate resource allocation
    f1 = fopen("/etc/hostname", "r");
    if (!f1) {
        printf("Error: cannot open file\n");
        goto cleanup;
    }

    f2 = fopen("/etc/passwd", "r");
    if (!f2) {
        printf("Error: cannot open second file\n");
        goto cleanup;
    }

    printf("Both files opened successfully\n");

cleanup:
    if (f2) fclose(f2);
    if (f1) fclose(f1);
    printf("Cleanup complete\n");
}

/* === 4.2 setjmp/longjmp for Exception-Like Recovery === */

static jmp_buf exception_env;

int safe_divide(int a, int b) {
    if (b == 0) {
        printf("Division by zero detected; longjmping\n");
        longjmp(exception_env, -1);  // Return -1 to exception handler
    }
    return a / b;
}

void demo_setjmp_longjmp(void) {
    printf("\n--- setjmp/longjmp Exception Simulation ---\n");

    int setjmp_result = setjmp(exception_env);

    if (setjmp_result == 0) {
        // Normal execution path
        printf("Attempting 20 / 4...\n");
        int r1 = safe_divide(20, 4);
        printf("Result: %d\n", r1);

        printf("Attempting 20 / 0...\n");
        int r2 = safe_divide(20, 0);  // Will longjmp
        printf("This line skipped\n");

    } else if (setjmp_result == -1) {
        // Exception handler: caught error
        printf("Caught exception! Error code: %d\n", setjmp_result);
    }
}

/* === 4.3 Signal Handler with Flag Pattern === */

static volatile sig_atomic_t signal_count = 0;

void signal_handler(int sig) {
    // Only modify sig_atomic_t; minimal work
    if (sig == SIGINT) {
        signal_count++;
    }
}

void demo_signal_handling(void) {
    printf("\n--- Signal Handling (Flag Pattern) ---\n");

    // Register signal handler
    signal(SIGINT, signal_handler);

    printf("Signal handler registered (SIGINT)\n");
    printf("signal_count = %d\n", signal_count);

    // Simulate signal receipt (self-directed)
    printf("Raising SIGINT to self...\n");
    raise(SIGINT);
    printf("signal_count = %d\n", signal_count);

    raise(SIGINT);
    printf("signal_count = %d\n", signal_count);

    // Restore default handler
    signal(SIGINT, SIG_DFL);
    printf("Signal handler restored\n");
}

/* === 4.4 Breaking from Nested Loops === */

void demo_nested_loops(void) {
    printf("\n--- Breaking Nested Loops with goto ---\n");

    int found = 0;

    for (int i = 0; i < 3 && !found; ++i) {
        for (int j = 0; j < 3 && !found; ++j) {
            printf("Checking [%d][%d]\n", i, j);
            if (i == 1 && j == 2) {
                printf("Found at [%d][%d]\n", i, j);
                found = 1;
            }
        }
    }

    // Alternative: use goto (works but less clean)
    printf("\nWith goto:\n");
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("Checking [%d][%d]\n", i, j);
            if (i == 1 && j == 2) {
                printf("Found at [%d][%d], jumping out\n", i, j);
                goto break_outer;
            }
        }
    }
break_outer:
    printf("Exited nested loop\n");
}

/* === 4.5 State Machine with goto === */

typedef enum {
    STATE_INIT,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE
} State;

void demo_state_machine(void) {
    printf("\n--- State Machine with goto ---\n");

    State state = STATE_INIT;
    int iteration = 0;

init_state:
    printf("STATE_INIT: Initializing\n");
    state = STATE_READING;
    goto reading_state;

reading_state:
    printf("STATE_READING: Reading data\n");
    state = STATE_PROCESSING;
    goto processing_state;

processing_state:
    printf("STATE_PROCESSING: Processing\n");
    if (iteration++ < 2) {
        printf("  Iteration %d\n", iteration);
        goto processing_state;  // Loop within state
    }
    state = STATE_DONE;
    goto done_state;

done_state:
    printf("STATE_DONE: Complete\n");
}

/* === 4.6 Structured Alternatives: Switch/Case === */

void demo_switch_case(void) {
    printf("\n--- Structured Alternative: Switch/Case ---\n");

    int command = 2;

    switch (command) {
        case 1:
            printf("Command 1: Initialize\n");
            break;
        case 2:
            printf("Command 2: Process\n");
            break;
        case 3:
            printf("Command 3: Finalize\n");
            break;
        default:
            printf("Unknown command\n");
    }
}

/* === 4.7 Control Flow Pitfall: Uninitialized After goto === */

void demo_goto_initialization_pitfall(void) {
    printf("\n--- Initialization Pitfall (Avoided) ---\n");

    int error_code = 0;

    if (error_code) {
        goto error;
    }

    {
        int value = 42;
        printf("Value initialized: %d\n", value);
        // Exiting block naturally (not via goto)
    }

error:
    printf("Error handler\n");
}

/* === 4.8 Sequence Point Demonstration === */

void demo_sequence_points(void) {
    printf("\n--- Sequence Points ---\n");

    int a = 5;
    int b = 10;

    // Operators enforce sequence points before/after
    a = 3;
    b = 4;  // Sequence point: prior assignment complete

    printf("a = %d, b = %d\n", a, b);

    // Function call introduces sequence point
    int result = (printf("Before\n"), 42, printf("After\n"), 100);
    printf("Result: %d\n", result);
}

int main(void) {
    printf("=== Chapter 19: Variations in Control Flow ===\n");

    demo_goto_recovery();
    demo_setjmp_longjmp();
    demo_signal_handling();
    demo_nested_loops();
    demo_state_machine();
    demo_switch_case();
    demo_goto_initialization_pitfall();
    demo_sequence_points();

    printf("\n=== End Chapter 19 ===\n");
    return 0;
}
```

**Key Takeaways**:

- Structured control flow (if/for/while) preferred over goto.
- goto useful for error recovery and breaking nested loops (sparingly).
- setjmp/longjmp provide exception-like mechanism; require manual resource cleanup.
- Signal handlers must be minimal and use only async-signal-safe functions.
- Sequencing rules prevent data races in single-threaded code.
