# Exercise Sheet 3

## Task 1

This task is about working with permissions.

The provided archive [`task1.tar`](task1/task1.tar) contains two files. One of them is a shell script called `exercise.sh` and the other a text file, `file.txt`.

Please use the following command to unpack the archive:
```sh
tar -xpf task1.tar
```

Start by executing the shell script `exercise.sh`. The script will tell you, what you need to do and what needs to be done with `file.txt`. Therefore, ensure that the file is in the same folder as the script. Do **not** edit the files, other than their permissions.
To execute the script, run `./exercise.sh`. This execution might not initially work, then look at your permissions and adjust them as required.

When you list the contents of a directory with details you can see the permissions, owning user, and owning group among other details. For example:

```sh
$ ls -l
drwxrwxr-x. 2 c7012345 lxusers 4096 Mar 9 19:11 task_1_queue
-rw-rw-r--. 1 c7012345 lxusers 1088 Mar 9 04:20 task_2_summary.txt
-rw-rw-r--. 1 c7012345 lxusers   61 Mar 9 00:07 task_3_summary.txt

 ↑            ↑        ↑
 Permissions  User     Group
```

*Hint:* Take a look at `chmod(1)`.

## Task 2

Create a program `calculator.c` which simulates a simple calculator. The program should accept any basic arithmetical operation as the first argument (`+`,`-`,`*`,`/`), followed by *at least one* number. The provided operator is then used to calculate the result using floating point arithmetic. If only one number is provided, this number is the expected result (for any valid operator). Consider using `strtod(3)` for parsing the input numbers.

Example output:

```bash
$ ./calculator
Usage: ./calculator <operator> <operand1> [operand2 ...]
Available operators: +,-,*,/
$ ./calculator '*' 1.0
Result: 1.00
$ ./calculator '/' 12.0 2.0 3.0
Result: 2.00
```

The goal of this exercise is to perform proper *error handling* and return according *exit codes*.

Below is a list of constraints for each available *exit code*:

- return `EXIT_SUCCESS` if the run is successful
- return `13` if too few arguments are provided (e.g.: `./calculator '+'`). In addition, print usage information.
- return `42` if the operator is unknown (e.g.: `./calculator '%' 2.0 3.0 4.0`). Again, print usage information.

For better readability, define an `enum` for non-standard *exit codes*.

Think of at least **two** other constraints and add appropriate *exit codes*.

Extend your program to add the value of an *environment variable* `OFFSET` to the calculated result. For example, when `OFFSET` is set to `42.5`, the call `./calculator '*' 2.0 3.0` should return `48.50`. Have a look at `getenv(3)`. In order to test your program, figure out a way to set/unset environment variables in your shell. Additionally, find a way to set the environment variable *only* for a specific command (in this case `./calculator` with appropriate arguments).

You can inspect environment variables on your shell with `printenv` and print them with `echo $MY_ENV_VAR`.

Answer the following questions in `solution.txt`:

- How do you set/unset environment variables in your shell?
- How can you retrieve the returned *exit code* in your shell after running the program?
- In your shell `;`, `&&`, and `||` can be used to execute multiple commands on a single line. What are the differences between these 3 operators?
- What is the `PATH` environment variable and why is it important?

## Task 3

Begin by skimming through `man pthreads`.
Take note of additional compiler flags that may be required.

### Task 3a

Write a program that initializes a global variable `accumulation` of type `int64_t` and initializes it to `0`.

The program begins by printing the value of `accumulation` and then creates a child process using fork.
The child accumulates the values from `1` to `N = 10` into `accumulation` and exits.
The parent waits for the child to exit and prints the value again.

Next, the program (now referred to as the _main thread_) spawns a single POSIX thread.
The thread does the same thing as the child process, i.e. accumulate the values from `1`to `N = 10` into the global variable and immediately exit.
The main thread waits for the thread to finish, and prints the value one more time.

What do you notice? Explain the behavior.

Make sure to check for possible errors that might occur in the program. Look at return codes of functions like `pthread_create` and print meaningful error messages to standard error using `fprintf`

### Task 3b

Write a program that receives an arbitrary number of files as arguments ([task3](task3/) contains such files). 
Let `N` be the number of arguments provided by the user.

The program creates `N` threads, each of which is assigned an ID `i`, with `0 < i <= N`.
The `i`-th thread computes a sum of all numbers contained in the file of the `i`-th argument, i.e. the first thread computes the sum of the first file, the second thread computes the sum of the second file, and so on. 

The main thread then waits for all threads to complete and prints their result _in order_ in the format `"sum <i> = <sum>"`. Lastly the main thread sums up all sums from each thread and prints the result. `"total sum = <sum>"`.

Investigate how you can pass multiple arguments to a thread function, as well as how to receive a result from it.
The program **must not** make use of any global variables.

Ensure that all threads compute their sums concurrently, not one after another.

Example output:

```sh
$ ./task3b ./random_one.txt ./random_two.txt ./random_three.txt ./random_four.txt ./random_five.txt
sum 1 = 510306
sum 2 = 493393
sum 3 = 497804
sum 4 = 508863
sum 5 = 498981
total sum = 2509347
```

Again, make sure to check for possible errors that might occur in the program. Look at return codes of functions like `pthread_create` and print meaningful error messages.

_Hint_: If you are using memory in multiple threads, make sure it is available and doesn't run out of scope, while it might still be used in another thread.

--------------

Submit your solution as a zip archive via [OLAT](https://lms.uibk.ac.at/), structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries.

```text
exc03_csXXXXXX.zip
├── Makefile             # optional
├── group.txt            # optional
├── task1
│   └── task1.txt
├── task2
│   ├── Makefile
│   ├── calculator.c
│   └── solution.txt
└── task3
    ├── Makefile
    ├── random_five.txt
    ├── random_four.txt
    ├── random_one.txt
    ├── random_three.txt
    ├── random_two.txt
    ├── task3a.c
    └── task3b.c
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
