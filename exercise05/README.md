# Exercise Sheet 5

## Task 1

In this task we will look at **POSIX** message queues — see `mq_overview(7)`.

The goal of this task is to simulate a task scheduler, with multiple tasks being processed according to their priority. 
A task is sorting five integers. 
The simulation consists of two executables: The `scheduler` receives tasks via a **message queue** and "prints" progress and the sorted numbers by writing them to `stdout`.
One or more clients, simply called `service`, submit tasks to the message queue.

Scheduler behavior:

- The scheduler receives the name of the message queue as its only argument.
  - To avoid naming collisions with other students on ZID-GPL, we recommend naming your queue based on your user name. (e.g. /csXXXX)
- It creates the message queue and continuously waits for incoming tasks, taking priorities into account.
- Upon receiving a task, it prints `Scheduling task with priority <priority>:`, followed by 
  - A progress in percent (while running) 
    - (Hint you can use something like this: `printf("\rSorting progress: %d%%", (100 * (i + 1) / totalOperations));`)
    - This code uses a `\r`, which is a carriage return. It moves the cursor to its position, so the next printf overwrites the previous line.
  - The sorted numbers (when done)
- Simulate computation times by sleeping 500 milliseconds in each iteration. As soon as one iteration is done it prints the progress in percent.
    Make sure to flush `stdout` to see the progress steadily being printed.
- Use a sorting algorithm of your choice.
- Find a way of **gracefully** shutting down the server.
  You may find _"signaling"_ inspiration in earlier exercise sheets.
  Upon shutdown, the server discards all outstanding jobs, prints `Shutting down.`, cleans up and exits.

Service behavior: 

- The client receives the message queue name and a _priority_ as its arguments.
- It then opens the message queue created by the server, sends a request with its contents read from `stdin` and then immediately exits.
- The request should contain five numbers (integers), which are to be sorted.

Example output:

```text
┌──────────────────────────────────┬─────────────────────────────────────────────┬───────────────────────────────────────────┐
│TERMINAL 1                        │TERMINAL 2                                   │TERMINAL 3                                 │
├──────────────────────────────────┼─────────────────────────────────────────────┼───────────────────────────────────────────┤
│$ ./scheduler "/csXXXX"           │                                             │                                           │
│                                  │$ echo "1 10 5 4 1" | ./service "/csXXXX" 1  │                                           │
│Scheduling task with priority 1:  │                                             │                                           │
│Sorting progress: 100%            │                                             │                                           │
│Sorted numbers: 1 1 4 5 10        │                                             │$ echo "5 4 3 2 1" | ./service "/csXXXX" 10│
│                                  │$ echo "10 5 4 7 1" | ./service "/csXXXX" 99 │                                           │
│Scheduling task with priority 99: │                                             │                                           │
│Sorting progress: 100%            │                                             │                                           │
│Sorted numbers: 1 4 5 7 10        │                                             │                                           │
│                                  │                                             │                                           │
│Scheduling task with priority 10: │                                             │                                           │
│Sorting progress: 100%            │                                             │                                           │
│Sorted numbers: 1 2 3 4 5         │                                             │                                           │
└──────────────────────────────────┴─────────────────────────────────────────────┴───────────────────────────────────────────┘
```

In this example, the scheduler starts sorting the numbers received by the task with priority 1, since it is the first to be received.
While this computation is done, 2 other requests are added to the queue.
After the first task has been completed, the scheduler takes the next message (according to priority) from the queue.

Additional notes:

`Attention: POSIX message queues are from the POSIX Realtime Extension, which isn't part of the POSIX core. 
Thus, macOS doesn't implement it. 
Students using macOS need to do this exercise in a VM or on zid-gpl.`

- Pick a reasonable message size (static)
- Ensure the message queue is properly cleaned up on shutdown
- Make sure to use appropriate file access modes

Answer the following questions, in a file task1.txt, provide this file additionally to the source codes.

- What is the range of possible priorities defined by POSIX?
  - Are you limited to this range on your system?
  - What is more important a task with priority 2 or 99 (gets scheduled first)?


## Task 2

In this task, you will develop a multi-process application using shared memory for communication.

Begin by reading `man 7 shm_overview`. Take note of additional linker flags that may be required.

---

The program receives three integers as arguments of type `uint64_t`:

- `N`, an arbitrary integer
- `K`, number of reads/writes to the buffer
- `L`, the length of the circular buffer (total size: `L * sizeof(uint64_t)`)

