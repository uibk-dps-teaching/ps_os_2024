# Exercise Sheet 4

## Task 1

In this task, we provide code that should do some string manipulation using threads.
However, this code is quite buggy.
Your task is to find the bugs in this program and fix them.
Describe the bugs you found as comments in the source code file.
Explain what was wrong and why the fixed version is correct.

In order to make presenting the results easier,
comment out the buggy lines and add the correct code,
for example:

```c
// FOUND do not compare strings with ==
// if(str1 == str2) {
if(strcmp(str1,str2) == 0) {
```

### Intended behavior

In order to fix a buggy program it is of course necessary to know what it should do.
The Intended behavior is described in this section.

All command line arguments to the program (including `argv[0]`) should be processed individually.
First, all command line arguments should be copied into heap memory.
Then, one thread per command line argument is spawned (so in total `argc` threads),
and each thread works on a single one of these arguments.

Each thread receives a pointer to the copy of the argument it should work on.
The job of the thread is shifting this argument to the right by two (2) positions (into a new array),
and converting the input string to upper case (in-place).
In order to make the shifted string accessible to the main thread,
a pointer to it should be returned by the thread.

Once the main thread is done spawning the threads, it waits for their completion
and for each completed thread, the original argument it received, the upper-cased version,
and the shifted argument are printed.

### Debugging

The bugs in this program are mostly issues with memory accesses, such as accessing addresses that are not part of an allocation or leaking dynamically allocated memory.

One tool which can be very helpful for finding such issues is `valgrind`.
This tool can run a binary and check if it accesses any invalid memory addresses or leaks memory. To get more informative messages, make sure to pass the flag `-g` to the C compiler.

A run of `valgrind` on the corrected version of the program might look like this:

```text
$ valgrind ./task1 some Arguments 4297
==3083991== Memcheck, a memory error detector
==3083991== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==3083991== Using Valgrind-3.21.0 and LibVEX; rerun with -h for copyright info
==3083991== Command: ./task1 some Arguments 4297
==3083991== 
original argv[0]: ./task1
uppercased: ./TASK1
shifted: k1./tas
original argv[1]: some
uppercased: SOME
shifted: meso
original argv[2]: Arguments
uppercased: ARGUMENTS
shifted: tsArgumen
original argv[3]: 4297
uppercased: 4297
shifted: 9742
==3083991== 
==3083991== HEAP SUMMARY:
==3083991==     in use at exit: 0 bytes in 0 blocks
==3083991==   total heap usage: 13 allocs, 13 frees, 2,168 bytes allocated
==3083991== 
==3083991== All heap blocks were freed -- no leaks are possible
==3083991== 
==3083991== For lists of detected and suppressed errors, rerun with: -s
==3083991== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

Note the absence of any error messages and memory leaks.
For debugging, it might be helpful to pass flags to `valgrind`,
in particular `--track-origins=yes`, `--leak-check=full`, and `--show-leak-kinds=all`.

It might also be helpful to use a debugger, either integrated in your editor/IDE or a standalone debugger like `gdb`.

## Task 2

On the shell you can provide the output of a command as input to another using a _pipe_ `|`.

```
$ cat file.txt | cut -c 22-
7
```

This _forwards_ the standard output of `cat` to the standard input of `cut` (remove sections from lines). 
The result you see is the content of [`file.txt`](task2/file.txt), minus the first 22 characters.
Multiple commands can be chained together to build longer _pipelines_.

```
$ cat file.txt | grep '^;' | cut -c 22-
3
```

Your task is to create a C program `cat_cut` that uses `fork` and `exec` to run the two commands `cat file.txt` and `cut` (with the argument `-c 22-`) like just shown.
Have a look at the `fork(2)`, `exec(3)`, `pipe(7)`, `pipe(2)`, and `dup(2)`/`dup2(2)` man pages.

Then, create a program `cat_grep_cut` that mimics the second example.
Your Makefile should build both executables.

There are even more helpful operators built into your shell.
Explain what the following operators do: `<`, `>`, `2>`, `2>&1`. Therefore, provide a task2.txt file.

## Task 3

In this task you will work with FIFOs, also called **named** pipes.

The goal is to write a simple **server** with a counter variable where multiple interactive **clients** can send number expressions to. The **server** will add these client numbers to the counter variable and print the result to the console.
Your solution should produce two separate executables, `client` and `server`. 

### Server

1. The **server** can be started with a list of clients that may connect to it, e.g. after starting with `./server jacobi riemann`, two clients named `jacobi` and `riemann` can connect.
2. The server then creates a FIFO for each client. Think about a way of associating each FIFO with a specific client.
   **NOTE**: If you have problems creating FIFOs, make sure that your filesystem actually supports them (this can be a problem in WSL when working on an NTFS drive). A good location to store FIFOs is somewhere inside `/tmp` (when working on ZID-GPL, be sure to avoid file naming collisions with other students).
3. The server then waits for clients to connect by opening each FIFO for reading. There is no need to use `fork(2)` in this task, you can assume that clients connect in the same order they are specified on the command line.
4. Once a client connects, the server prints the message `"<client name> connected."`.
5. The server continuously monitors all FIFOs for incoming math expressions using `poll(2)`.
   1. Once an expression is received, it is added to the counter variable and printed to the console like so `"<client name>: counter = <counter>."`.
   2. In the beginning, the server initializes the counter variable with  `0`. If an expression does not conform to the expected format, the server instead prints `"<client name>: <expression> is malformed."`.
   3. If a client disconnects, the server prints the message `"<client name> disconnected."`.
6. Once all clients are disconnected, the server cleans up the FIFOs and exits.

### Client

1. A **client** is started by providing it with a name, e.g. `./client jacobi`.
2. The client will then attempt to connect to the server by opening one of the FIFOs for writing.
3. Once connected, the client continuously prompts for a number expression to send.
4. Each expression is then written to the FIFO. You can assume an expression to be at most `PIPE_BUF` long. Why is this important?
5. If the expression is empty, the client disconnects by closing the FIFO and exiting.

Here is an example session (each line represents a different point in time, `>` indicates user input).

```
TERMINAL 1                     │ TERMINAL 2             │ TERMINAL 3
───────────────────────────────┼────────────────────────┼─────────────────────────
> ./server jacobi riemann │                        │
                               │ > ./client jacobi │
