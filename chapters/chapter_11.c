#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <math.h>

/* ============================================================ */
/* 1. ADDRESS-OF & DEREFERENCE OPERATORS */
/* ============================================================ */

void demo_address_dereference(void) {
    printf("=== Address-Of & Dereference Operators ===\n");

    int value = 42;
    int* ptr = &value;  /* ptr points to value */

    printf("Value: %d\n", value);
    printf("Address: %p\n", (void*)&value);
    printf("Pointer contents: %p\n", (void*)ptr);
    printf("Dereferenced pointer: %d\n", *ptr);

    /* Modify through pointer */
    *ptr = 100;
    printf("After *ptr = 100: value = %d\n", value);
}

/* ============================================================ */
/* 2. SWAP FUNCTION (NON-PURE): MODIFYING CALLER'S OBJECTS */
/* ============================================================ */

void swap_ints(int* p0, int* p1) {
    int tmp = *p0;
    *p0 = *p1;
    *p1 = tmp;
}

void demo_swap(void) {
    printf("\n=== Swap Function (Non-Pure) ===\n");

    int a = 10, b = 20;
    printf("Before: a=%d, b=%d\n", a, b);

    swap_ints(&a, &b);  /* Pass addresses */
    printf("After swap_ints(&a, &b): a=%d, b=%d\n", a, b);
}

/* ============================================================ */
/* 3. POINTER ARITHMETIC: WALKING ARRAYS */
/* ============================================================ */

double array_sum_indexed(size_t len, const double* arr) {
    double sum = 0.0;
    for (size_t i = 0; i < len; ++i) {
        sum += *(arr + i);  /* arr + i = address of i-th element */
    }
    return sum;
}

double array_sum_walking(size_t len, const double* arr) {
    double sum = 0.0;
    const double* end = arr + len;
    for (const double* p = arr; p < end; ++p) {
        sum += *p;  /* Walk through array */
    }
    return sum;
}

double array_sum_backward(size_t len, const double* arr) {
    double sum = 0.0;
    const double* p = arr + len - 1;  /* Start at last */
    do {
        sum += *p;
        --p;
    } while (p >= arr);  /* Continue downward */
    return sum;
}

