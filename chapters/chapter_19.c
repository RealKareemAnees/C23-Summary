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