jacobi connected.              │ Expression:            │
                               │                        │ > ./client riemann
riemann connected.             │                        │ Expression:
                               │ > 2                    │
jacobi: counter = 2            │ Expression:            │
                               │                        │ > 3
riemann: counter = 5           │                        │ Expression:
                               │ > i                    │
jacobi: i is malformed.        │ Expression:            │
                               │ > (empty)              │
jacobi disconnected.           │                        │
```

Additional notes and hints:

- You can assume that all clients are connected before handling their messages.
- Your server doesn't need to support clients re-connecting, i.e. they can (and must) connect exactly once.
- Your solution **must** use `poll(2)`, **not** `select(2)`.
- Make sure to use appropriate file permissions (which we discussed in connection to `chmod`) to create and open your FIFOs.
  Justify your choice.
- Your FIFOs should be **blocking**, i.e. you **must not** use `O_NONBLOCK` in your calls to `open(2)`.
- You can use the `%g` format specifier to let `printf(3)` decide on the number of significant digits to print.
- As always, make sure to properly release all allocated resources (e.g. FIFOs) upon exiting.
- On macOS, `poll(2)` is bugged and will not return when you close the pipe from the client,
  run on ZID-GPL to ensure the correct behaviour.
- If you want to create multiple processes side-by-side for testing, there are several different options available to you:
  1. Open multiple terminal windows or tabs.
  2. Use shell jobs to switch between different processes: <kbd>CTRL</kbd> + <kbd>Z</kbd>, `&`, `fg`, `bg`, `jobs`, etc.
  3. Use a terminal multiplexer such as `tmux` or `screen`

--------------

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries.

```text
exc04_csXXXXXX.zip
├── Makefile             # optional
├── group.txt            # optional
├── task1
│   ├── Makefile
│   └── task1.c
├── task2
│   ├── Makefile
│   ├── cat_grep_cut.c
│   ├── cat_cut.c
│   ├── file.txt
│   └── task2.txt
└── task3
    ├── Makefile
    ├── client.c
    ├── common.h       # optional
    └── server.c
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
