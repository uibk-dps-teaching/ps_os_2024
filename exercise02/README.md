# Exercise Sheet 2

From here on, `Makefiles` with `all` (default) and `clean` rules are required. Your source code must compile by running make in the corresponding folder.

## Task 1

In this task you will familiarize yourself with the build system `make`.

### Using make for building C code

In the directory [task1](task1/) we provide some source code.
This exercise doesn't involve coding C, therefore don't modify the files in task1.
The task here is to create a Makefile in this directory which can be used to prepare and compile the provided code.

Your Makefile shall meet the following criteria:

- The rules `all`, `create_main` and `clean` must be available
    - `all` should prepare (create the `main.c` file - see below) and build the code.
    - `create_main` should create the missing `main.c` file. (`>>` from shell scripting might be helpful).
    This main file should look exactly like this:
    ```c 
    #include "hello.h"
    #include "goodbye.h"
    #include <stdlib.h>

    int main(void) {
        hello();
        goodbye();
        return EXIT_SUCCESS;
    }
    ```
    - `clean` should remove all artifacts as well as the created executable and the `main.c` file.
- When running just `make` in the `task1` directory, the `all` rule should be executed.
- All calls to the C compiler should use the flags `-std=c11 -Wall -Wextra`.
- Make sure to use `.PHONY` correctly (see [here](https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html)).
- The C files should first be compiled into intermediary object files (`hello.o`, `goodbye.o` and `main.o`),
  which are then combined into the final executable in a separate rule.
- For clarity, the make file should echo `Compiling xxx`, before actually compiling the object files. (rule prerequisites might help here.)

If everything works correctly, you should be able to call `make` in the directory `task1`,
which generates the executable `main`, and some object files. This call should output this (Note the compiler commands might differ depending on your system):

```
Compiling main:
cc -std=c11 -Wall -Wextra   -c -o main.o main.c
Compiling hello:
cc -std=c11 -Wall -Wextra   -c -o hello.o hello.c
Compiling goodbye:
cc -std=c11 -Wall -Wextra   -c -o goodbye.o goodbye.c
cc   main.o hello.o goodbye.o   -o main
```

Running this executable should print:

```
Hello World.
Bye World.
```
When running `make clean`, the executable, `main.c` and the object files should be deleted.

