#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <time.h>

/* === 4.1 Basic Thread Creation and Joining === */

int simple_worker(void* arg) {
    int id = *(int*)arg;
    printf("Worker thread %d started\n", id);
    for (int i = 0; i < 3; ++i) {
        printf("  Thread %d: iteration %d\n", id, i);
    }
    printf("Worker thread %d exiting\n", id);
    return id * 100;  // Exit code
}

void demo_basic_threads(void) {
    printf("\n--- Basic Thread Creation ---\n");

    thrd_t threads[2];
    int args[2] = {1, 2};

    for (int i = 0; i < 2; ++i) {
        if (thrd_create(&threads[i], simple_worker, &args[i]) != thrd_success) {
            printf("Failed to create thread %d\n", i);
            return;
        }
    }

    // Join threads
    for (int i = 0; i < 2; ++i) {
        int exit_code;
        if (thrd_join(threads[i], &exit_code) == thrd_success) {
            printf("Thread %d exited with code %d\n", i, exit_code);
        }
    }
}

/* === 4.2 Race Condition Example === */

static int shared_counter = 0;

int increment_unsafe(void* arg) {
    (void)arg;
    for (int i = 0; i < 100000; ++i) {
        shared_counter++;  // Race condition!
    }
    return 0;
}

void demo_race_condition(void) {
    printf("\n--- Race Condition (Unsafe) ---\n");

    shared_counter = 0;
    thrd_t threads[4];

    for (int i = 0; i < 4; ++i) {
        thrd_create(&threads[i], increment_unsafe, NULL);
    }

    for (int i = 0; i < 4; ++i) {
        thrd_join(threads[i], NULL);
    }

    printf("Final counter (unsafe): %d (expected 400000)\n", shared_counter);
}

/* === 4.3 Atomic Operations (Safe) === */

static _Atomic(int) atomic_counter = 0;

int increment_atomic(void* arg) {
    (void)arg;
    for (int i = 0; i < 100000; ++i) {
        atomic_fetch_add(&atomic_counter, 1);
    }
    return 0;
}

void demo_atomic_operations(void) {
    printf("\n--- Atomic Operations (Safe) ---\n");

    atomic_store(&atomic_counter, 0);
    thrd_t threads[4];

    for (int i = 0; i < 4; ++i) {
        thrd_create(&threads[i], increment_atomic, NULL);
    }

    for (int i = 0; i < 4; ++i) {
        thrd_join(threads[i], NULL);
    }

    printf("Final counter (atomic): %d (expected 400000)\n",
           atomic_load(&atomic_counter));
}

/* === 4.4 Mutex for Critical Section Protection === */

static mtx_t data_lock;
static int protected_data = 0;

int mutex_worker(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; ++i) {
        mtx_lock(&data_lock);

        printf("Thread %d: reading protected_data = %d\n", id, protected_data);
        int old = protected_data;
        protected_data = old + 1;
        printf("Thread %d: incremented to %d\n", id, protected_data);

        mtx_unlock(&data_lock);
    }
    free(arg);
    return 0;
}

void demo_mutex(void) {
    printf("\n--- Mutex for Mutual Exclusion ---\n");

    mtx_init(&data_lock, mtx_plain);
    protected_data = 0;

    thrd_t threads[3];
    for (int i = 0; i < 3; ++i) {
        int* id = malloc(sizeof *id);
        *id = i + 1;
        thrd_create(&threads[i], mutex_worker, id);
    }

    for (int i = 0; i < 3; ++i) {
        thrd_join(threads[i], NULL);
    }

    printf("Final protected_data: %d (expected 15)\n", protected_data);

    mtx_destroy(&data_lock);
}

/* === 4.5 Condition Variables === */

static mtx_t cv_lock;
static cnd_t cv;
static _Atomic(int) ready = 0;
static int shared_value = 0;

int producer(void* arg) {
    (void)arg;
    printf("Producer: computing value...\n");
    shared_value = 42;

    mtx_lock(&cv_lock);
    atomic_store(&ready, 1);
    cnd_broadcast(&cv);  // Wake all waiters
    mtx_unlock(&cv_lock);

    printf("Producer: signaled consumers\n");
    return 0;
}

int consumer(void* arg) {
    int id = *(int*)arg;

    mtx_lock(&cv_lock);
    printf("Consumer %d: waiting for value...\n", id);

    while (!atomic_load(&ready)) {
        cnd_wait(&cv, &cv_lock);  // Release lock, sleep, reacquire
    }

    printf("Consumer %d: received value %d\n", id, shared_value);
    mtx_unlock(&cv_lock);

    free(arg);
    return 0;
}

void demo_condition_variable(void) {
    printf("\n--- Condition Variables (Producer-Consumer) ---\n");

    mtx_init(&cv_lock, mtx_plain);
    cnd_init(&cv);
    atomic_store(&ready, 0);

    thrd_t prod, cons[2];

    // Spawn consumers first
    for (int i = 0; i < 2; ++i) {
        int* id = malloc(sizeof *id);
        *id = i + 1;
        thrd_create(&cons[i], consumer, id);
    }

    // Brief sleep to ensure consumers waiting first
    struct timespec delay = {0, 100000000};  // 100ms
    thrd_sleep(&delay, NULL);

    // Spawn producer
    thrd_create(&prod, producer, NULL);

    thrd_join(prod, NULL);
    for (int i = 0; i < 2; ++i) {
        thrd_join(cons[i], NULL);
    }

    mtx_destroy(&cv_lock);
    cnd_destroy(&cv);
}

/* === 4.6 Thread-Local Storage === */

_Thread_local int thread_local_value = 0;

int tlsworker(void* arg) {
    int id = *(int*)arg;
    thread_local_value = id * 100;
    printf("Thread %d: thread_local_value = %d\n", id, thread_local_value);
    free(arg);
    return 0;
}

void demo_thread_local(void) {
    printf("\n--- Thread-Local Storage ---\n");

    thrd_t threads[3];
    for (int i = 0; i < 3; ++i) {
        int* id = malloc(sizeof *id);
        *id = i + 1;
        thrd_create(&threads[i], tlsworker, id);
    }

    for (int i = 0; i < 3; ++i) {
        thrd_join(threads[i], NULL);
    }
}

/* === 4.7 Memory Ordering Example === */

static _Atomic(int) order_a = 0;
static _Atomic(int) order_b = 0;

int writer_thread(void* arg) {
    (void)arg;
    atomic_store_explicit(&order_a, 1, memory_order_release);
    return 0;
}

int reader_thread(void* arg) {
    (void)arg;
    while (atomic_load_explicit(&order_a, memory_order_acquire) == 0) {
        // Spin (busy wait)
    }
    printf("Reader: order_a == 1 (acquired)\n");
    return 0;
}

void demo_memory_ordering(void) {
    printf("\n--- Memory Ordering (Release/Acquire) ---\n");

    atomic_store(&order_a, 0);
    thrd_t w, r;

    thrd_create(&r, reader_thread, NULL);
    thrd_create(&w, writer_thread, NULL);

    thrd_join(w, NULL);
    thrd_join(r, NULL);
}

int main(void) {
    printf("=== Chapter 20: Threads ===\n");

    demo_basic_threads();
    demo_race_condition();
    demo_atomic_operations();
    demo_mutex();
    demo_condition_variable();
    demo_thread_local();
    demo_memory_ordering();

    printf("\n=== End Chapter 20 ===\n");
    return 0;
}
