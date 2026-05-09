#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/* 1. DERIVED DATA TYPES SHOWCASE */

/* Arrays: Homogeneous grouping */
void demo_arrays(void) {
    printf("=== ARRAYS ===\n");

    /* Fixed-length array (CLA) */
    double temps[5] = { 20.0, 21.5, 19.3, 22.1, 20.8 };
    size_t len = sizeof(temps) / sizeof(temps[0]);
    printf("Daily temps (count=%zu): ", len);
    for (size_t i = 0; i < len; ++i) {
        printf("%.1f ", temps[i]);
    }
    printf("\n");

    /* Multidimensional array */
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    printf("Matrix[1][2] = %d\n", matrix[1][2]);

    /* Designated initializers */
    int sparse[10] = { [2] = 5, [7] = 99 };
    printf("sparse[2]=%d, sparse[7]=%d, sparse[0]=%d (default)\n",
           sparse[2], sparse[7], sparse[0]);
}

/* Strings: Special 0-terminated char arrays */
void demo_strings(void) {
    printf("\n=== STRINGS ===\n");

    /* String declaration and operations */
    char name[] = "Modern C";  /* Automatically 0-terminated */
    printf("String: \"%s\"\n", name);
    printf("Length: %zu\n", strlen(name));

    /* String comparison */
    char greeting[20];
    strcpy(greeting, "Hello, ");
    printf("After strcpy: \"%s\"\n", greeting);

    /* String functions */
    const char* source = "Device:Hostname:Port";
    char buffer[256];
    strcpy(buffer, source);

    /* Manual parsing without strtok (safer) */
    const char* ptr = buffer;
    int part = 0;
    for (size_t i = 0; i < strlen(buffer); ++i) {
        if (buffer[i] == ':') {
            buffer[i] = '\0';
            printf("Part %d: \"%s\"\n", part++, ptr);
            ptr = &buffer[i + 1];
        }
    }
    printf("Part %d: \"%s\"\n", part, ptr);

    /* memcpy for non-0-terminated data */
    unsigned char raw_data[6] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21 };
    char text[10] = { };
    memcpy(text, raw_data, 5);
    text[5] = '\0';
    printf("Raw bytes as text: \"%s\"\n", text);
}

/* Pointers: Opaque references (brief introduction) */
void demo_pointers(void) {
    printf("\n=== POINTERS (Introduction) ===\n");

    /* Valid pointer */
    int value = 42;
    int const* ptr_to_value = &value;
    printf("Pointed value: %d\n", *ptr_to_value);

    /* Null pointer */
    int* null_ptr = nullptr;
    printf("Null pointer is %s\n", (null_ptr ? "valid" : "null"));

    /* Pointer to string literal */
    char const* string_ptr = "C23 Standard";
    printf("String pointer points to: \"%s\"\n", string_ptr);
}

/* Structures: Aggregate heterogeneous data */
void demo_structures(void) {
    printf("\n=== STRUCTURES ===\n");

    /* Simple structure */
    struct Date {
        unsigned char day;     /* 1-31: 1 byte */
        unsigned char month;   /* 1-12: 1 byte */
        int16_t year;         /* Full year: 2 bytes */
    };                         /* Total: 4 bytes (no padding typically) */

    struct Date today = {
        .day = 7,
        .month = 4,
        .year = 2026
    };
    printf("Today: %u/%u/%d\n", today.day, today.month, today.year);

    /* Nested structures */
    struct Person {
        char name[50];
        struct DateOfBirth {
            struct Date date;
            unsigned char hour;
        } dob;
    };

    struct Person alice = {
        .name = "Alice",
        .dob = {
            .date = { 15, 3, 1990 },
            .hour = 14
        }
    };
    printf("Person: %s, DOB: %u/%u/%d @ %02u:00\n",
           alice.name, alice.dob.date.day, alice.dob.date.month,
           alice.dob.date.year, alice.dob.hour);

    /* Structure assignment */
    struct Date copy = today;
    printf("Copied date: %u/%u/%d\n", copy.day, copy.month, copy.year);

    /* Bit-fields for space efficiency */
    struct TimeCompact {
        unsigned char second : 6;  /* 0-59: 6 bits */
        unsigned char minute : 6;  /* 0-59: 6 bits */
        unsigned char hour : 5;    /* 0-23: 5 bits */
    };                             /* Total: 2 bytes vs 3 without bit-fields */

    struct TimeCompact now = { .second = 45, .minute = 30, .hour = 14 };
    printf("Compact time: %02u:%02u:%02u (uses ~2 bytes)\n",
           now.hour, now.minute, now.second);
}

/* Arrays of structures */
void demo_array_of_structs(void) {
    printf("\n=== ARRAYS OF STRUCTURES ===\n");

    struct Student {
        char name[30];
        float gpa;
        int id;
    };

    struct Student roster[3] = {
        { .name = "Alice", .gpa = 3.9f, .id = 101 },
        { .name = "Bob", .gpa = 3.7f, .id = 102 },
        { .name = "Carol", .gpa = 3.95f, .id = 103 }
    };

    printf("Roster:\n");
    for (size_t i = 0; i < 3; ++i) {
        printf("  %s (ID: %d, GPA: %.2f)\n",
               roster[i].name, roster[i].id, roster[i].gpa);
    }
}

/* typedef: Creating type aliases */
void demo_typedef(void) {
    printf("\n=== TYPEDEF ===\n");

    typedef unsigned int uint;
    typedef struct {
        uint width;
        uint height;
    } Dimensions;

    Dimensions screen = { .width = 1920, .height = 1080 };
    printf("Screen: %u x %u\n", screen.width, screen.height);
}

/* Function that modifies struct passed by value */
struct ColorRGB add_brightness(struct ColorRGB color, unsigned char delta) {
    /* Modifications affect only local copy, not original */
    color.red = (color.red + delta > 255) ? 255 : color.red + delta;
    color.green = (color.green + delta > 255) ? 255 : color.green + delta;
    color.blue = (color.blue + delta > 255) ? 255 : color.blue + delta;
    return color;  /* Return modified copy */
}

struct ColorRGB {
    unsigned char red, green, blue;
};

void demo_struct_passbyvalue(void) {
    printf("\n=== STRUCT PASS-BY-VALUE ===\n");

    struct ColorRGB original = { .red = 200, .green = 100, .blue = 50 };
    printf("Original color: R=%u, G=%u, B=%u\n",
           original.red, original.green, original.blue);

    struct ColorRGB brightened = add_brightness(original, 30);
    printf("After brightening: R=%u, G=%u, B=%u\n",
           brightened.red, brightened.green, brightened.blue);
    printf("Original unchanged: R=%u, G=%u, B=%u\n",
           original.red, original.green, original.blue);
}

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Chapter 6: Derived Data Types - C23 Comprehensive Demo   ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    demo_arrays();
    demo_strings();
    demo_pointers();
    demo_structures();
    demo_array_of_structs();
    demo_typedef();
    demo_struct_passbyvalue();

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                  Demo Complete                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return 0;
}
