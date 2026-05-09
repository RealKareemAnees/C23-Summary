# Chapter 21: Atomic Access and Memory Consistency

Source: 21-atomicaccess.md

## 1. Exhaustive Concept Breakdown

### 1.1 Happened-Before Relation

TAKEAWAY 21.1: The happened-before relation constitutes the basic order that we need to understand to reason about concurrent code.

**Happened-before**: Formal relation describing when one operation's effects become visible to another.

**Program order**: Within a single thread, operations happen in source code order (except compiler reordering for performance).

**Synchronization points**: Atomic operations with specific memory ordering enforce happened-before edges between threads.

### 1.2 Synchronization and Consistency

TAKEAWAY 21.2: A synchronization happens between threads; consistency is about all threads seeing a consistent view of data.

**Release-acquire pair**:

- Thread A releases a value (atomic_store with memory_order_release).
- Thread B acquires it (atomic_load with memory_order_acquire).
- All writes by A before the release are visible to B after the acquire.

**Sequentially consistent atomics** (memory_order_seq_cst):

- Total order across all seq_cst operations.
- Eliminates subtle reordering issues.
- Higher performance cost.

### 1.3 Memory Ordering Categories

**Memory ordering levels** (weakest → strongest guarantee):

1. **memory_order_relaxed**: No synchronization. Only ensures atomicity of the operation itself.

   ```c
   atomic_store_explicit(&x, 5, memory_order_relaxed);  // Store atomically; no ordering guarantees
   ```

2. **memory_order_release** (store) / **memory_order_acquire** (load): One-way barrier.

   ```c
   // Writer releases
   atomic_store_explicit(&flag, 1, memory_order_release);

   // Reader acquires
   if (atomic_load_explicit(&flag, memory_order_acquire)) { ... }
   ```

3. **memory_order_acq_rel** (read-modify-write): Acquire and release combined.

   ```c
   atomic_fetch_add_explicit(&counter, 1, memory_order_acq_rel);
   ```

4. **memory_order_seq_cst** (default): Sequential consistency.
   ```c
   atomic_store(&flag, 1);  // Implies memory_order_seq_cst
   ```

### 1.4 The "Happens-Before" Model Formally

For atomics specified with ordering:

- **Sequenced-before**: Instructions in same thread; compiler respects source order.
- **Synchronized-with**: Release-acquire pair between threads.
- **Happens-before**: Transitive closure of sequenced-before and synchronized-with.

TAKEAWAY 21.3: A write to an atomic object synchronizes with a read from that object if the read acquires the result of the write or a later modification.

**Example**:

```c
// Thread 1               Thread 2
x = 10;
atomic_store_explicit(
    &flag, 1,
    memory_order_release);
                          if (atomic_load_explicit(&flag, memory_order_acquire)) {
                              printf("%d", x);  // Guaranteed to see x=10
                          }
```

### 1.5 Modification Order

TAKEAWAY 21.4: Each atomic object has a modification order; threads must agree on which effects happened first.

All threads see the same modification order for each atomic variable (though they may have different views of when each modification occurred).

### 1.6 Weak Ordering (ARM, PowerPC, x86 64-bit)

Weak ordering architectures allow instructions to be reordered. Explicit barriers needed.

**Strong ordering** (x86): Most instructions act as barriers; memory_order_relaxed often fast.

### 1.7 Lock-Free Programming

TAKEAWAY 21.5: Lock-free programming is about preventing situations in which one thread can block waiting for another thread that is blocked.

**Compare-and-swap (CAS)** enables retry-based synchronization without locks:

```c
_Atomic(int) value = 0;
int expected = 0;
if (atomic_compare_exchange_strong(&value, &expected, 1)) {
    // Success: value was 0, now 1
} else {
    // Failure: value was not 0; expected now contains actual value
}
```

## 2. Syntax, Types, and Functions Dictionary

### Memory Ordering Enumeration

| Ordering               | Level         | Use Case                             |
| ---------------------- | ------------- | ------------------------------------ |
| `memory_order_relaxed` | 0 (weakest)   | Counters without ordering guarantees |
| `memory_order_acquire` | 1             | Load acquiring value                 |
| `memory_order_release` | 2             | Store releasing value                |
| `memory_order_acq_rel` | 3             | Read-modify-write with full barrier  |
| `memory_order_seq_cst` | 4 (strongest) | Default for all atomics              |