First, each input argument is parsed (as discussed in the lab). Then, it sets up shared memory for communication. It contains a [circular buffer](https://en.wikipedia.org/wiki/Circular_buffer) and one element for the result. We provided you with a sample struct. Feel free to use it.

The circular buffer allows the number of reads/writes defined by `K` to be larger than the number of elements `L` in the buffer. If the end is reached, the process restarts at the front (using `buffer[i % L]`, where `i` is the number of iterations done so far).

Next, two child processes are created.
The processes run in parallel and perform calculations on the buffer:

- **Child A**:
  - The process loops `K` times, starting from 0. In each iteration `i`, the number `N * (i + 1)` is written into position `i % L` of the circular buffer.
  - It then finishes up, and returns success
- **Child B**:
  - The process computes the sum of each element in the circular buffer. It prints the final result, and writes it into the result element in the shared memory.
  - It then finishes up, and returns success

The following struct might help:

```c
struct TODO {
    uint64_t result;
    uint64_t buffer[];
};
```

In this struct, `buffer` is a so-called [flexible array member](https://en.wikipedia.org/wiki/Flexible_array_member).

The parent process waits for the termination of both child processes. It reads the result of their computation from the result element in the shared memory. It then validates the result of the computation using the following function.
It then finishes up, and returns success.
```c
void validate_result(uint64_t result, const uint64_t K, const uint64_t N) {
    for (uint64_t i = 0; i < K; i++) {
        result -= N * (i + 1);
    }
    printf("Checksum: %lu \n", result);
}
```

### Example outputs

```sh
$ ./task1 32044 3042 3040
Result: 148312545732
Checksum: 0
```

```sh
$ ./task1 32044 3042 3040
Result: 148507373252
Checksum: 194827520
```


You will notice that the result is not always correct. This is by design and does not indicate an implementation mistake.

- Test your implementation with different values for `N`, `K`, and `L`.
- What is the checksum indicating?
   - Which values are correct vs. incorrect?
   - How can the checksum be used to tell if a result is correct or not?

Additional notes and hints:

> ℹ️ **Macro support**:
>
> In order to use `sigaction`, `usleep`, or macros such as `MAP_ANONYMOUS` you have to define `_POSIX_C_SOURCE` and `_BSD_SOURCE` feature test macros.
> This can be done by adding the compile flag `-D_POSIX_C_SOURCE -D_BSD_SOURCE`.
> See the `feature_test_macros(7)` man page for more information.
> On modern systems the `_BSD_SOURCE` feature test macro has been deprecated and replaced by `_DEFAULT_SOURCE`, however this is not available on zid-gpl.
> You may ignore the deprecation warning that is emitted when using `_BSD_SOURCE`, however your program should not generate any other warnings.

- Make sure to perform proper error handling and input parsing
- Test your implementation using `valgrind`
  - Did you clean everything up?
  - Do you have access to everything you need?
  - Is everything correctly initialized?

## Task 3

In this task, you will use _semaphores_ to synchronize the access to the shared data structure.

This exercise MUST be implemented using the POSIX semaphore facilities.

`Attention: POSIX semaphores are from the POSIX Realtime Extension, which isn't part of the POSIX core. 
Thus, macOS doesn't implement it. 
Students using macOS need to do this exercise in a VM or on zid-gpl.`

### Task 3a

Implement the same program as in the previous task 2. This time however, ensure that the checksum is always correct by utilizing synchronization concepts.

Use two semaphores to implement this behavior.

Your program should now always compute the correct checksum.

**Important**: Make sure that the processes can perform work on the circular buffer _concurrently_, i.e., the semaphores must be used for _counting_.

Measure the execution time of your solutions for Task 2 and Task 3a using `/usr/bin/time`.
What do you notice?

### Task 3b

Answer the following questions:

- What is the difference between shared memory and e.g. a global variable after calling `fork()`?
- What is a race condition? Why do race conditions occur?
- Is the output of a program with a race condition always incorrect?
- If you run your program 100 times and always get the correct result, does that mean that there is no race condition?
- What is a critical section? What role does it play in preventing race conditions?
- What is synchronization?
- Why are semaphores well suited for synchronizing such programs?
- Could you use also one semaphore to solve this this program?
- There are two ways of creating a POSIX semaphore. Which would you use when?

---

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries.

```text
exc05_csXXXXXX.zip
├── Makefile             # optional
├── group.txt            # optional
├── task1
│   ├── Makefile
│   ├── task1.txt
│   └── scheduler.c
│   └── service.c
├── task2
│   ├── Makefile
│   ├── solution.txt
│   └── task2.c
└── task3
    ├── Makefile
    ├── solution.txt
    └── task3.c
```

Requirements

- [ ] Auto-format all source files
- [ ] Check your submission on ZID-GPL
- [ ] Check your file structure
- [ ] Submit zip
- [ ] Mark solved exercises in OLAT
- [ ] Any implementation MUST NOT produce any additional output
- [ ] If you work in a group, create a `group.txt` file according to the format specified below.

If you worked in a group, the `group.txt` file must be present and have one line per student which contains the proseminar group in the beginning, followed by the matriculation number and the student's name. For example, if the group consists of Jane Doe, who is in proseminar group 2 and has matriculation number 12345678, and Max Mustermann, who is in proseminar group 3 and has matriculation number 87654321, the `group.txt` file should look like this:

```text
ps_group_2 12345678 Jane Doe
ps_group_3 87654321 Max Mustermann
```
