# Chapter 2: The Principal Structure of a Program

Source: 02-principalstructure.md

> Program structure balances syntactical form (grammar, punctuation, keywords) with semantic meaning (declarations, definitions, statements). Every C program is assembled from lexical elements organized by grammatical rules, with declarations introducing identifiers, definitions allocating objects, and statements specifying execution logic. Understanding these three layers—syntax, declarations, and control flow—is essential for writing correct, portable C code.

## 1. Exhaustive Concept Breakdown

### 1.1 Lexical Structure and Syntactical Elements

C programs are built from distinct lexical (textual) elements that must follow precise grammatical rules. The compiler parses these elements in sequence to construct a coherent program representation.

**Keywords (Special Words)**: Reserved identifiers that have fixed meaning defined by the C language specification. They cannot be redefined or repurposed by programmers. Examples include `int`, `double`, `char`, `void`, `for`, `return`, `#include`, and `[[maybe_unused]]`. Keywords are printed in bold in formal documentation to distinguish them from user-defined identifiers.

**Literals**: Fixed values embedded directly in source code, representing concrete data:

- Numeric literals: `0`, `1`, `5`, `9.0`, `2.9`, `3.E+25`, `.00007`
- String literals: sequences of characters enclosed in double quotes, e.g., `"element %zu is %g, \tits square is %g"`
- Literals are immutable at compile-time and become part of the executable

**Identifiers**: Names given by programmers (or predefined by the standard) to refer to program entities. Identifiers must start with a letter or underscore and contain only alphanumeric characters and underscores. Examples: `A`, `i`, `main`, `printf`, `size_t`, `EXIT_SUCCESS`. Identifiers serve multiple roles:

- Data objects (variables): `A`, `i`
- Type aliases: `size_t` (conventionally suffixed with `_t` to indicate a type)
- Functions: `main`, `printf`
- Constants: `EXIT_SUCCESS`

### 1.2 Punctuation and Bracket Pairs

C uses six bracket types, each serving specific grammatical purposes:

- `{...}`: Encloses compound statements (blocks); contains multiple statements grouped together
- `(...)`: Encloses parameter lists, expressions, and controls statement grouping
- `[...]`: Array declarators and array subscripts (indexing)
- `[[...]]`: Modern C (C23) attributes providing metadata to the compiler (e.g., `[[maybe_unused]]`)
- `/*...*/`: Block comments spanning multiple lines
- `<...>`: Include directive angle brackets (for system/standard headers)

**Separators and Terminators**:

- **Comma (`,`)**: Separates function arguments, array elements in initializer lists, and multiple declarations
- **Semicolon (`;`)**: Terminates statements, marking the logical end of an instruction

**Critical principle**: The same punctuation characters serve multiple grammatical purposes. For example, `{}` encloses both compound statements (control flow blocks) and aggregate (structured) type definitions. Similarly, `[]` is used for array declarations, array indexing, and designated initializers. Context determines meaning.

### 1.3 Comments: Program Documentation

Comments are programmer annotations ignored by the compiler; they serve purely as human-readable documentation.

