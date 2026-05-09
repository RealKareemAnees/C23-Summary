# Chapter 10: Organization and Documentation

Source: 10-organizationdocumentation.md

## Exhaustive Concept Breakdown

### Documentation Hierarchy

**TAKEAWAY 10.1**: (What) Function interfaces describe what is done.
**TAKEAWAY 10.2**: (What for) Interface comments document the purpose of a function.
**TAKEAWAY 10.3**: (How) Function code shows how the function is organized.
**TAKEAWAY 10.4**: (In which manner) Code comments explain the manner in which function details are implemented.

### Documentation Priority Order

1. **What** (highest priority): Interface specification
2. **What for**: Purpose/intent of interface
3. **How**: Code structure (should be self-evident)
4. **In which manner** (lowest priority): Implementation details

**Consequence**: Users read interface specs; fewer read explanation; very few read source code. Balance details accordingly.

## 10.1 Interface Documentation

### File Organization: Headers vs Translation Units

**TAKEAWAY 10.5**: Separate interface and implementation.
**TAKEAWAY 10.6**: Document the interface; explain the implementation.

| Type              | Extension | Role                    | Content                                                  |
| ----------------- | --------- | ----------------------- | -------------------------------------------------------- |
| Header files      | `.h`      | Interface specification | Type declarations, function prototypes, public constants |
| Translation units | `.c`      | Implementation          | Function definitions, internal state, algorithms         |

### Doxygen Documentation Standard

**TAKEAWAY 10.7**: Document interfaces thoroughly.

**Doxygen syntax** (widely adopted cross-platform standard):

```c
/**
 * @brief Brief one-line description
 *
 * Detailed description of what function does, any special behavior,
 * mathematical formulas, usage notes.
 *
 * @param param1 Description of first parameter
 * @param param2 Description of second parameter
 * @return Description of return value
 * @see related_function()
 * @see RELATED_HEADER
 */
```

**Common Doxygen keywords:**

- `@brief`: One-line summary
- `@param`: Parameter documentation
- `@return`: Return value description
- `@a`: Inline parameter reference
- `@f$...@f$`: LaTeX mathematical expressions
- `@see`: Cross-references
- `@file`: File-level documentation

### Header File Structure

**TAKEAWAY 10.8**: Structure your code in units that have strong semantic connections.

**Typical header organization:**

```c
#ifndef MODULENAME_H
#define MODULENAME_H 1

#include <necessary_headers.h>

/* @file Module description */

/* Type definitions grouped by semantic unit */
typedef struct module_data module_data;

/* Related enumeration */
enum module_state { IDLE, ACTIVE, SUSPENDED };

/* Function interface declarations (prototypes) */
module_data* module_create(void);
void module_destroy(module_data*);

#endif
```

### Include Guards

Standard pattern to prevent multiple inclusion:

```c
#ifndef UNIQUE_HEADER_GUARD_NAME
#define UNIQUE_HEADER_GUARD_NAME 1
/* header contents */
#endif
```

## 10.2 Implementation Documentation

### Code Quality Through Structure

**TAKEAWAY 10.9**: Implement literally.
**TAKEAWAY 10.10**: Control flow must be obvious.

Good code is self-documenting through:

- Clear variable/function names
- Obvious control flow
- Minimal surprises
- Appropriate comments (not redundant)

### Obfuscating Control Flow (Anti-patterns)

**Problems to avoid:**

1. **Buried jumps**: `break`, `continue`, `return`, `goto` hidden in nested structures
2. **Flyspeck expressions**: Complex operators stacked unnaturally (e.g., `!!++*p–` or `a -> 0`)

### Pure Functions for Code Clarity

**TAKEAWAY 10.13**: Function parameters are passed by value.
**TAKEAWAY 10.14**: Global variables are frowned upon.

**Pure function definition:**

- No effects except return value
- Return value depends only on parameters
- Optimization-friendly: can be moved, parallelized, executed when ready

**Pure vs impure side effects:**

- **Disqualifying effects**: Reads changeable program state (other than args), modifies globals, maintains internal state, performs I/O