### Atomic Operations with Explicit Ordering

| Function                                                                  | Purpose                               |
| ------------------------------------------------------------------------- | ------------------------------------- |
| `atomic_store_explicit(obj, val, order)`                                  | Store with specified ordering         |
| `atomic_load_explicit(obj, order)`                                        | Load with specified ordering          |
| `atomic_fetch_add_explicit(obj, delta, order)`                            | Add with ordering                     |
| `atomic_compare_exchange_strong_explicit(obj, expected, new, succ, fail)` | CAS with ordering for success/failure |
| `atomic_exchange_explicit(obj, new, order)`                               | Atomic swap with ordering             |

### Synchronization Primitives (Higher-Level)

| Type                    | Purpose                                 |
| ----------------------- | --------------------------------------- |
| `mtx_t`                 | Mutual exclusion lock                   |
| `cnd_t`                 | Condition variable                      |
| `tss_t`                 | Thread-specific storage key             |
| `once_flag`             | One-time initialization                 |
| `call_once(flag, func)` | Call function exactly once, thread-safe |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Relaxed Atomics Without Synchronization

**Pitfall**: Using memory_order_relaxed expecting cross-thread visibility.

```c
_Atomic(int) data = 0;
_Atomic(int) flag = 0;

// Thread 1
data = 10;  // Unordered with respect to flag
atomic_store_explicit(&flag, 1, memory_order_relaxed);

// Thread 2
if (atomic_load_explicit(&flag, memory_order_relaxed)) {
    printf("%d\n", data);  // NOT guaranteed to see 10!
}
```

**Fix**: Use memory_order_release/acquire if ordering matters.

### 3.2 Mixing Orderings Across Same Variable

**Pitfall**: Some accesses seq_cst, others relaxed.

```c
// Thread 1: atomic_store_explicit(&x, 1, memory_order_seq_cst);
// Thread 2: atomic_store_explicit(&x, 2, memory_order_relaxed);
// Mixing orderings for same variable affects guarantees
```

**Fix**: Use consistent ordering strategy per variable.

### 3.3 Incorrect Memory Ordering for Synchronization

**Pitfall**: Using acquire where release needed (or vice versa).

```c
// Thread 1 (Producer)
data = 42;
atomic_store_explicit(&ready, 1, memory_order_acquire);  // WRONG! Acquire is for loads

// Thread 2 (Consumer)
if (atomic_load_explicit(&ready, memory_order_acquire)) {
    printf("%d\n", data);  // May see uninitialized data
}
```

**Fix**: Use memory_order_release for stores (releases values); memory_order_acquire for loads (acquires values).

### 3.4 Double-Checked Locking (Notorious Pattern)

**Pitfall**: Trying to optimize with unsynchronized load.

```c
if (!initialized) {  // Unsynchronized load – may see stale value
    mtx_lock(&init_lock);
    if (!initialized) {
        // init...
        initialized = 1;
    }
    mtx_unlock(&init_lock);
}
```

**Fix**: Use `call_once()` for one-time initialization; or synchronize even the initial check.

### 3.5 ABA Problem in Lock-Free Code

**Pitfall**: Using CAS on pointer; object freed and reallocated at same address.

```c
if (atomic_compare_exchange_strong(&head, &expected, new)) {
    // Success; but expected might have been freed and reallocated!
}
```

**Fix**: Use generation counters or tagged pointers; manage memory carefully.

## 4. Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <time.h>

/* === 4.1 Memory Ordering Demonstration === */

static _Atomic(int) a = 0;
static _Atomic(int) b = 0;
static int order_test_result = 0;

int thread_a_writes(void* arg) {
    (void)arg;
    atomic_store_explicit(&a, 1, memory_order_release);
    return 0;
}

int thread_b_reads(void* arg) {
    (void)arg;
    // Spin until a is set
    while (atomic_load_explicit(&a, memory_order_acquire) == 0) {
        // Busy wait
    }
    order_test_result = 1;  // A happened-before this line
    return 0;
}

