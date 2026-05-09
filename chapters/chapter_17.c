#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* === 4.1 Safe Macro with Parenthesization === */

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

void demo_safe_macros(void) {
    printf("\n--- Safe Macros with Parenthesization ---\n");
    printf("MAX(3, 7) = %d\n", MAX(3, 7));
    printf("MAX(3, 7) * 2 = %d (not 3 * 2)\n", MAX(3, 7) * 2);
    printf("ABS(-5) = %d\n", ABS(-5));
}

/* === 4.2 Variadic Macro === */

#define LOG(level, fmt, ...) \
    printf("[%s] " fmt "\n", level, ##__VA_ARGS__)

#define DEBUG(fmt, ...) LOG("DEBUG", fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) LOG("ERROR", fmt, ##__VA_ARGS__)

void demo_variadic(void) {
    printf("\n--- Variadic Macros ---\n");
    DEBUG("Starting process...");
    DEBUG("Value: %d, Name: %s", 42, "test");
    ERROR("Failed operation");
}

/* === 4.3 String Literal Enforcement === */

#define SAFE_PRINT(fmt) \
    do { \
        printf("Safe: " fmt " [from %s:%d]\n", __func__, __LINE__); \
    } while(0)

void demo_string_enforcement(void) {
    printf("\n--- String Literal Enforcement ---\n");
    SAFE_PRINT("Using string literal");
    // SAFE_PRINT(user_string);  // Compile error: can't concatenate with variable
}

/* === 4.4 do-while-false Idiom === */

#define SWAP(a, b) \
    do { \
        typeof(a) _tmp = (a); \
        (a) = (b); \
        (b) = _tmp; \
    } while(0)

void demo_swap(void) {
    printf("\n--- do-while-false Swap Idiom ---\n");

    int x = 10, y = 20;
    printf("Before: x=%d, y=%d\n", x, y);
    SWAP(x, y);
    printf("After: x=%d, y=%d\n", x, y);

    double a = 1.5, b = 2.5;
    SWAP(a, b);
    printf("Double swap: a=%f, b=%f\n", a, b);
}

/* === 4.5 Stringify Operator (#) === */

#define STR(x) #x
#define STR_VALUE(x) "Value of " #x " is: %d\n"

void demo_stringify(void) {
    printf("\n--- Stringify Operator ---\n");

    int value = 42;
    printf("STR(value) = %s\n", STR(value));
    printf(STR_VALUE(value), value);

    printf("STR(MAX) = %s\n", STR(MAX));  // Macro name stringified
}

/* === 4.6 Token Concatenation (##) === */

#define MAKE_PAIR_TYPE(T) \
    typedef struct { \
        T first; \
        T second; \
    } pair_ ## T

// Create specific pair types
MAKE_PAIR_TYPE(int);
MAKE_PAIR_TYPE(double);

void demo_concatenation(void) {
    printf("\n--- Token Concatenation ---\n");

    pair_int pi = { 1, 2 };
    pair_double pd = { 1.5, 2.5 };

    printf("pair_int: (%d, %d)\n", pi.first, pi.second);
    printf("pair_double: (%f, %f)\n", pd.first, pd.second);
}

/* === 4.7 Predefined Macros === */

void demo_predefined(void) {
    printf("\n--- Predefined Macros ---\n");
    printf("__FILE__: %s\n", __FILE__);
    printf("__LINE__: %d\n", __LINE__);
    printf("__func__: %s\n", __func__);
    printf("__DATE__: %s\n", __DATE__);
    printf("__TIME__: %s\n", __TIME__);
}

/* === 4.8 Conditional Compilation === */

#define DEBUG_MODE 1

void demo_conditional(void) {
    printf("\n--- Conditional Compilation ---\n");

#if DEBUG_MODE
    printf("Debug mode ENABLED\n");
#else
    printf("Debug mode DISABLED\n");
#endif

#ifdef __STDC_NO_VLA__
    printf("VLAs are NOT supported on this platform\n");
#else
    printf("VLAs are supported\n");
#endif
}

/* === 4.9 Type-Generic with _Generic (C11+) === */

#define print_value(x) \
    _Generic((x), \
        int: printf("int: %d\n", (x)), \
        double: printf("double: %f\n", (x)), \
        char*: printf("string: %s\n", (x)), \
        unsigned: printf("unsigned: %u\n", (x)) \
    )

void demo_generic(void) {
    printf("\n--- Type-Generic Macros ---\n");
    print_value(42);
    print_value(3.14);
    print_value("hello");
    print_value(100U);
}

int main(void) {
    printf("=== Chapter 17: Function-like Macros ===\n");

    demo_safe_macros();
    demo_variadic();
    demo_string_enforcement();
    demo_swap();
    demo_stringify();
    demo_concatenation();
    demo_predefined();
    demo_conditional();
    demo_generic();

    printf("\n=== End Chapter 17 ===\n");
    return 0;
}