void demo_array_arithmetic(void) {
    printf("\n=== Pointer Arithmetic: Array Walking ===\n");

    double data[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    size_t count = sizeof(data) / sizeof(data[0]);

    printf("Array: ");
    for (size_t i = 0; i < count; ++i) printf("%.1f ", data[i]);
    printf("\n");

    double sum1 = array_sum_indexed(count, data);
    double sum2 = array_sum_walking(count, data);
    double sum3 = array_sum_backward(count, data);

    printf("Sum (indexed): %.1f\n", sum1);
    printf("Sum (walking forward): %.1f\n", sum2);
    printf("Sum (walking backward): %.1f\n", sum3);
}

/* ============================================================ */
/* 4. POINTER DIFFERENCE: FINDING POSITION */
/* ============================================================ */

int find_positive(size_t len, const double* arr) {
    const double* end = arr + len;

    for (const double* p = arr; p < end; ++p) {
        if (*p < 0.0) {
            ptrdiff_t pos = p - arr;  /* Distance from start */
            printf("Found negative at index %td: %.1f\n", pos, *p);
            return (int)pos;
        }
    }
    return -1;  /* Not found */
}

void demo_pointer_difference(void) {
    printf("\n=== Pointer Difference ===\n");

    double values[] = { 1.0, 2.0, -3.0, 4.0, -5.0 };
    size_t count = sizeof(values) / sizeof(values[0]);

    find_positive(count, values);
    find_positive(count, values);
}

/* ============================================================ */
/* 5. STRUCT POINTERS: ARROW OPERATOR */
/* ============================================================ */

typedef struct {
    char name[50];
    int age;
    double salary;
} Employee;

void display_employee(const Employee* emp) {
    if (!emp) {
        printf("(null employee)\n");
        return;
    }
    printf("Name: %s, Age: %d, Salary: $%.2f\n",
           emp->name, emp->age, emp->salary);
}

void increase_salary(Employee* emp, double percent) {
    if (emp) {
        emp->salary *= (1.0 + percent / 100.0);
    }
}

void demo_struct_pointers(void) {
    printf("\n=== Struct Pointers ===\n");

    Employee alice = { "Alice", 32, 75000.0 };
    Employee* eptr = &alice;

    printf("Direct access: %s, age %d\n",
           alice.name, alice.age);
    printf("Pointer access: %s, age %d\n",
           eptr->name, eptr->age);
    printf("Before raise: ");
    display_employee(eptr);

    increase_salary(eptr, 10);
    printf("After 10%% raise: ");
    display_employee(eptr);
}

/* ============================================================ */
/* 6. NULL POINTER TESTING */
/* ============================================================ */

void demo_null_pointers(void) {
    printf("\n=== Null Pointer Handling ===\n");

    int value = 42;
    int* valid_ptr = &value;
    int* null_ptr = nullptr;

    printf("Valid pointer: %s\n", valid_ptr ? "non-null" : "null");
    printf("Null pointer: %s\n", null_ptr ? "non-null" : "null");

    /* Safe null-aware access */
    if (valid_ptr) {
        printf("Dereferencing valid ptr: %d\n", *valid_ptr);
    }

    if (!null_ptr) {
        printf("Null pointer detected; skipping dereference\n");
    }
}

/* ============================================================ */
/* 7. ARRAY NOTATION WITH POINTERS */
/* ============================================================ */

void print_as_array(size_t len, int const arr[len]) {
    printf("Array (via pointer): ");
    for (size_t i = 0; i < len; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void demo_array_notation(void) {
    printf("\n=== Array Notation with Pointers ===\n");

    int nums[] = { 10, 20, 30, 40, 50 };
    size_t count = sizeof(nums) / sizeof(nums[0]);

    int* ptr = nums;  /* Decay to pointer */
    printf("Length of array: %zu\n", count);
    printf("Length via sizeof pointer: %zu\n",
           sizeof(ptr) / sizeof(int));  /* WRONG! */

    print_as_array(count, ptr);
}

/* ============================================================ */
/* 8. POINTER CHAINS: MULTIPLE LEVELS */
/* ============================================================ */

void demo_pointer_chains(void) {
    printf("\n=== Multi-Level Pointers ===\n");

    int value = 99;
    int* ptr1 = &value;           /* Pointer to int */
    int** ptr2 = &ptr1;           /* Pointer to pointer to int */

    printf("Value: %d\n", value);
    printf("Via *ptr1: %d\n", *ptr1);
    printf("Via **ptr2: %d\n", **ptr2);

    /* Modify through chain */
    **ptr2 = 200;
    printf("After **ptr2 = 200: value = %d\n", value);
}

/* ============================================================ */
/* 9. ARRAY OF POINTERS */
/* ============================================================ */

void demo_array_of_pointers(void) {
    printf("\n=== Array of Pointers ===\n");

    int a = 10, b = 20, c = 30;
    int* arr[3] = { &a, &b, &c };  /* Array of pointers */

    printf("Pointed values via array:\n");
    for (int i = 0; i < 3; ++i) {
        printf("  arr[%d] -> %d\n", i, *arr[i]);
    }
}

/* ============================================================ */
/* MAIN COORDINATION */
/* ============================================================ */

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║        Chapter 11: Pointers - C23 Comprehensive Demo      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    demo_address_dereference();
    demo_swap();
    demo_array_arithmetic();
    demo_pointer_difference();
    demo_struct_pointers();
    demo_null_pointers();
    demo_array_notation();
    demo_pointer_chains();
    demo_array_of_pointers();

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    Demo Complete                           ║\n");
    printf("║         All pointer operations demonstrated!              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;
}
