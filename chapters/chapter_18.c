#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <tgmath.h>
#include <stdarg.h>

/* === 4.1 _Generic for Type-Specific Handling === */

#define print_value(x) _Generic((x), \
    int: printf("int: %d\n", (x)), \
    float: printf("float: %f\n", (x)), \
    double: printf("double: %f\n", (x)), \
    char*: printf("string: %s\n", (x)), \
    default: printf("unknown type\n") \
)

void demo_generic(void) {
    printf("\n--- _Generic Compile-Time Selection ---\n");
    print_value(42);
    print_value(3.14f);
    print_value(2.718);
    print_value("Generic!");
}

/* === 4.2 Type-Generic Math Functions === */

void demo_tgmath(void) {
    printf("\n--- Type-Generic Math (<tgmath.h>) ---\n");

    // tgmath.h macros dispatch to float/double/long double versions
    float f = 0.5f;
    double d = 0.5;
    long double ld = 0.5L;

    printf("sin(float 0.5) = %f\n", sin(f));      // calls sinf()
    printf("sin(double 0.5) = %f\n", sin(d));     // calls sin()
    printf("sin(long double 0.5) = %Lf\n", sin(ld));  // calls sinl()
}

/* === 4.3 Type Inference (C23) === */

void demo_auto(void) {
    printf("\n--- Type Inference (auto, C23) ---\n");

    auto x = 42;                    // int
    auto y = 3.14;                  // double
    auto z = "hello";               // char const*

    printf("x (auto) = %d\n", x);
    printf("y (auto) = %f\n", y);
    printf("z (auto) = %s\n", z);

    // typeof for meta-programming
    typedef typeof(x) int_type;
    typedef typeof(y) double_type;

    int_type a = 100;
    double_type b = 2.5;
    printf("typeof(x): a = %d, typeof(y): b = %f\n", a, b);
}

/* === 4.4 typeof_unqual (C23) === */

void demo_typeof_unqual(void) {
    printf("\n--- typeof_unqual (Remove Qualifiers, C23) ---\n");

    const int cx = 10;
    volatile double vd = 3.14;

    // typeof preserves qualifiers
    typeof(cx) y1 = 20;      // y1 is const int
    // y1 = 30;  // Error: const

    // typeof_unqual strips qualifiers
    typeof_unqual(cx) y2 = 30;     // y2 is int
    typeof_unqual(vd) y3 = 2.71;   // y3 is double

    printf("y2 (unqualified) = %d\n", y2);
    printf("y3 (unqualified) = %f\n", y3);
}

/* === 4.5 Comparison Functions for qsort === */

typedef struct {
    int value;
    char name[20];
} Record;

int compare_records_by_value(void const* a, void const* b) {
    Record const* ra = (Record const*)a;
    Record const* rb = (Record const*)b;
    return (ra->value > rb->value) - (ra->value < rb->value);
}

void demo_qsort(void) {
    printf("\n--- Generic Sort (qsort) ---\n");

    Record records[] = {
        {30, "Charlie"},
        {10, "Alice"},
        {20, "Bob"}
    };

    size_t n = sizeof records / sizeof *records;

    printf("Before sort:\n");
    for (size_t i = 0; i < n; ++i) {
        printf("  %d: %s\n", records[i].value, records[i].name);
    }

    qsort(records, n, sizeof *records, compare_records_by_value);

    printf("After sort:\n");
    for (size_t i = 0; i < n; ++i) {
        printf("  %d: %s\n", records[i].value, records[i].name);
    }
}

/* === 4.6 Type-Safe String Search (C23) === */

void demo_type_generic_search(void) {
    printf("\n--- Type-Generic String Search (C23) ---\n");

    // Const string search returns const pointer
    char const cs[] = "hello world";
    char const* pos1 = strstr(cs, "world");
    if (pos1) printf("Found in const: %s\n", pos1);

    // Mutable string search returns mutable pointer
    char ms[] = "hello world";
    char* pos2 = strstr(ms, "world");
    if (pos2) {
        printf("Found in mutable: %s\n", pos2);
        pos2[0] = 'W';  // Can modify
        printf("Modified: %s\n", ms);
    }
}

/* === 4.7 Macro-Based Type Genericity === */

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN_OF_THREE(a, b, c) MIN(MAX(a, b), c)

#define SWAP(a, b) \
    do { \
        typeof(a) _tmp = (a); \
        (a) = (b); \
        (b) = _tmp; \
    } while(0)

void demo_macro_genericity(void) {
    printf("\n--- Macro-Based Type Genericity ---\n");

    int i1 = 5, i2 = 10;
    SWAP(i1, i2);
    printf("After SWAP: i1=%d, i2=%d\n", i1, i2);

    double d1 = 1.5, d2 = 2.5;
    SWAP(d1, d2);
    printf("After SWAP: d1=%f, d2=%f\n", d1, d2);

    printf("MAX(3.5, 7.2) = %f\n", (double)MAX(3.5, 7.2));
}

/* === 4.8 Operator Type-Genericity (Conversions) === */

void demo_operator_genericity(void) {
    printf("\n--- Operator Type-Genericity ---\n");

    int i = 10;
    float f = 2.5f;
    double d = 3.0;

    // Operators work across types via conversions
    auto result1 = i + f;      // int + float → float
    auto result2 = f * d;      // float * double → double
    auto result3 = d / i;      // double / int → double

    printf("10 + 2.5f = %f\n", result1);
    printf("2.5f * 3.0 = %f\n", result2);
    printf("3.0 / 10 = %f\n", result3);
}

int main(void) {
    printf("=== Chapter 18: Type-Generic Programming ===\n");

    demo_generic();
    demo_tgmath();
    demo_auto();
    demo_typeof_unqual();
    demo_qsort();
    demo_type_generic_search();
    demo_macro_genericity();
    demo_operator_genericity();

    printf("\n=== End Chapter 18 ===\n");
    return 0;
}
