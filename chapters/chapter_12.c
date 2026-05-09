#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* === 4.1 Object Representation Inspection via Unions === */

typedef union {
    uint32_t val;
    unsigned char bytes[sizeof(uint32_t)];
} uint32Inspect;

void inspect_endianness(void) {
    printf("\n--- Endianness Inspection ---\n");

    uint32Inspect num = { .val = 0x12345678 };
    printf("Value: 0x%X\n", num.val);

    printf("Byte order (first to last): ");
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        printf("0x%02X ", num.bytes[i]);
    }
    printf("\n");

    if (num.bytes[0] == 0x78) {
        printf("-> Little-endian system\n");
    } else {
        printf("-> Big-endian system\n");
    }
}

/* === 4.2 Alignment Requirements === */

void demonstrate_alignment(void) {
    printf("\n--- Alignment Information ---\n");

    printf("Alignment of char:           %zu bytes\n", alignof(char));
    printf("Alignment of short:          %zu bytes\n", alignof(short));
    printf("Alignment of int:            %zu bytes\n", alignof(int));
    printf("Alignment of long long:      %zu bytes\n", alignof(long long));
    printf("Alignment of double:         %zu bytes\n", alignof(double));
    printf("Alignment of int*:           %zu bytes\n", alignof(int*));
}

/* === 4.3 Forced Alignment with alignas === */

void demonstrate_aligned_storage(void) {
    printf("\n--- Forced Alignment ---\n");

    // Force alignment to double size
    alignas(sizeof(double) * 4) char buffer[64];

    printf("buffer alignment: %zu bytes (requested: %zu)\n",
           (size_t)((uintptr_t)buffer % 16 == 0 ? 16 : 8),
           alignof(double) * 4);
}

/* === 4.4 Void Pointer Generic Programming === */

typedef struct {
    int id;
    char name[32];
} Person;

void process_generic(void* obj, const char* type_name) {
    printf("\n--- Generic Processing ---\n");
    printf("Processing %s object at %p\n", type_name, obj);

    // Cast back to known type
    if (strcmp(type_name, "Person") == 0) {
        Person* p = (Person*)obj;
        printf("  ID: %d, Name: %s\n", p->id, p->name);
    }
}

/* === 4.5 Pointer Arithmetic and Effective Types === */

void demonstrate_pointer_arithmetic(void) {
    printf("\n--- Pointer Arithmetic ---\n");

    int arr[5] = { 10, 20, 30, 40, 50 };
    int* p = &arr[0];

    printf("Array address: %p\n", (void*)p);
    printf("Element at p[0]: %d\n", *p);
    printf("Element at p[2]: %d\n", p[2]);

    // Pointer difference
    int* end = &arr[4];
    ptrdiff_t diff = end - p;
    printf("Difference (end - start): %td elements\n", diff);
}

/* === 4.6 Union-based Type Overlay (Safe) === */

typedef union {
    struct {
        uint16_t low;
        uint16_t high;
    } parts;
    uint32_t full;
} uint32Parts;

void demonstrate_union_overlay(void) {
    printf("\n--- Union Type Overlay ---\n");

    uint32Parts num = { .full = 0xDEADBEEF };
    printf("Full value: 0x%X\n", num.full);
    printf("Low 16 bits: 0x%X\n", num.parts.low);
    printf("High 16 bits: 0x%X\n", num.parts.high);
}

/* === 4.7 Byte-level Inspection (Proper Cast Usage) === */

void inspect_double_bytes(double d) {
    printf("\n--- Double Byte Inspection ---\n");
    printf("Double value: %g\n", d);
    printf("Byte representation: ");

    // PROPER cast: from object pointer to unsigned char pointer
    unsigned char* bytes = (unsigned char*)&d;
    for (size_t i = 0; i < sizeof(double); ++i) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

/* === 4.8 Aliasing Rules Demonstration === */

void demonstrate_aliasing(void) {
    printf("\n--- Aliasing Rules ---\n");

    // Same-type pointers CAN alias
    int x = 10;
    int y = 20;
    int* p1 = &x;
    int* p2 = &y;
    printf("p1 (%p) and p2 (%p) are different: %s\n",
           (void*)p1, (void*)p2,
           p1 != p2 ? "yes (aliasing allowed)" : "no");

    // Different-type pointers CANNOT alias (compiler optimization)
    int i = 42;
    double d = 3.14;
    int* pi = &i;
    double* pd = &d;
    printf("int* and double* are different: yes (no aliasing rule)\n");
}

/* === 4.9 Effective Type Restrictions === */

void demonstrate_effective_types(void) {
    printf("\n--- Effective Type Rules ---\n");

    int x = 100;
    printf("Variable x (int): %d\n", x);

    // Valid: access through declared type
    printf("Access via int*: %d\n", *(int*)&x);

    // Valid: access as unsigned char array
    printf("Access as bytes: ");
    unsigned char* bytes = (unsigned char*)&x;
    for (size_t j = 0; j < sizeof(int); ++j) {
        printf("%02X ", bytes[j]);
    }
    printf("\n");

    // INVALID (UB): access through different type
    // printf("Access via double*: %g\n", *(double*)&x);  // DON'T
}

int main(void) {
    printf("=== Chapter 12: C Memory Model ===\n");

    inspect_endianness();
    demonstrate_alignment();
    demonstrate_aligned_storage();

    Person alice = { 1, "Alice" };
    process_generic(&alice, "Person");

    demonstrate_pointer_arithmetic();
    demonstrate_union_overlay();

    inspect_double_bytes(3.14159);
    demonstrate_aliasing();
    demonstrate_effective_types();

    printf("\n=== End Chapter 12 ===\n");
    return 0;
}