void demo_memory_ordering_formal(void) {
    printf("\n--- Memory Ordering: Release/Acquire Synchronization ---\n");

    atomic_store(&a, 0);
    order_test_result = 0;

    thrd_t ta, tb;
    thrd_create(&tb, thread_b_reads, NULL);
    thrd_create(&ta, thread_a_writes, NULL);

    thrd_join(ta, NULL);
    thrd_join(tb, NULL);

    printf("Result: %d (guaranteed 1 due to release-acquire)\n", order_test_result);
}

/* === 4.2 Sequentially Consistent Atomics === */

static _Atomic(int) x = 0;
static _Atomic(int) y = 0;
static int result1 = 0, result2 = 0;

int seq_writer(void* arg) {
    int id = *(int*)arg;
    if (id == 1) {
        atomic_store(&x, 1);  // memory_order_seq_cst (default)
    } else {
        atomic_store(&y, 1);
    }
    free(arg);
    return 0;
}

int seq_reader(void* arg) {
    int id = *(int*)arg;
    if (id == 1) {
        result1 = atomic_load(&y);  // See if y=1
    } else {
        result2 = atomic_load(&x);
    }
    free(arg);
    return 0;
}

void demo_sequential_consistency(void) {
    printf("\n--- Sequential Consistency (Total Order) ---\n");

    atomic_store(&x, 0);
    atomic_store(&y, 0);
    result1 = 0;
    result2 = 0;

    thrd_t threads[4];

    // Create threads
    for (int i = 0; i < 2; ++i) {
        int* id = malloc(sizeof *id);
        *id = i + 1;
        thrd_create(&threads[i], seq_writer, id);
    }
    for (int i = 0; i < 2; ++i) {
        int* id = malloc(sizeof *id);
        *id = i + 1;
        thrd_create(&threads[2 + i], seq_reader, id);
    }

    for (int i = 0; i < 4; ++i) {
        thrd_join(threads[i], NULL);
    }

    printf("x=%d, y=%d, result1=%d, result2=%d\n",
           atomic_load(&x), atomic_load(&y), result1, result2);
}

/* === 4.3 Compare-and-Swap (CAS) for Lock-Free Programming === */

static _Atomic(int*) lock_free_stack = NULL;

typedef struct Node {
    int value;
    struct Node* next;
} Node;

void push_lock_free(int value) {
    Node* new_node = malloc(sizeof *new_node);
    new_node->value = value;

    Node* head;
    do {
        head = atomic_load(&lock_free_stack);
        new_node->next = head;
    } while (!atomic_compare_exchange_weak(&lock_free_stack, &head, new_node));

    printf("Pushed %d\n", value);
}

int pop_lock_free(void) {
    Node* head;
    do {
        head = atomic_load(&lock_free_stack);
        if (!head) return -1;  // Empty
    } while (!atomic_compare_exchange_weak(&lock_free_stack, &head, head->next));

    int value = head->value;
    free(head);
    return value;
}

void demo_cas_lock_free(void) {
    printf("\n--- Compare-and-Swap Lock-Free Stack ---\n");

    atomic_store(&lock_free_stack, NULL);

    push_lock_free(10);
    push_lock_free(20);
    push_lock_free(30);

    printf("Popped %d\n", pop_lock_free());
    printf("Popped %d\n", pop_lock_free());
    printf("Popped %d\n", pop_lock_free());
}

/* === 4.4 Modification Order Consistency === */

static _Atomic(int) modification_order = 0;

int mod_incrementer(void* arg) {
    (void)arg;
    for (int i = 0; i < 1000; ++i) {
        atomic_fetch_add(&modification_order, 1);
    }
    return 0;
}

void demo_modification_order(void) {
    printf("\n--- Modification Order Consistency ---\n");

    atomic_store(&modification_order, 0);
    thrd_t threads[4];

    for (int i = 0; i < 4; ++i) {
        thrd_create(&threads[i], mod_incrementer, NULL);
    }

    for (int i = 0; i < 4; ++i) {
        thrd_join(threads[i], NULL);
    }

    printf("Final value: %d (all threads agreed on modification order)\n",
           atomic_load(&modification_order));
}

/* === 4.5 Acquire/Release Synchronization === */

static _Atomic(int) producer_done = 0;
static int protected_buffer[10];
static _Atomic(int) buffer_index = 0;

