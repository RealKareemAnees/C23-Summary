#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/* ============================================================ */
/* STYLE DEMONSTRATION: FORMATTING, NAMING, DOCUMENTATION     */
/* ============================================================ */

/**
 * Example of well-formatted, well-named C code following style guidelines
 */

/* Library-specific prefix to avoid conflicts */
#define MYLIB_MAX_NAME_LENGTH 256
#define MYLIB_DEFAULT_CAPACITY 10

/**
 * @brief A well-documented data structure with descriptive member names
 *
 * This represents a person in our system with essential biographical data.
 * Members have semantic names that clearly indicate their purpose.
 */
typedef struct {
    char full_name[MYLIB_MAX_NAME_LENGTH];  /* Clear, snake_case */
    int age;                                 /* Descriptive */
    double salary;                           /* Amount is self-evident */
    bool is_employed;                        /* Boolean prefix clarity */
} Person;

/**
 * @brief Create a new person with given data
 *
 * @param name Full name (must be non-NULL)
 * @param years Age in years (should be positive)
 * @param pay Annual salary (should be non-negative)
 * @return Person structure initialized with given values
 */
Person create_person(const char* name, int years, double pay) {
    Person p = {0};  /* Initialize all members to 0 */

    /* Be explicit about what we're doing */
    strncpy(p.full_name, name, MYLIB_MAX_NAME_LENGTH - 1);
    p.full_name[MYLIB_MAX_NAME_LENGTH - 1] = '\0';
    p.age = years;
    p.salary = pay;
    p.is_employed = (pay > 0.0);

    return p;
}

/**
 * @brief Print person information in human-readable format
 *
 * @param person The person to display
 */
void print_person(const Person* person) {
    printf("Name: %s\n", person->full_name);
    printf("Age: %d years\n", person->age);
    printf("Salary: $%.2f\n", person->salary);
    printf("Employed: %s\n", person->is_employed ? "Yes" : "No");
}

/**
 * @brief Compute annual tax based on salary
 *
 * Uses simplified tax bracket system:
 * - First $25000: 10%
 * - Next $50000: 15%
 * - Above $75000: 20%
 *
 * @param salary Annual salary in dollars
 * @return Computed tax amount
 */
double compute_tax(double salary) {
    double tax = 0.0;

    /* Tax brackets with clear, self-documenting constants */
    const double bracket1_limit = 25000.0;
    const double bracket2_limit = 75000.0;
    const double bracket1_rate = 0.10;
    const double bracket2_rate = 0.15;
    const double bracket3_rate = 0.20;

    /* Readable conditional logic */
    if (salary <= bracket1_limit) {
        tax = salary * bracket1_rate;
    } else if (salary <= bracket2_limit) {
        tax = (bracket1_limit * bracket1_rate) +
              ((salary - bracket1_limit) * bracket2_rate);
    } else {
        tax = (bracket1_limit * bracket1_rate) +
              ((bracket2_limit - bracket1_limit) * bracket2_rate) +
              ((salary - bracket2_limit) * bracket3_rate);
    }

    return tax;
}

/**
 * @brief Calculate net income (salary minus taxes)
 *
 * @param person Person whose net income to calculate
 * @return Net income in dollars
 */
double get_net_income(const Person* person) {
    if (!person->is_employed) {
        return 0.0;
    }

    double tax = compute_tax(person->salary);
    return person->salary - tax;
}

/**
 * @brief Compare two people by salary for sorting
 *
 * @param a First person
 * @param b Second person
 * @return Negative if a earns less, 0 if equal, positive if a earns more
 */
int compare_by_salary(const void* a, const void* b) {
    const Person* person_a = (const Person*)a;
    const Person* person_b = (const Person*)b;

    /* Return -1, 0, or 1 for proper sorting */
    if (person_a->salary < person_b->salary) return -1;
    if (person_a->salary > person_b->salary) return 1;
    return 0;
}

/**
 * @brief Display statistics for an array of people
 *
 * @param people Array of Person structures
 * @param count Number of people in array
 */
void display_statistics(const Person* people, size_t count) {
    if (count == 0) {
        printf("No people to display.\n");
        return;
    }

    double total_salary = 0.0;
    double total_tax = 0.0;
    double min_salary = people[0].salary;
    double max_salary = people[0].salary;

    /* Clear iteration pattern */
    for (size_t i = 0; i < count; ++i) {
        double salary = people[i].salary;
        double tax = compute_tax(salary);

        total_salary += salary;
        total_tax += tax;

        if (salary < min_salary) min_salary = salary;
        if (salary > max_salary) max_salary = salary;
    }

    double average_salary = total_salary / (double)count;

    /* Well-formatted output */
    printf("\n=== Statistics for %zu People ===\n", count);
    printf("Total Salary:    $%.2f\n", total_salary);
    printf("Average Salary:  $%.2f\n", average_salary);
    printf("Minimum Salary:  $%.2f\n", min_salary);
    printf("Maximum Salary:  $%.2f\n", max_salary);
    printf("Total Taxes:     $%.2f\n", total_tax);
}

/* ============================================================ */
/* MAIN: Demonstrating style principles */
/* ============================================================ */

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║      Chapter 9: Style - Code Organization & Naming        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    /* Create example data with clear, readable initialization */
    Person team[] = {
        create_person("Alice Johnson", 32, 75000.0),
        create_person("Bob Smith", 28, 55000.0),
        create_person("Carol Davis", 45, 95000.0),
        create_person("David Wilson", 26, 35000.0),
    };
    size_t team_size = sizeof(team) / sizeof(team[0]);

    /* Display original data */
    printf("=== Team Members ===\n\n");
    for (size_t i = 0; i < team_size; ++i) {
        print_person(&team[i]);
        printf("Net Income: $%.2f\n\n", get_net_income(&team[i]));
    }

    /* Sort by salary (demonstrating qsort usage) */
    qsort(team, team_size, sizeof(Person), compare_by_salary);

    printf("=== Sorted by Salary (Ascending) ===\n\n");
    for (size_t i = 0; i < team_size; ++i) {
        printf("%2zu. %-20s $%.2f\n", i + 1,
               team[i].full_name, team[i].salary);
    }

    /* Display aggregate statistics */
    display_statistics(team, team_size);

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Key Style Principles Demonstrated:                       ║\n");
    printf("║  ✓ Consistent formatting (braces, indentation)            ║\n");
    printf("║  ✓ Descriptive naming (actions, attributes, concepts)     ║\n");
    printf("║  ✓ Comprehensive comments (not obvious, explains why)     ║\n");
    printf("║  ✓ Semantic member names (full_name not fn, age not yr)  ║\n");
    printf("║  ✓ Constants with meaning (MYLIB_MAX_NAME_LENGTH)         ║\n");
    printf("║  ✓ Readable control flow (no obfuscation)                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;
}