**Block comments** (`/* ... */`): Span multiple lines; used for detailed explanations, copyright notices, and detailed documentation. Can be nested in some modern compilers (though standard C doesn't guarantee nesting support).

**Line comments** (`// ...`): Extend from `//` to the end of the line; original to C++ but now part of C99 and later standards. Ideal for short inline explanations.

Best practice: Use comments generously to explain _why_ the code does something, not merely _what_ it does. The _what_ should be clear from reading the code; the _why_ often requires insight.

### 1.4 Operators: Fundamental Computations

Operators perform computations on operands. The chapter introduces several key operators:

**Assignment operator (`=`)**: Initializes or assigns a value to a variable. In initialization, it appears in declarations; in assignment statements, it modifies an existing variable's value.

**Comparison operator (`<`)**: Produces a boolean result (true/false); used in conditions to test relationships between values.

**Increment operator (`++`)**: Increases a variable's value by 1. The prefix form (`++i`) increments before the expression is evaluated; the postfix form (`i++`) increments after evaluation. For simple loops, both produce identical results, but prefix is preferred as it's marginally more efficient (no temporary value needed).

**Multiplication operator (`*`)**: Performs numeric multiplication of two operands. In type declarations, `*` denotes pointer types (to be explored later).

### 1.5 Attributes (Modern C23 Feature)

Attributes, enclosed in double square brackets `[[...]]`, provide supplemental information or hints to the compiler. The `[[maybe_unused]]` attribute signals that a variable (typically a function parameter) may legitimately remain unused in a particular code path. This suppresses compiler warnings about unused parameters, improving build cleanliness when certain parameters are conditionally used or serve as part of a standardized interface.

### 1.6 Grammar and Parsing

C's grammar defines how lexical elements (keywords, identifiers, literals, punctuation) combine to form valid constructs (declarations, statements, function definitions). The grammar distinguishes between:

- **Syntax** (grammatical form): "Is this text structured correctly according to language rules?"
- **Semantics** (meaning): "Does this text express the intended computation logically?"

The compiler uses the grammar to parse source text, building an internal representation (abstract syntax tree) from which it derives executable code.

### 1.7 Declarations: Introducing Identifiers

A **declaration** specifies what an identifier represents—its type, storage class, and other properties—without necessarily allocating storage or defining initial values.

**Core principle**: All identifiers used in a C program must be declared before use. Declarations tell the compiler the identifier's type and role.

**Characteristics of declarations**:

- Multiple consistent declarations of the same identifier are permitted (helpful when spreading code across multiple files)
- However, inconsistent declarations cause errors; the compiler enforces consistency within a scope
- Declarations establish what properties an identifier has; they don't create the object itself

**Predeclared identifiers**: Some identifiers (e.g., `printf`, `size_t`, `EXIT_SUCCESS`) are not explicitly declared in user code. Instead, they come from standard library headers (included via `#include`). These headers contain the actual declarations, normally in system files on the computer.

### 1.8 Scope: Visibility and Binding

**Scope** defines the region of program text where an identifier is visible to the compiler and accessible to use.

**Block scope**: Identifiers declared within `{...}` or `(...)` are visible from their declaration point to the end of that block.

- **Local block scope**: Variables declared in function bodies (e.g., `A` in `main`)
- **Loop scope**: Loop variables declared in `for` statements (e.g., `i` in the for loop) are scoped to that loop
- **Primary and secondary blocks**: Nested blocks create nested scopes; inner scopes inherit from outer scopes but shadow outer-scope identifiers with the same name

**File scope (global scope)**: Identifiers declared outside any function (e.g., `main` itself) have file scope; they are visible from declaration to end of file (or compilation unit).

**Parameters**: Function parameters (e.g., `argc`, `argv` in `main`) have scope spanning the entire function body.

**Nesting**: Scopes nest like parentheses. The example program has four nested scope levels: file scope (for `main`), function block scope (inside `main`), primary for-loop block scope, and secondary for-loop body scope.

### 1.9 Declarations vs. Definitions

**Declaration**: Specifies the type and properties of an identifier, but doesn't necessarily allocate storage (for objects) or provide implementation (for functions).

**Definition**: Allocates storage for objects and provides implementation for functions. For objects, a definition typically combines a declaration with an initializer. For functions, a definition is a declaration followed by a function body (`{...}`).

**Core rule**: Every object and function must have exactly one definition. Multiple definitions cause linker errors. However, multiple declarations of the same identifier (consistent with each other) are permitted.

### 1.10 Initialization and Designated Initializers

**Initialization** augments a declaration with an initial value, instructing the compiler to allocate storage and set that storage to the provided value. When a declaration includes an initializer, it simultaneously defines the object.

**Simple initialization**: `size_t i = 0;` declares `i` as type `size_t` and initializes it to `0`.

**Designated initializers** (array/struct initialization): Explicitly specify which elements receive which values:

```
double A[5] = {
    [0] = 9.0,
    [1] = 2.9,
    [4] = 3.E+25,
    [3] = .00007,
};
```

- Index `[n]` designates the element at position `n`
- Unspecified positions default to zero (0.0 for floating-point types)
- In this example, index `[2]` is not listed, so it defaults to `0.0`

**Zero-initialization rule**: Any position in an initializer list that is not explicitly assigned defaults to zero of the appropriate type. This is safe and predictable in Modern C.

### 1.11 Arrays

**Array declaration**: `type identifier[size]` allocates a contiguous block of memory for `size` elements, each of type `type`.

**Properties**:

- **Size is fixed**: Array size is determined at compile-time and cannot change at runtime
- **Zero-indexed**: First element is at index `0`, last at index `size - 1`
- **Contiguous storage**: All elements occupy adjacent memory locations
- **Same element type**: All array elements share the same type

**Array access**: `A[i]` refers to the element at index `i`. Valid indices range from `0` to `size - 1`. Accessing outside this range is undefined behavior; no runtime bounds checking is performed.

### 1.12 Type System and Type Safety

Every identifier has a **type** specifying what kind of value it can hold and what operations are valid on it.

**Type examples**:

- `int`: Integer values (typically 32-bit signed)
- `double`: Floating-point numbers with fractional precision (typically 64-bit IEEE 754)
- `size_t`: Unsigned integer for sizes and indices (platform-dependent; typically 64-bit on modern systems, 32-bit on legacy systems)
- `char*`: Pointer to character (address of a character or string)

**Type declaration**: Declarations begin with a type specifier, establishing the type properties of all identifiers declared in that declaration.

### 1.13 Function Declarations and Definitions

A **function declaration** specifies:

- Return type (e.g., `int` for `main`)
- Function name (identifier)
- Parameter types and names (in parentheses)

**Function definition**: Declaration + function body (compound statement in `{...}`).

**Special role of `main`**: Every C program must have exactly one `main` function, which serves as the program entry point. The system calls `main` to begin execution.

**Parameter semantics**: In C, all function parameters use **call by value** semantics—the values of arguments are copied to parameter variables. The called function receives copies and cannot modify the original variables in the caller's scope. (Pointers provide an indirect mechanism to modify caller's data, but that's still call-by-value of a pointer value.)

### 1.14 Function Calls and Control Flow

A **function call** suspends execution of the current function, transfers control to the named function, and resumes at the call site when the called function returns.

**Execution flow**:

1. Program starts with system startup code calling `main`
2. `main` calls functions (e.g., `printf`)
3. Called function executes until `return` statement
4. Control returns to call site; caller resumes

**Return statement**: Terminates function execution and optionally provides a return value. For functions with non-void return types, a value must be returned matching the declared return type.

**Return type matching**: If a function is declared to return `int`, the return statement must provide an `int` value (or a value implicitly convertible to `int`). Type mismatches are errors.

### 1.15 Distinct Roles of Identifiers

The same syntactic element (identifier) plays multiple distinct semantic roles:

- **Data objects**: Named storage locations holding values
- **Functions**: Named code blocks that can be called
- **Type aliases**: Names for types (e.g., `size_t`)
- **Constants**: Names for fixed values (e.g., `EXIT_SUCCESS`)

Context determines which role an identifier plays in any given occurrence.

### 1.16 Header Files and Includes

**Header files** (`.h` extension) contain declarations and macro definitions that describe identifiers provided by libraries or the standard library. Including a header makes those declarations available to the compiler in the current source file.

**Mechanism**: `#include <stdio.h>` tells the preprocessor to locate and insert the contents of the `stdio.h` header file into the current source file at that point. The angle brackets indicate a system/standard header located in the compiler's standard search paths.

## 2. Syntax, Types & Functions Dictionary

| Entity                                   | Type/Category              | Size/Signature                             | Parameters                                    | Return / Behavior                                                       |
| ---------------------------------------- | -------------------------- | ------------------------------------------ | --------------------------------------------- | ----------------------------------------------------------------------- |
| `#include <header>`                      | Preprocessor directive     | —                                          | Header filename                               | Inserts header contents into source file                                |
| `int`, `double`, `char`, `size_t`        | Type specifiers            | Various                                    | —                                             | Declare variable/parameter type                                         |
| `type identifier[size]`                  | Array declaration          | `size * sizeof(type)` bytes                | —                                             | Allocates fixed-size contiguous array                                   |
| `[index] = value`                        | Designated initializer     | —                                          | Array index, initialization value             | Explicitly assigns array element                                        |
| `type function(type param1, ...)`        | Function declaration       | —                                          | Parameters (types and names)                  | Declares function signature                                             |
| `type function(...) { statements }`      | Function definition        | —                                          | Parameters                                    | Declares function + implements body                                     |
| `int main(int argc, char* argv[argc+1])` | Main function declaration  | —                                          | Command-line argument count and values        | Program entry point; returns status code                                |
| `/* comment */`                          | Block comment              | —                                          | —                                             | Multi-line documentation (ignored by compiler)                          |
| `// comment`                             | Line comment               | —                                          | —                                             | Single-line documentation to end of line                                |
| `{ statements }`                         | Compound statement (block) | —                                          | Statements                                    | Groups multiple statements into one block; defines scope                |
| `for (init; cond; incr) statement`       | For loop                   | —                                          | Initialization, condition, increment          | Domain iteration; repeats statement while condition is true             |
| `return value;`                          | Return statement           | —                                          | Value (type matching function return type)    | Exits function, passing value to caller                                 |
| `identifier = value`                     | Assignment                 | —                                          | Left operand (lvalue), right operand (rvalue) | Assigns value to variable                                               |
| `function(arg1, arg2, ...)`              | Function call              | Depends on function                        | Arguments (values passed to function)         | Calls function with provided arguments; returns function's return value |
| `identifier++`                           | Post-increment             | —                                          | Variable                                      | Increments variable by 1; expression evaluates to old value             |
| `++identifier`                           | Pre-increment              | —                                          | Variable                                      | Increments variable by 1; expression evaluates to new value             |
| `value1 < value2`                        | Less-than comparison       | `int` (boolean: 0 or 1)                    | Left operand, right operand                   | Returns 1 if left < right, 0 otherwise                                  |
| `value1 * value2`                        | Multiplication             | Result type (usually arithmetic promotion) | Two numeric operands                          | Product of operands                                                     |
| `[...]`                                  | Array subscript            | Element type                               | Array identifier, index                       | Accesses element at index                                               |
| `[[attribute]]`                          | Attribute (C23)            | —                                          | Attribute name and arguments                  | Provides metadata to compiler (e.g., `[[maybe_unused]]`)                |
| `scope`                                  | Program region             | —                                          | —                                             | Region where identifier is visible and accessible                       |
| `block scope`                            | Scope type                 | —                                          | —                                             | Visibility from declaration to end of enclosing block                   |
| `file scope` (global)                    | Scope type                 | —                                          | —                                             | Visibility from declaration to end of file/compilation unit             |

## 3. The "Danger Zone": UB, Pitfalls, and Warnings

### 3.1 Array Index Out of Bounds

**Problem**: Accessing array elements with indices outside the valid range `[0, size-1]`.

**Why it's wrong**: Arrays have fixed size; indices beyond the array's boundaries refer to memory not allocated to the array.

**Compiler behavior**: No compile-time error or warning; C does not perform runtime bounds checking.

**Consequence**: **Undefined Behavior**. Reading garbage data from adjacent memory, corrupting adjacent memory on write, or causing a segmentation fault. The program's behavior is unpredictable and platform-dependent.

**Mitigation**: Use loop conditions like `i < array_size` to ensure indices remain valid. Carefully validate index values before array access.

### 3.2 Declaration-Definition Confusion

**Problem**: Confusing the purpose of declarations vs. definitions. Programmers sometimes attempt to "declare and initialize" a variable, not realizing that initialization simultaneously defines it (allocates storage).

**Why it matters**: Multiple declarations are legal; multiple definitions are not. A common error is declaring a variable in a header file without `extern`, causing duplicate definitions when that header is included in multiple source files.

**Consequence**: Linker errors for duplicate symbols, or unexpected runtime behavior if definitions in different files initialize to different values.

**Mitigation**: Understand that declaration + initializer = definition. Use `extern` keyword (advanced topic) to separate declaration from definition when appropriate.

### 3.3 Uninitialized Variables

**Problem**: Using a variable before explicit initialization.

**Compiler behavior**: Modern compilers may warn about uninitialized variables (`-Wuninitialized` flag).

**Consequence**: Reading garbage values from memory; program behavior is unpredictable. The "default zero-initialization" of designated initializers is special; regular variable declarations do not zero-initialize unless explicitly specified.

**Mitigation**: Always initialize variables at declaration. Use initializers even if explicitly setting to zero.

### 3.4 Scope-Shadowing Errors

**Problem**: Declaring an identifier in an inner scope with the same name as an identifier in an outer scope. The inner declaration "shadows" the outer identifier, making it inaccessible within that inner scope.

**Compiler behavior**: Modern compilers can warn about shadowing with the `-Wshadow` flag.

**Example**:

```c
int i = 0;  // File scope
{
    int i = 1;  // Block scope; shadows outer i
    // Inside this block, i refers to the inner variable, not the file-scope i
}
```

**Consequence**: If programmer intended to use the outer `i`, the code will silently use the inner one instead, causing logic errors.

**Mitigation**: Use unique identifier names to avoid shadowing. Enable compiler warnings for shadowing.

### 3.5 Missing Function Declarations

**Problem**: Calling a function without including the header that declares it (or providing a forward declaration).

**Compiler behavior**: Older C allows implicit declarations (assumes `int` return type, even if incorrect). Modern compilers with `-Wall` warn or error.

**Consequence**: Arguments may be misinterpreted if the implicit signature doesn't match the actual function signature. Calling a variadic function (like `printf`) without its declaration causes undefined behavior.

**Mitigation**: Always include appropriate headers for standard library functions. Never rely on implicit declarations.

### 3.6 Parameter Type Mismatch in Function Calls

**Problem**: Passing arguments of types different from the declared parameter types.

**Example**: Passing `double` argument to a function expecting `int` parameter.

**Compiler behavior**: Implicit type conversion may occur, or a warning is issued.

**Consequence**: Unintended type conversions, loss of precision, or incorrect values passed to the function.

**Mitigation**: Ensure argument types match parameter types. Use explicit casts if conversion is intentional.

### 3.7 Designated Initializer Ordering

**Problem**: Assuming that unspecified array elements retain previous values or have predictable non-zero values.

**Compiler behavior**: Modern C zeros unspecified elements; older standards have implementation-defined behavior.

**Example**:

```c
double A[5] = { [1] = 2.9, [3] = .00007 };
// Indices 0, 2, 4 are uninitialized/zero; behavior is implementation-defined in older C
```

**Consequence**: On older compilers or non-conforming implementations, unspecified elements may contain garbage. ModernC (C23) guarantees zero-initialization, but portability to older standards is compromised.

**Mitigation**: Explicitly initialize all elements, or rely on C23 guarantees and document the requirement.

### 3.8 Loop Variable Reuse and Scope

**Problem**: Declaring loop variables outside the `for` statement and reusing them in multiple loops, or declaring them insider but misunderstanding their scope.

**Compiler behavior**: Legal but error-prone.

**Example** (poor practice):

```c
int i;
for (i = 0; i < 5; ++i) { ... }
for (i = 0; i < 10; ++i) { ... }  // Reusing i; scope extends across both loops
```

**Better practice**:

```c
for (size_t i = 0; i < 5; ++i) { ... }
for (size_t i = 0; i < 10; ++i) { ... }  // Each i is scoped to its loop
```

**Consequence**: Confusing code readability; harder to maintain; risk of accidental interference between loops.

**Mitigation**: Declare loop variables in the `for` initialization to scope them to that loop.

### 3.9 Function Return Type Mismatch

**Problem**: Returning a value of type different from the declared return type, or not returning when a value is expected.

**Example**: Returning `double` from a function declared to return `int`.

**Compiler behavior**: Implicit type conversion occurs; modern compilers may warn.

**Consequence**: Unintended conversion (e.g., truncation of fractional part if converting `double` to `int`).

**Mitigation**: Ensure return type matches declaration. Use explicit casts if conversion is intentional.

### 3.10 Multiple Function Definitions

**Problem**: Defining the same function in multiple places (e.g., in multiple source files without using `static` or appropriate header guards).

**Compiler behavior**: Compilation succeeds, but linker fails with "multiple definition" error.

**Consequence**: Program cannot be linked into an executable.

**Mitigation**: Ensure exactly one definition per function. Use header files with declarations; implement function in one source file.

### 3.11 Inconsistent Declarations

**Problem**: Declaring the same function or variable with inconsistent signatures across different scopes or files.

**Example**:

```c
// File1: int foo(int x);
// File2: double foo(double x);  // Inconsistent!
```

**Compiler behavior**: Each file compiles independently; linker may fail or produce runtime errors.

**Consequence**: Wrong function being called, or wrong type conversions applied.

**Mitigation**: Use header files to declare functions once, consistently. Include headers in all files using those functions.

### 3.12 Call-by-Value Misconception

**Problem**: Assuming that modifications to function parameters affect the original variables in the caller's scope.

**Why it's wrong**: C uses call-by-value; parameters are copies. Modifying a parameter doesn't affect the caller's variable.

**Example**:

```c
void increment(int x) { x++; }  // Increments the copy, not the original
```

**Consequence**: Functions cannot modify non-pointer variables in the caller, contrary to programmer expectation.

**Mitigation**: Understand call-by-value semantics. Use pointers (advanced topic) if the function must modify caller's data.

## 4. Comprehensive C23 Working Example

```c
#include <stdlib.h>
#include <stdio.h>

// ============================================================
// Comprehensive Example: Program Structure, Declarations,
// Definitions, and Statements
// ============================================================

// This program demonstrates:
// 1. Proper main() function signature with parameter declaration
// 2. Local variable declarations with initialization (definitions)
// 3. Array declaration with designated initializers
// 4. For loop: iteration with loop variable scoped to the loop
// 5. Function calls (printf) with proper arguments
// 6. Arithmetic operations (multiplication, comparison)
// 7. Scope management (file scope, block scope, loop scope)
// 8. Control flow (function return with status code)
// 9. Proper handling of declared vs. defined entities

// Global constant (file scope)
// Demonstrates: top-level declaration and definition
static const int MAX_ITERATIONS = 5;

// Forward function declaration (optional, for demonstration)
// Demonstrates: declaration without definition
void print_section_header(const char* header);

// ============================================================
// Main Program Entry Point
// ============================================================

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    // ========================================
    // Section 1: Local Variable Declarations with Initialization
    // ========================================

    // Declaration + initializer = definition
    // This declares sensor_data and allocates storage for an array of 5 doubles
    double sensor_data[5] = {
        [0] = 23.5,    // Designated initializer: explicitly set index 0
        [1] = 24.1,    // Designated initializer: explicitly set index 1
        [4] = 22.8,    // Designated initializer: explicitly set index 4
        // Indices [2] and [3] are not explicitly listed
        // In Modern C (C23), they default to 0.0
    };

    print_section_header("Raw Sensor Data");

    // ========================================
    // Section 2: For Loop with Domain Iteration
    // ========================================

    // Loop variable i is declared and scoped to this for loop
    // Demonstrates: proper loop variable declaration in for initialization
    // Demonstrates: proper bounds checking (i < 5 ensures valid array indices)
    for (size_t i = 0; i < 5; ++i) {
        // At each iteration, i takes values 0, 1, 2, 3, 4
        // Loop condition (i < 5) ensures i never reaches 5 (out of bounds)

        // sensor_data[i] accesses array element at index i
        // This is safe because i is in range [0, 4]

        printf("Sensor %zu: %.1f degrees C\n", i, sensor_data[i]);

        // ====================================
        // Edge Case Handling: Scope Awareness
        // ====================================

        // If the loop condition were i < 10 (incorrect), attempting
        // sensor_data[i] at i = 5, 6, 7, 8, 9 would access memory
        // outside the array (undefined behavior).
        // We prevent this with correct bounds i < 5.
    }

    // ========================================
    // Scope End: Loop Variable i is Inaccessible
    // ========================================

    // The identifier 'i' is no longer accessible here.
    // Its scope ended at the closing } of the for loop.
    // This is intentional block scoping.

    print_section_header("Statistical Analysis");

    // ========================================
    // Section 3: Multiple Loop Iterations with Independent Scope
    // ========================================

    // Each loop gets its own scope for the loop variable.
    // This is safe and clear.
    for (size_t iter = 0; iter < 5; ++iter) {
        double value = sensor_data[iter];
        double squared = value * value;

        printf("Reading %zu: %.1f, Square: %.1f\n", iter, value, squared);
    }

    // ========================================
    // Section 4: Demonstrating Declaration vs. Definition
    // ========================================

    // Declaration only (no initializer):
    int result;  // This declares result but does NOT initialize it

    // To avoid undefined behavior, we initialize before use:
    result = 42;  // Assignment after declaration; result is now defined with a value

    printf("\nFinal result: %d\n", result);

    // ========================================
    // Section 5: Function Return and Status Code
    // ========================================

    // main() returns int (not void; that would be incorrect in Modern C)
    // EXIT_SUCCESS signals successful program termination to the OS
    // This satisfies the semantic requirement of the main function declaration
    return EXIT_SUCCESS;
}

// ============================================================
// Function Definition: print_section_header
// ============================================================

// Forward declaration at top (demonstrates multiple declarations allowed)
// Actual definition here (demonstrates declaration + body = definition)

void print_section_header(const char* header) {
    // This function was declared forward above (declaration only)
    // Now we define it (provide the implementation in {...})

    printf("\n===== %s =====\n", header);

    // Function ends with implicit return (void return type requires no value)
}

// ============================================================
// Compilation (all these commands are equivalent for this example):
// ============================================================
// gcc -std=c23 -Wall -o program_structure_example program_structure_example.c
// clang -std=c23 -Wall -o program_structure_example program_structure_example.c
// c23 -Wall -o program_structure_example program_structure_example.c
//
// Execution:
// ./program_structure_example
//
// Expected Output:
// ===== Raw Sensor Data =====
// Sensor 0: 23.5 degrees C
// Sensor 1: 24.1 degrees C
// Sensor 2: 0.0 degrees C
// Sensor 3: 0.0 degrees C
// Sensor 4: 22.8 degrees C
//
// ===== Statistical Analysis =====
// Reading 0: 23.5, Square: 552.2
// Reading 1: 24.1, Square: 580.8
// Reading 2: 0.0, Square: 0.0
// Reading 3: 0.0, Square: 0.0
// Reading 4: 22.8, Square: 519.8
//
// Final result: 42
//
// ============================================================
// Key Concepts Embedded in This Code:
// ============================================================
// 1. Proper main() signature and return type
// 2. Function forward declaration (declaration without definition)
// 3. Array declaration with designated initializers
// 4. Scope management: file scope (MAX_ITERATIONS), block scope (sensor_data),
//    loop scope (i, iter)
// 5. Definition = declaration + initializer/implementation
// 6. Loop variables scoped to their for loops (best practice)
// 7. Safe array indexing with loop bounds ensuring valid indices
// 8. Call-by-value semantics in function parameters
// 9. Function definition providing implementation
// 10. Status code return from main() for OS signaling
```
