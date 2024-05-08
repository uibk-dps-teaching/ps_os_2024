# Exercise Sheet 7

## Task 1

Programs frequently use _system calls_ to interact with the operating system, for example when spawning a child process or when performing everyday file operations.
It sometimes can be useful to inspect the system calls performed by a program, e.g. to debug its behavior when no source code is available, or simply to find out more about how it works.
Linux systems provide the `strace` utility to do just that.

Alongside this document you will find a binary file called [`secret`](secret), which has been compiled to run on ZID-GPL. You need to run the binary on ZID-GPL.

Begin by reading `man strace` to familiarize yourself with its output format.
Then, use `strace` to investigate the behavior of the provided binary. What is the binary trying to do? Try to use the binary as intended and report your findings.

## Task 2

In the provided file [task2.c](task2/task2.c) you can find an
implementation of the classic
[Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem).
However, the implementation is flawed: In some situations multiple philosophers
will wait forever for a chopstick being released, constituting a
**deadlock**.

- Explain the program.
- How can a deadlock occur? Does it always happen?
- Change the program to prevent any potential deadlocks from occurring.
  Submit your solution.
  Explain how your changes prevent deadlocks.
- How can you test your solution to ensure that it is deadlock-free?
- What are the advantages and disadvantages of your solution?
- Can you think of a computer science based real-world scenario where a similar problem could occur?

## Task 3

In this exercise you will implement a [`thread pool`](https://en.wikipedia.org/wiki/Thread_pool), containing a limited number of worker threads.

The provided code in [`task3.c`](task3/task3.c) computes a fixed amount of jobs (not important but for your interest, the job is computing the average time you win if betting on black while playing roulette.).

The file contains a version which spawns several threads. Compile the code with `make task3` and run it. Find out how many threads it spawns.

This file also contains a version which uses a thread pool interface, which is defined in [`thread_pool.h`](task3/thread_pool.h).
Compiling this (`make task3a`) will not work because the thread pool functions are not implemented yet.
More information and additional requirements about the thread pool implementation can be found in [`README.md`](task3/README.md).

**Note**: The provided values for `NUM_EVALUATIONS` and `NUM_SPINS` should be used for benchmarking, but you might want to use smaller values during development.

Complete the following tasks:

- Choose suitable data types for the thread pool implementation and implement the functions in [`thread_pool.c`](task3/thread_pool.c).
- Experiment with different thread pool sizes (change the `POOL_SIZE` constant). Which size results in the best performance and why?
- Compare the performance of `task3` and `task3a` (with the optimal thread pool size) by benchmarking them with `/usr/bin/time -v` and report your findings.
- Answer these questions:
  - What are the advantages of using a thread pool rather than spawning a new thread for each job?
  - In which situations does it make sense to use a thread pool?

---

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries.

```text
exc06_csXXXXXX.zip
├── Makefile             # optional
├── group.txt            # optional
├── task1
│   └── solution.txt
├── task2
│   ├── Makefile
│   ├── task2.c
│   └── solution.txt
└── task3
    ├── Makefile
    ├── solution.txt
    ├── roulette.c
    ├── roulette.h
    ├── task3.c
    ├── thread_pool.c
    └── thread_pool.h
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