int producer_thread(void* arg) {
    (void)arg;
    for (int i = 0; i < 10; ++i) {
        protected_buffer[i] = (i + 1) * 100;
    }
    atomic_store_explicit(&buffer_index, 10, memory_order_release);
    printf("Producer: released buffer with 10 elements\n");
    return 0;
}

int consumer_thread(void* arg) {
    (void)arg;
    int count = atomic_load_explicit(&buffer_index, memory_order_acquire);
    printf("Consumer: acquired buffer with %d elements\n", count);
    for (int i = 0; i < count; ++i) {
        printf("  [%d] = %d\n", i, protected_buffer[i]);
    }
    return 0;
}

void demo_acquire_release(void) {
    printf("\n--- Acquire/Release Synchronization ---\n");

    atomic_store(&buffer_index, 0);

    thrd_t prod, cons;
    thrd_create(&prod, producer_thread, NULL);
    thrd_create(&cons, consumer_thread, NULL);

    thrd_join(prod, NULL);
    thrd_join(cons, NULL);
}

/* === 4.6 Relaxed Atomics (Counter Example) === */

static _Atomic(int) relaxed_counter = 0;

int relaxed_incrementer(void* arg) {
    (void)arg;
    for (int i = 0; i < 250000; ++i) {
        atomic_fetch_add_explicit(&relaxed_counter, 1, memory_order_relaxed);
    }
    return 0;
}

void demo_relaxed_atomics(void) {
    printf("\n--- Relaxed Atomics (No Ordering) ---\n");

    atomic_store(&relaxed_counter, 0);
    thrd_t threads[4];

    for (int i = 0; i < 4; ++i) {
        thrd_create(&threads[i], relaxed_incrementer, NULL);
    }

    for (int i = 0; i < 4; ++i) {
        thrd_join(threads[i], NULL);
    }

    printf("Relaxed counter: %d (expected 1000000, no ordering guarantees)\n",
           atomic_load_explicit(&relaxed_counter, memory_order_relaxed));
}

/* === 4.7 Happens-Before Transitive Property === */

static _Atomic(int) hb_a = 0;
static _Atomic(int) hb_b = 0;
static _Atomic(int) hb_c = 0;
static int hb_result = -1;

int hb_thread1(void* arg) {
    (void)arg;
    atomic_store_explicit(&hb_a, 1, memory_order_release);
    return 0;
}

int hb_thread2(void* arg) {
    (void)arg;
    while (atomic_load_explicit(&hb_a, memory_order_acquire) == 0) ;
    atomic_store_explicit(&hb_b, 1, memory_order_release);
    return 0;
}

int hb_thread3(void* arg) {
    (void)arg;
    while (atomic_load_explicit(&hb_b, memory_order_acquire) == 0) ;
    hb_result = 1;  // Transitively happened-after T1
    return 0;
}

void demo_happens_before_transitive(void) {
    printf("\n--- Happens-Before Transitivity ---\n");

    atomic_store(&hb_a, 0);
    atomic_store(&hb_b, 0);
    hb_result = -1;

    thrd_t t1, t2, t3;
    thrd_create(&t1, hb_thread1, NULL);
    thrd_create(&t2, hb_thread2, NULL);
    thrd_create(&t3, hb_thread3, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
    thrd_join(t3, NULL);

    printf("Result: %d (guaranteed 1 due to transitive happens-before)\n", hb_result);
}

int main(void) {
    printf("=== Chapter 21: Atomic Access and Memory Consistency ===\n");

    demo_memory_ordering_formal();
    demo_sequential_consistency();
    demo_cas_lock_free();
    demo_modification_order();
    demo_acquire_release();
    demo_relaxed_atomics();
    demo_happens_before_transitive();

    printf("\n=== End Chapter 21 ===\n");
    return 0;
}
```

**Key Takeaways**:

- Happened-before relation formalizes when effects are visible across threads.
- Memory ordering controls visibility; must match synchronization intent (release for producer, acquire for consumer).
- Sequentially consistent atomics simplify reasoning but cost performance.
- Weak ordering (ARM/PowerPC) requires explicit barriers; strong ordering (x86) mostly hides complexity.
- Lock-free programming uses CAS; complex but necessary for certain performance-critical code.
