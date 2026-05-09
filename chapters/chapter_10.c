#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ============================================================ */
/* ORGANIZATION & DOCUMENTATION EXAMPLE: A Simple Stack       */
/* ============================================================ */

/**
 * @file stack_example.c
 * @brief Demonstration of organization and documentation principles
 *
 * This module implements a simple integer stack with proper:
 * - Interface/implementation separation
 * - Comprehensive documentation (Doxygen compatible)
 * - Pure functions wherever possible
 * - Obvious control flow
 * - Semantic organization
 */

/* ---- PUBLIC INTERFACE (should be in stack.h) ---- */

/**
 * @brief Opaque stack data structure
 *
 * Users cannot access internal structure; must use provided functions.
 * This hides implementation details and allows future changes.
 */
typedef struct Stack Stack;

/**
 * @brief Create a new empty stack
 *
 * Allocates memory for a new stack with given capacity.
 *
 * @param capacity Maximum number of elements
 * @return Pointer to initialized stack, or NULL on allocation failure
 */
Stack* stack_create(size_t capacity);

/**
 * @brief Destroy a stack and free its resources
 *
 * After calling this, the pointer is invalid and must not be used.
 *
 * @param stack Stack to destroy (may be NULL)
 */
void stack_destroy(Stack* stack);

/**
 * @brief Push an element onto the stack
 *
 * @param stack Target stack (must not be NULL)
 * @param value Value to push
 * @return True if successful; false if stack full
 */
bool stack_push(Stack* stack, int value);

/**
 * @brief Pop an element from the stack
 *
 * @param stack Target stack (must not be NULL)
 * @param value Output parameter: receives popped value
 * @return True if successful; false if stack empty
 *
 * If false is returned, value is unchanged.
 */
bool stack_pop(Stack* stack, int* value);

/**
 * @brief Peek at the top element without removing it
 *
 * This is a PURE function: no side effects, depends only on stack state.
 *
 * @param stack Target stack (must not be NULL)
 * @param value Output parameter: receives top value
 * @return True if stack has elements; false if empty
 */
bool stack_peek(const Stack* stack, int* value);

/**
 * @brief Query current number of elements in stack
 *
 * PURE FUNCTION: no side effects.
 *
 * @param stack Target stack (must not be NULL)
 * @return Number of elements currently in stack
 */
size_t stack_size(const Stack* stack);

/**
 * @brief Check if stack is empty
 *
 * PURE FUNCTION: no side effects.
 *
 * @param stack Target stack (must not be NULL)
 * @return True if stack contains no elements
 */
bool stack_is_empty(const Stack* stack);

/* ---- PRIVATE IMPLEMENTATION ---- */

/**
 * Internal stack structure (hidden from users)
 */
struct Stack {
    int* data;           /* Array of elements */
    size_t capacity;     /* Maximum capacity */
    size_t top;          /* Index of next free position */
};

/* ---- HELPER FUNCTIONS (uncommented; flow is obvious) ---- */

/**
 * Allocate and initialize a new stack structure
 *
 * PURE ALLOCATION FUNCTION
 */
static Stack* stack_alloc(size_t capacity) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (!s) return NULL;

    s->data = (int*)malloc(capacity * sizeof(int));
    if (!s->data) {
        free(s);
        return NULL;
    }

    s->capacity = capacity;
    s->top = 0;

    return s;
}

/* ---- PUBLIC FUNCTION IMPLEMENTATIONS ---- */

Stack* stack_create(size_t capacity) {
    /* Precondition check */
    if (capacity == 0) {
        fprintf(stderr, "Error: stack capacity must be > 0\n");
        return NULL;
    }

    return stack_alloc(capacity);
}

void stack_destroy(Stack* stack) {
    if (!stack) return;  /* Safe to call on NULL */

    free(stack->data);
    free(stack);
}

bool stack_push(Stack* stack, int value) {
    /* Precondition: stack must not be NULL */
    assert(stack != NULL);

    /* Check capacity */
    if (stack->top >= stack->capacity) {
        return false;  /* Stack full; no error message (caller handles) */
    }

    /* Add element and advance */
    stack->data[stack->top] = value;
    ++stack->top;

    return true;
}

bool stack_pop(Stack* stack, int* value) {
    /* Preconditions */
    assert(stack != NULL);
    assert(value != NULL);

    /* Check if stack is empty */
    if (stack->top == 0) {
        return false;
    }

    /* Decrement and retrieve */
    --stack->top;
    *value = stack->data[stack->top];

    return true;
}

bool stack_peek(const Stack* stack, int* value) {
    /* Preconditions */
    assert(stack != NULL);
    assert(value != NULL);

    /* Check if empty */
    if (stack->top == 0) {
        return false;
    }

    /* Return top without modifying */
    *value = stack->data[stack->top - 1];

    return true;
}

size_t stack_size(const Stack* stack) {
    assert(stack != NULL);
    return stack->top;
}

bool stack_is_empty(const Stack* stack) {
    assert(stack != NULL);
    return stack->top == 0;
}

/* ---- DEMONSTRATION ---- */

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Chapter 10: Organization & Documentation - Stack Demo    ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    /* Create stack with capacity of 5 */
    Stack* mystack = stack_create(5);
    if (!mystack) {
        fprintf(stderr, "Failed to create stack\n");
        return EXIT_FAILURE;
    }

    /* Push values */
    printf("Pushing values: 10, 20, 30, 40, 50\n");
    for (int i = 1; i <= 5; ++i) {
        int value = i * 10;
        if (stack_push(mystack, value)) {
            printf("  Pushed %d\n", value);
        } else {
            printf("  Failed to push %d (stack full)\n", value);
        }
    }

    printf("\nStack size: %zu\n", stack_size(mystack));

    /* Try to push one more (should fail) */
    printf("\nAttempting to push 60 (should fail)...\n");
    if (!stack_push(mystack, 60)) {
        printf("  Push failed: stack at capacity\n");
    }

    /* Peek at top */
    printf("\nPeeking at top:\n");
    int top;
    if (stack_peek(mystack, &top)) {
        printf("  Top element: %d\n", top);
    }

    /* Pop all values */
    printf("\nPopping all values:\n");
    while (!stack_is_empty(mystack)) {
        int value;
        if (stack_pop(mystack, &value)) {
            printf("  Popped %d\n", value);
        }
    }

    printf("\nStack empty: %s\n", stack_is_empty(mystack) ? "YES" : "NO");

    /* Clean up */
    stack_destroy(mystack);

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Key Organization Principles Demonstrated:               ║\n");
    printf("║  ✓ Separation of interface (header) from implementation  ║\n");
    printf("║  ✓ Opaque data types (users can't access internals)       ║\n");
    printf("║  ✓ Comprehensive Doxygen documentation                   ║\n");
    printf("║  ✓ Pure functions where possible (peek, size, empty)      ║\n");
    printf("║  ✓ Clear lifecycle (create, use, destroy)                ║\n");
    printf("║  ✓ Obvious control flow (minimal nesting)                ║\n");
    printf("║  ✓ Self-documenting code (no redundant comments)          ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;
}
