# Chapter 20: Threads

Source: 20-threads.md

## 1. Exhaustive Concept Breakdown

### 1.1 threading Library Basics

TAKEAWAY 20.1: Threads are a dangerous weapon that can easily lead to catastrophe.

C11 introduces `<threads.h>` for portable threading (previously pthreads on POSIX).

**Thread lifecycle**:

1. Create thread: `thrd_create(&id, func, arg)`.
2. Execute function concurrently with creating thread.
3. Join (wait for completion): `thrd_join(id, &exitcode)`.

TAKEAWAY 20.2: If two threads access the same object, at least one of them must write to it, we have a data race.

### 1.2 Race Conditions

**Race condition**: Multiple threads access shared data; at least one writes; execution order unpredictable.

**Atomic operations** prevent races by providing indivisible read-modify-write.

```c
_Atomic(int) counter = 0;
// counter++;  // UB: RMW without atomicity
atomic_fetch_add(&counter, 1);  // Safe
```

TAKEAWAY 20.3: We must use atomic objects or synchronization primitives for concurrent access.

### 1.3 Atomic Objects

**`_Atomic(T)` qualifier**: Declares type as atomic.

```c
_Atomic(int) x;           // Atomic int
x = 5;                    // Atomic store
int val = x;              // Atomic load
atomic_fetch_add(&x, 1);  // Atomic increment
```

**Memory ordering** (C11 `<stdatomic.h>`):

- `memory_order_relaxed`: No synchronization
- `memory_order_acquire`/`memory_order_release`: Acquire/release semantics
- `memory_order_acq_rel`: Both acquire and release
- `memory_order_seq_cst`: Sequentially consistent (safest, slowest)

### 1.4 Synchronization Primitives

**Mutex**: Exclusive lock.

```c
mtx_t lock;
mtx_init(&lock, mtx_plain);
mtx_lock(&lock);
// Critical section (only one thread at a time)
mtx_unlock(&lock);
mtx_destroy(&lock);
```

**Condition variable**: Wake sleeping threads when condition met.

```c
cnd_t cv;
cnd_init(&cv);
mtx_lock(&lock);
while (!condition) cnd_wait(&cv, &lock);  // Release lock, sleep, reacquire
mtx_unlock(&lock);
cnd_broadcast(&cv);  // Wake all waiters
cnd_destroy(&cv);
```

### 1.5 Thread-Local Storage

`_Thread_local` (C11):

```c
_Thread_local int thread_id = 0;  // Each thread has own copy
```

### 1.6 Memory Consistency Models

TAKEAWAY 20.4: The different memory models with atomic operations allow the programmer to express the minimal order constraints required to avoid data races.

**Synchronization pairs**:

- Release by thread A + acquire by thread B ensures A's writes visible to B.
- Enables efficient weak atomic operations without full sequential consistency.

## 2. Syntax, Types, and Functions Dictionary

### Threading Functions (`<threads.h>`)

| Function                                                           | Purpose                        |
| ------------------------------------------------------------------ | ------------------------------ |
| `int thrd_create(thrd_t* thr, int (*func)(void*), void* arg)`      | Create thread                  |
| `int thrd_join(thrd_t thr, int* res)`                              | Wait for thread; get exit code |
| `int thrd_detach(thrd_t thr)`                                      | Thread cleanup automatic       |
| `thrd_t thrd_current(void)`                                        | Get current thread ID          |
| `int thrd_sleep(const struct timespec* dur, struct timespec* rem)` | Sleep                          |
| `void thrd_exit(int res)`                                          | Exit thread with code          |

### Mutex Functions (`<threads.h>`)

| Function                             | Purpose                                     |
| ------------------------------------ | ------------------------------------------- |
| `int mtx_init(mtx_t* mtx, int type)` | Initialize mutex (mtx_plain, mtx_recursive) |
| `int mtx_lock(mtx_t* mtx)`           | Lock (block if held)                        |
| `int mtx_trylock(mtx_t* mtx)`        | Lock (non-blocking)                         |
| `int mtx_unlock(mtx_t* mtx)`         | Unlock                                      |
| `int mtx_destroy(mtx_t* mtx)`        | Destroy mutex                               |

