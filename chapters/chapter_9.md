# Chapter 9: Style

Source: 09-style.md

## Exhaustive Concept Breakdown

### Core Philosophy

**TAKEAWAY 9.1**: All C code must be readable.

Programs serve dual purpose: (1) instructions to compiler, (2) documentation for humans (users, maintainers, legally accountable parties). Code readability fundamentally depends on two factors: physical ability (short-term memory, visual field ~80x30 characters) and cultural context (project conventions).

**TAKEAWAY 9.2**: Short-term memory and the field of vision are small.
**TAKEAWAY 9.3**: Coding style is not a question of taste but of culture.
**TAKEAWAY 9.4**: Each established project constitutes its own cultural space.

## 9.1 Formatting Rules

**TAKEAWAY 9.5**: Choose a consistent strategy for white space and other text formatting.

### Standard Conventions (Used Throughout This Book)

- **Opening braces**: Prefix notation (opening `{` at end of line)
- **Type modifiers**: Bind to left (e.g., `const int*` not `int * const`)
- **Function parens**: Bind to left; condition parens separated with space
- **Ternary operators**: Spaces around `?` and `:`
- **Punctuation**: No space before (`,`, `;`, `:`); one space or newline after
- **Indentation**: Automatic formatting tools (never manual typing)

**TAKEAWAY 9.6**: Have your text editor automatically format your code correctly.

**Recommended tools**:

- **Emacs**: Understands C structure
- **astyle**: Command-line formatting
- **Project enforcement**: Big projects automate formatting for all code

## 9.2 Naming Conventions

**TAKEAWAY 9.7**: Choose a consistent naming policy for all identifiers.

### Technical Restrictions: Reserved Names

**TAKEAWAY 9.8**: Any identifier that is visible in a header file must be conforming.

**Reserved identifiers** (don't use):

1. Starting with `__` (double underscore) or uppercase + underscore: Reserved for extensions/internals
2. Starting with single `_`: Reserved for file-scope identifiers, struct/union/enum tags
3. All-caps identifiers: Reserved for macros
4. Predefined meanings: All C library functions (e.g., `str*`, `E*` prefixes, `*_t` suffix)

### Namespace Pollution Prevention

**TAKEAWAY 9.9**: Don't pollute the global space of identifiers.

Expose only types/functions that are part of public API. Use prefixes to prevent collisions:

- Standard example: `pthread_*` (POSIX threads)
- Library example: `p99_` for public API, `p00_` for internals

### Semantic Naming: The Four Categories

**TAKEAWAY 9.10**: Names must be recognizable and quickly distinguishable.

| Category             | Rule                           | Examples                                                    |
| -------------------- | ------------------------------ | ----------------------------------------------------------- |
| **Type names**       | Identify a concept             | `time` (struct timespec), `list`, `person`                  |
| **Global constants** | Identify special artifacts     | `M_PI` (mathematical constant), `SIZE_MAX` (platform limit) |
| **Global variables** | Identify shared state          | `toto_initialized`, `visited_entries` (rare; frowned upon)  |
| **Functions**        | Identify actions (often verbs) | `strcmp`, `isless`, `getFlag()`, `setFlag()`                |

**TAKEAWAY 9.13**: A type name identifies a concept.
**TAKEAWAY 9.14**: A global constant identifies an artifact.
**TAKEAWAY 9.15**: A global variable identifies state.
**TAKEAWAY 9.16**: A function or functional macro identifies an action.

### Naming Schemes

| Scheme             | Example                                | Issues                                  |
| ------------------ | -------------------------------------- | --------------------------------------- |
| CamelCase          | `myLineNumber`                         | Obscures line of text; harder to read   |
| snake_case         | `my_line_number`                       | More readable; still occupies space     |
| Hungarian notation | `szName` (sz = string zero-terminated) | Type-dependent; breaks with API changes |

**TAKEAWAY 9.11**: Naming is a creative act.

**Pragmatic approach**: Single-letter loop variables (i, j, n, m) acceptable in restricted scope; multi-word identifiers need clarity; avoid abbreviations unless domain-standard.

**TAKEAWAY 9.12**: File scope identifiers must be comprehensive.

## 9.3 Internationalization

**TAKEAWAY 9.17**: The natural language of a project should be chosen to accommodate the majority of the participants.

### Unicode Support in C23

- **Identifiers**: Unicode allowed via Normalization Form C (UAX #31)
- **Character composition**: é (U+00E9) vs é (e + accent U+0301) normalized to single form
- **Ambiguities**: Greek Alpha vs Latin A (same glyph) → use only if intentional

**TAKEAWAY 9.18**: Alphabetic letters are only allowed in identifiers if they map to themselves for Normalization Form C.
**TAKEAWAY 9.19**: Only use alphabetic letters in identifiers if they originate directly from natural languages or they are clearly distinguishable from all natural languages.
**TAKEAWAY 9.20**: Only use letters from different scripts or variations of decimal digits in identifiers if they are clearly distinguishable from each other.

## Syntax/Types/Functions Dictionary

### Formatting Constructs

| Pattern                          | Purpose                       |
| -------------------------------- | ----------------------------- |
| `if (cond) { ... } else { ... }` | Readable control structure    |
| `for (init; cond; incr) { ... }` | Standard loop formatting      |
| `int x = 10;  /* value */ `      | Trailing comment with spacing |

### Naming Patterns

| Pattern             | Purpose                                                        |
| ------------------- | -------------------------------------------------------------- |
| `UPPERCASE_MACRO`   | Macro names (all-caps)                                         |
| `file_scope_func()` | Internal function (leading underscore avoided in single-scope) |
| `public_api_thing`  | Public interface (clear, descriptive)                          |
| `prefix_action()`   | Library function (clear prefix preventing conflicts)           |

## Danger Zone: Pitfalls & Undefined Behavior

### Formatting Pitfalls

1. **Inconsistent indentation**: Makes code unreadable; use automatic tools
2. **Inconsistent brace placement**: Confuses human readers
3. **Lines exceeding 80-120 chars**: Forces horizontal scrolling
4. **Mixed tabs/spaces**: Different viewers render differently

### Naming Pitfalls

1. **Reserved name collision**: Compiler may reject or silently override
2. **Macro name conflicts**: Preprocessor substitution causes silent bugs
3. **Abbreviated names in headers**: Hard to distinguish (e.g., `p00Orb` vs `p00Urb`)
4. **Changes to struct member names**: Breaks all code using them

### Internationalization Pitfalls

1. **Non-normalized Unicode**: Character decomposition causes identifier mismatches
2. **Script ambiguity**: Greek/Latin similar glyphs cause confusion
3. **Decimal digit variations**: Mathematical bold "0" vs regular 0 indistinguishable
4. **Platform encoding assumptions**: ASCII fallback when Unicode needed

## Comprehensive C23 Working Example

```c
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
```

## Summary of Key TAKEAWAYs

1. **All code must be readable**: Dual purpose (machine + human documentation)
2. **Consistent formatting**: Use automatic tools; don't type manually
3. **Consistent naming**: Follows cultural conventions of project
4. **Reserved names**: Avoid `__*`, leading `_`, unqualified library names
5. **Limit global scope**: Expose only public API
6. **Type names**: Identify concepts
7. **Function names**: Identify actions (often verbs)
8. **Constants**: Identify special artifacts
9. **Variables**: Identify state (rare; generally avoided)
10. **Unicode support**: C23 enables non-English identifiers
