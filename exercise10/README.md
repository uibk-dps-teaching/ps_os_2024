# Exercise Sheet 10

## Task 1

In this task you'll simulate a simple dice rolling game using **only** *pthread barriers* for synchronization.
Start by reading the man pages on how to use barriers, `man 3 pthread_barrier_init` and `man 3 pthread_barrier_wait`.

The game is started with `n` players, specified as a command line argument.
Each round all of the players roll a die (generate a random number between 1 and 6 for this).
The player(s) with the highest score are then eliminated from the game.
If all remaining players roll the same number, the round is repeated until a winner has emerged.
The game is finished if only one player is left.

- The main thread **only** does setup and cleanup.
- Each player is simulated by one dedicated thread.
- One of the players (**not the main thread**) determines which player(s) are eliminated.
  Use `pthread_barrier_wait`'s return value for this.
- You **must not** use any global variables!
- It might be helpful to define a struct to combine multiple arguments that are passed to a thread function.
- You might also want to use a struct to save your game state. This could look something like this:

```c
typedef struct {
  uint32_t n;
  volatile int rolls[];
} game_state;
```

Example output where player 3 wins the game:

```
$ ./task1 5
Player 0 rolled a 5
Player 1 rolled a 3
Player 2 rolled a 5
Player 3 rolled a 1
Player 4 rolled a 3
Eliminating player 0
Eliminating player 2
---------------------
Player 1 rolled a 1
Player 3 rolled a 1
Player 4 rolled a 4
Eliminating player 4
---------------------
Player 1 rolled a 3
Player 3 rolled a 1
Eliminating player 1
---------------------
Player 3 has won the game!
```

**Note:** The pthread barrier functionality will only compile if you define the right feature test macro. In this case, you can define `-D_DEFAULT_SOURCE` when compiling your code.

## Task 2

`some_program` is an executable that should run on most `x86_64` Linux machines (and in particular it runs on ZID-GPL).
`some_library.so` is a _shared library_ used by the executable.

You can run the executable like so:

```sh
LD_LIBRARY_PATH=. ./some_program
```

Unfortunately, the library contains bugs.
While the library implementation is available in [some_library.c](some_library.c), the name of the function got corrupted.

- Use `nm` to find the name of the function (its name will tell you the intended behavior).
- Read up on how GCC can be used to compile shared libraries (i.e. _shared objects_). You should start by looking at the [linking options](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html) GCC offers.
- Fix the bug in the library function.
- Write a `Makefile` to compile the revised version of `some_library.so`.
- Verify that `some_program` now behaves as expected.
- Is `some_library.so` the only shared library used by `some_program`?
  Use `ldd` to check.

Then:

- Re-implement the functionality of `some_program` and `some_library.so` in a single program `my_program`.
- Extend your `Makefile` to compile this program as a _static_ executable (again, have a look at the [linking options](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html) and find out how this can be done with GCC).
- Compare the sizes of `some_program` and `my_program` — what do you find?
- Verify that your executable is actually static. Use `ldd` and `file` for this.

Equipped with this knowledge, answer the following questions:

- What is the difference between dynamic and static linking?
- When would you use dynamic linking?
- When would you use static linking?
- What is _position independent code_, and why is it useful?
- What is the purpose of the `LD_LIBRARY_PATH` environment variable?

**Note:** You can use `objdump` to disassemble a binary.
Inspecting a binary obtained from an unknown source before running it is always a good idea!

```sh
objdump --disassemble some_program --section=.text
```

**Note 2:** Linking a fully _static_ executable is not possible on ZID-GPL as a static version of the standard library is missing.

## Task 3

Dynamic linking is usually handled by the loader when running an executable. 
However, in some situations, a more manual approach is required. 
In particular, _plugin systems_ are commonly implemented this way.

Write a program called `map_string` that receives (as arguments) a string followed by a list of shared libraries that act as _plugins_ for the program.

The initial string is used as input for the first plugin. The result of the first plugin is used as input for the second plugin, and so on.

To achieve this, first read the man pages for `dlopen(3)` and `dlsym(3)`.

This kind of plugin system can be very useful for implementing so called _filter chains_. In our case, we want to implement a _filter chain_ that, according to the plugin order, modifies the input string such that the output can be reversed to the original string by running the plugins in reverse order. This can be very useful for implementing encryption and decryption algorithms, with the added benefit that the encryption and decryption algorithms can be extended at the time of program execution.

To do this, you have to implement the following plugins:
* `caesar_cipher.so`: Shifts all letters in the string by 13 positions. Shifting can be performed by adding the integer value 13 to a character. You might need to work with the modulo operator. Consider [this](https://en.wikipedia.org/wiki/Caesar_cipher) for more details.
* `xor_string.so`: XORs all letters in the string with the value 0x20. XOR can be performed on a character using the `^` operator.
* `reverse_string.so`: Reverses the string. 

For example, given those plugins:
```sh
LD_LIBRARY_PATH=. ./map_string "Hello World" ./caesar_cipher.so ./xor_string.so ./reverse_string.so
./caesar_cipher.so: Uryyb Jbeyq
./xor_string.so: uRYYB jBEYQ
./reverse_string.so: QYEBj BYYRu
```

Running these specific plugins in reverse order should yield the original string:
```sh 
LD_LIBRARY_PATH=. ./map_string "QYEBj BYYRu" ./caesar_cipher.so ./xor_string.so ./reverse_string.so
./caesar_cipher.so: DLROw OLLEh
./xor_string.so: dlroW olleH
./reverse_string.so: Hello World
```
Additionally, think of two different plugins similar to the ones in the example above and implement them. If you can't think of any reversible plugins, you can also implement a plugin that modifies the string in a way that it can't be reversed to the original string.

---

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries.

```text
exc10_csXXXXXX.zip
├── Makefile             # optional
├── group.txt            # optional
├── task1
│   ├── Makefile
│   └── task1.c
├── task2
│   ├── Makefile
│   ├── solution.txt
│   ├── some_library.c
│   └── my_program.c
└── task3
    ├── ceasar_chipher.c
    ├── Makefile
    ├── map_string.c
    ├── reverse_string.c
    └── xor_string.c
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