### Condition Variable Functions

| Function                                                                | Purpose                         |
| ----------------------------------------------------------------------- | ------------------------------- |
| `int cnd_init(cnd_t* cond)`                                             | Initialize condition variable   |
| `int cnd_wait(cnd_t* cond, mtx_t* mtx)`                                 | Release mutex, sleep, reacquire |
| `int cnd_timedwait(cnd_t* cond, mtx_t* mtx, const struct timespec* ts)` | Wait with timeout               |
| `int cnd_signal(cnd_t* cond)`                                           | Wake one waiter                 |
| `int cnd_broadcast(cnd_t* cond)`                                        | Wake all waiters                |
| `int cnd_destroy(cnd_t* cond)`                                          | Destroy condition variable      |

### Atomic Operations (`<stdatomic.h>`)

| Function                                             | Purpose                     |
| ---------------------------------------------------- | --------------------------- |
| `atomic_store(obj, value)`                           | Atomic write                |
| `atomic_load(obj)`                                   | Atomic read                 |
| `atomic_fetch_add(obj, delta)`                       | Atomic add; return old      |
| `atomic_fetch_sub(obj, delta)`                       | Atomic subtract; return old |
| `atomic_compare_exchange_strong(obj, expected, new)` | CAS if equal                |
| `atomic_exchange(obj, new)`                          | Atomic swap; return old     |

### Memory Ordering

| Ordering               | Synchronization                   |
| ---------------------- | --------------------------------- |
| `memory_order_relaxed` | None (no synchronization)         |
| `memory_order_acquire` | Acquire semantics (load)          |
| `memory_order_release` | Release semantics (store)         |
| `memory_order_acq_rel` | Both acquire and release          |
| `memory_order_seq_cst` | Sequentially consistent (default) |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Data Races

**Pitfall**: Multiple threads access shared data without synchronization.

```c
int shared = 0;
// Thread 1: shared++;
// Thread 2: shared++;
// Race condition: final value unpredictable (0, 1, or 2)
```

**Fix**: Use atomic operations or mutexes.

### 3.2 Deadlock

**Pitfall**: Circular lock waiting.

```c
// Thread 1: lock(A), then lock(B)
// Thread 2: lock(B), then lock(A)
// Deadlock: both waiting for other's lock
```

**Fix**: Always acquire locks in same order; use timeouts; prefer lock-free algorithms.

### 3.3 Use-After-Free with Threads

**Pitfall**: Freeing shared data while thread accesses it.

```c
struct Data* p = malloc(sizeof *p);
thrd_create(&th, worker_thread, p);
free(p);  // UB: thread still using p
```

**Fix**: Join before freeing; use reference counting.

### 3.4 Condition Variable Spurious Wakeups

**Pitfall**: Condition variable may wake without condition met.

```c
if (cnd_wait(&cv, &mtx) == thrd_success) {
    // Condition not guaranteed! May be spurious wakeup
}
```

**Fix**: Always use while loop checking condition.

```c
while (!condition) {
    cnd_wait(&cv, &mtx);
}
```

### 3.5 Atomic Ordering Misunderstanding

**Pitfall**: Using `memory_order_relaxed` expecting synchronization.

```c
// Thread 1
atomic_store_explicit(&x, 1, memory_order_relaxed);

// Thread 2
while (atomic_load_explicit(&x, memory_order_relaxed) == 0) ;
// NOT guaranteed to see Thread 1's write!
```

**Fix**: Use `memory_order_release`/`memory_order_acquire` or `seq_cst`.

## 4. Comprehensive C23 Working Example

```c
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
```

**Key Takeaways**:

- Threads enable concurrent execution; require synchronization to prevent races.
- Atomic operations provide low-level safe concurrent access.
- Mutexes protect critical sections; condition variables coordinate threads.
- Memory ordering controls visibility guarantees; most code uses sequential consistency.
- Deadlock, use-after-free, and spurious wakes are common threading pitfalls.
