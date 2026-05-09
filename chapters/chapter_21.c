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