Comprehensive documentation of GNU make can be found [online](https://www.gnu.org/software/make/manual/html_node/index.html).

### Using make to automate other tasks

`make` is mainly used for building code, but it can also be helpful for many other tasks.
For each rule, the recipe which is executed can run arbitrary shell commands (as seen before with creating the `main.c` file).

The idea of this part of the task is automating part of the homework submission process:

1. Cleaning up build artifacts from the different tasks.
2. Building the zip archive which you can then submit.

For all tasks in which you work with C code there should be a Makefile with a `clean` rule in the corresponding directory,
as specified in the previous part of the task.
Create a Makefile in the directory which contains the directories `task1`, `task2`, and `task3`.
This Makefile should also have a `clean` rule, which calls `make clean` in the directories of the three different tasks
(you do not have to do it for tasks 2 and 3 if you do not submit them).

Additionally, define a rule called `zip` which first deletes the archive if it already exists
and then zips the content of the directory in which this Makefile is stored.
The generated zip archive should only contain the files we want in the submission,
so there should be no extra files in this directory or any child directories.
To ensure that no build artifacts are included in the archive,
write the `zip` rule in such a way that it first calls the `clean` rule described in the previous paragraph.

_Hint_: For information on how `make` should be called in a Makefile, check the [GNU make documentation](https://www.gnu.org/software/make/manual/html_node/Recursion.html).

_Hint_: You can recursively zip a directory with `zip -r ../archive.zip .`
This will put everything contained in the current directory into the archive, which is stored in the parent directory.

Try to write your Makefiles in a way which allows you to continue using them for future homework with minimal modification.

## Task 2

Create a program that registers handler(s) for the signals `SIGINT`, `SIGSTOP`, `SIGCONT`, `SIGKILL`, `SIGUSR1`, and `SIGUSR2` using `sigaction` (you must **not** use `signal`).
When a signal is received, it outputs a message indicating which signal was received.
After registering the signal handler(s), call `usleep` in a loop to prevent the process from exiting. On a high level, your program should look as follows:

```C
// include necessary header files

// define signal handler

int main(void) {
    // use sigaction to register signal handler

    // allow required signals

    while (true) {
        usleep(100);
        // more code (if needed)
    }

    return EXIT_SUCCESS;
}
```

Have a look at the following man pages `sigaction(2)`, `signal(7)`, and `signal-safety(7)`.
Read also [SIG30-C](https://wiki.sei.cmu.edu/confluence/display/c/SIG30-C.+Call+only+asynchronous-safe+functions+within+signal+handlers) and [SIG31-C](https://wiki.sei.cmu.edu/confluence/display/c/SIG31-C.+Do+not+access+shared+objects+in+signal+handlers).

You can use the shell commands `ps` and `pgrep` to find the process ID (PID) of a process.
`kill` can be used to send a signal to a process.

What difference do you notice between `SIGINT` and `SIGKILL`?

What happens when you press `CTRL + c` while the program is running?

What can the signals `SIGSTOP` and `SIGCONT` be used for?
for
_Note:_ In order to use `sigaction` and `usleep` you have to define `_POSIX_C_SOURCE` and `_BSD_SOURCE` feature test macros.
This can be done by adding the compile flag `-D_POSIX_C_SOURCE -D_BSD_SOURCE`.
See the `feature_test_macros(7)` man page for more information.
On modern systems the `_BSD_SOURCE` feature test macro has been deprecated and replaced by `_DEFAULT_SOURCE`, however, this is not available on `zid-gpl`.
You may ignore the deprecation warning that is emitted when using `_BSD_SOURCE`, however, your program should not generate any other warnings.

## Task 3
Write a program that creates `N` child processes using `fork(2)`, where each one estimates the probability of rolling a specific target number `T` on a single dice through a method based on repeated trials, referred to as a **dice-roll simulation**, using `S` steps. The values for `N` and `S` can be taken from the command line. `T` should be randomly generate in each child process.

First, the parent process creates **all children processes**, then **awaits** until they have exited, and finally prints `Done.`. The parent must wait for all children **only after** they have **all** been created.

Every child:

- Computes the probability of rolling a specific target number `T` on a single dice using a "dice-roll" simulation with `S` steps.
- prints a message of the form `Child <i> PID = <PID>. DR_p(<T,S>) = <DR_p(T,S)>. Elapsed time = <t> (s).` where
  `<i>` is the child number,
  `<PID>` is the child's process id,
  `<T>` is the target number on a single dice rolling,
  `<S>` is the random samples parameter,
  `<DR_p(S)>` is the calculation result,
  and `<t>` is the elapsed time since the parent was started.

Make sure to only once measure the starting time of the parent process.

_Hint:_ You can use `clock_gettime()` (have a look at the man page `clock_gettime(2)`) to measure wall time. For this to work you have to define `-D_POSIX_C_SOURCE=199309L` feature test macros (see Task 2).

Use the following implementation for the dice-roll simulation:

```c
double DR_p(int T, int64_t S) {
	int64_t hit_count = 0;
	for (int64_t i = 0; i < S; ++i) {
		const int roll = rand() % 6 + 1;
		if (roll == T) {
			hit_count++;
		}
	}
	return (double)hit_count / S;
}
```
_Hint_: A parent process can wait for its children using `wait(2)` or `waitpid(2)`.

Example output:

```sh
$ ./task3 3 100000000
Child 0 PID = 2942722. DR_p(2,100000000) = 0.166671. Elapsed time = 7.931573 (s)
Child 2 PID = 2942724. DR_p(4,100000000) = 0.166730. Elapsed time = 8.062064 (s)
Child 1 PID = 2942723. DR_p(5,100000000) = 0.166717. Elapsed time = 8.099407 (s)
Done.
```

Experiment with different values for `N` and `S`.
Analyze the obtained output.
Is the order of the messages consistent?
Can the order of these messages be predicted?
What does it depend on?

Notice that the function `DR_p` uses `rand(3)` to repeatedly generate pseudo-random numbers.
By default, this function returns the same sequence of numbers every time.
To get different estimates from each process, _seed_ the random number generator using `srand(getpid())`.
Does it matter whether you do this before or after the call to `fork()`?
Explain your answer.

--------------

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries.

```text
exc02_csXXXXXX.zip
├── Makefile             # second Makefile from task1
├── group.txt            # optional
├── task1
│   ├── Makefile
│   ├── goodbye.c
│   ├── goodbye.h
│   ├── hello.c
│   └── hello.h
├── task2
│   ├── Makefile
│   ├── signal_handler.c
│   └── solution.txt
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