**TAKEAWAY 10.15**: Express small tasks as pure functions whenever possible.

### Macros: Usage Guidelines

**TAKEAWAY 10.11**: Macros should not change the control flow in a surprising way.
**TAKEAWAY 10.12**: Function-like macros should syntactically behave like function calls.

**Anti-pattern (avoid):**

```c
#define ERRORCHECK(CODE) if (CODE) return -1   /* Dangerous! */
if (a) ERRORCHECK(x);  /* Dangling else problem */
else puts("a is 0!");
```

**Better pattern (do-while-false):**

```c
#define ERRORCHECK(CODE) \
    do { \
        if (CODE) return -1; \
    } while (false)

if (a) ERRORCHECK(x);  /* Now else attaches to outer if */
else puts("a is 0!");
```

**Macro pitfalls:**

- Missing else causes binding to inner if
- Trailing semicolons terminate outer structures
- Comma operator ambiguity (avoid)
- Continuable expressions bind unexpectedly
- Multiple evaluation if parameter used twice

## Syntax/Types/Functions Dictionary

### Documentation Patterns

| Construct                 | Purpose                    |
| ------------------------- | -------------------------- |
| `/** @brief ... */`       | Brief function description |
| `@param name Description` | Parameter documentation    |
| `@return Description`     | Return value documentation |
| `@see function()`         | Cross-reference            |
| `#ifndef HEADER_H`        | Include guard opening      |
| `#endif`                  | Include guard closing      |

### Macro Defensive Patterns

| Pattern                                                  | Purpose                       |
| -------------------------------------------------------- | ----------------------------- |
| `#define DO_SOMETHING \ do { /* code */ } while (false)` | Safe multi-line macro         |
| `((expression))`                                         | Parenthesize macro parameters |
| Avoid comma operators                                    | Prevent unexpected binding    |

### Code Organization Patterns

| Pattern                                  | Purpose                      |
| ---------------------------------------- | ---------------------------- |
| Header: type definitions + prototypes    | Public interface             |
| Translation unit: implementations        | Private state and algorithms |
| Global module prefix + `_create/destroy` | Lifecycle management         |

## Danger Zone: Pitfalls & Undefined Behavior

### Documentation Pitfalls

1. **Stale documentation**: Outdates with code changes; maintains false info
2. **Overly verbose**: Comments state the obvious (e.g., `++i; // increment i`)
3. **Under-documented interfaces**: Users can't understand usage
4. **Missing preconditions**: Function assumes invalid inputs won't arrive; crashes silently

### Implementation Pitfalls

1. **Global state everywhere**: Hard to test, debug, parallelize
2. **Hidden macro substitutions**: Surprised developers introduce bugs
3. **Dangling else from macros**: Silent semantic changes
4. **Multiple macro evaluation**: `MAX(i++, j++)` increments twice
5. **Complex expressions**: `a -> 0` (arrow operator chaining) confuses readers

### Organization Pitfalls

1. **Poor module boundaries**: Unrelated functions mixed together
2. **Circular dependencies**: Headers include each other directly
3. **No include guards**: Multiple inclusion causes redefinition errors
4. **Monolithic files**: Single .c file with thousands of lines
5. **Public implementation details**: Struct internals visible in header (breaks encapsulation)

## Comprehensive C23 Working Example

```c
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
```

## Summary of Key TAKEAWAYs

1. **Documentation hierarchy**: What → What for → How → In which manner (decreasing priority)
2. **Interface documentation**: Doxygen standard; thorough and machine-readable
3. **Interface vs implementation**: Separate concerns; document interface thoroughly, code should be self-evident
4. **Pure functions**: Better for testing, optimization, parallelization
5. **Obvious control flow**: Avoid buried jumps and complex expressions
6. **Macro safety**: Use do-while(false) wrapper; parenthesize all parameters
7. **Module cohesion**: Group semantically-related functions together
8. **Opaque types**: Hide implementation; enable future changes
9. **Include guards**: Prevent multiple inclusion errors
10. **Comments for non-obvious**: Explain why, not what; code shows how
