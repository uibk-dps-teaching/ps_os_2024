# Exercise Sheet 1

This exercise sheet contains the self-assessment exercise along with some introductory exercises to using the command-line.

## Task 1

This first task does not require the use of any operating systems concepts besides what you have already learned in the previous semester.
Instead, the purpose of this task is to check (for yourself) whether you are sufficiently fluent in using the C programming language.
Most exercises in this course will build directly on top of what you've learned in the previous semester.
If you are struggling with this task, consider doing this course a year later, familiarizing yourself with the C Programming language in the meantime.
The workload might otherwise be higher for you than originally planned by us.

The first task requires you to implement a `singly linked list` in C, containing a contact (first name, last name, age, email). A `singly linked list` is a data structure that consists of a sequence of dynamically allocated nodes. Each node contains a pointer to the next node in the sequence and a data element (in our case `void* data`). The last node in the sequence has a `NULL` pointer for the next node. The list itself is represented by an `address_book` struct, which contains a pointer to the first node in the sequence. The files `addressbook.h` and `addressbook.c` contain the function signatures and the function implementations as well as the struct definitions. **You should implement the functions in `addressbook.c`. All other files must not be modified.**

The following functions should be implemented:
  * `address_book_t* create_address_book();` - creates a new empty address book
  * `contact_t* create_contact(char* first_name, char* last_name, int age, char* email);` - creates a new contact
  * `void add_contact(address_book_t* address_book, contact_t* contact);` - adds a contact to the address book at the end of the list
  * `void remove_contact(address_book_t* address_book, contact_t* contact);` - removes a specific contact from the address book
  * `contact_t* find_contact(address_book_t* address_book, char* first_name, char* last_name);` - finds a contact by first and last name
  * `contact_t* duplicate_contact(contact_t* contact);` - duplicates a contact by creating a new contact with the same data
  * `address_book_t* filter_address_book(address_book_t* address_book, bool (*filter)(contact_t*));` - creates a new address book containing only the contacts that satisfy the filter function
  * `void sort_address_book(address_book_t* address_book, bool (*compare)(contact_t*, contact_t*));` - sorts the address book using the compare function (sorting algorithm is up to you, *don't complicate it too much*)
  * `bool compare_by_name(contact_t* contact1, contact_t* contact2);` - compares two contacts by their first and last name
  * `bool is_adult(contact_t* contact);` - returns true if the contact is an adult (age >= 18)
  * `size_t count_contacts(address_book_t* address_book);` - returns the number of contacts in the address book
  * `void free_address_book(address_book_t* address_book);` - frees the memory of the address book and all its contacts

The `main.c` file contains test functions that should test the functionality of your implementation as well as the memory management. 

To compile the program, a `Makefile` is provided. As of now, you don't need to understand the `Makefile` in detail, but it simplifies the compilation process. 

The following commands are available in the `Makefile`:
  * `make` - compiles the program
  * `make clean` - removes the compiled files and resets terminal color
  * `make test` - compiles the program and runs the executable
  * `make check` - compiles the program and checks the output of the executable with the file expected_output.txt (if everything works, "Test successful" is printed. Note that this command might not work on all systems, run on ZID-GPL to ensure the correct behavior.)

Some notes:
  * The tests are written in a way that they work incrementally. This means that you should have a look at the tests and implement the functions in the order in which they are tested. If this is done, you should be able to uncomment the tests one by one and see them pass, one by one. (Comments above the test calls hint at which functions need to be implemented for the test to work (incrementally))
  * Some of the functions are quite simple and can be implemented in a few lines of code, so don't be overwhelmed by the number of functions to implement.
  * Reuse the functions you have already implemented.
  * If you don't understand the code it includes, don't worry about it. Those contain the test framework and memory counting functions. You don't need to understand them to implement the functions.
  * The expected output provided in the file `expected_output.txt` is the full output the tests should produce, except for the: Total allocated memory 6772 bytes and Total freed memory 6772 bytes. These two lines could be different depending on your system. (The `make check` command does ignore these two lines)
  * A `print_address_book` function is provided to print a full address book. You can use this function to print the contacts in the tests (but remove all calls of `print_address_book` before handin).

## Task 2

As a computer scientist you will likely spend a lot of your time in a terminal.
It is therefore a good idea to get accustomed to a few basic commands.
We will gradually introduce new commands and concepts over the semester.

For now, you should teach yourself how to do the following tasks.
Introductory [slides from the previous year](../lab01/history_and_shell.pdf) are provided, but you are free to use any tutorial you find.

- Connecting to ZID-GPL via `ssh`
  - See [LPCCS System](https://www.uibk.ac.at/zid/systeme/linux/)
  - If the `ssh` command is not available on your system, install the *OpenSSH Client*
    - Windows: Settings > Apps & features > Optional features > OpenSSH Client
    - Ubuntu: `sudo apt install openssh-client` in your terminal
    - MacOS: `brew install openssh` (https://brew.sh/) 
  - You can close the connection by logging out with the `exit` command

- Looking around, changing directory (`pwd`, `ls`, `cd`)
- Creating files and directories (`touch`, `mkdir`)
- Copying, moving, deleting files and directories (`cp`, `mv`, `rm`)
- Filtering an input (`grep`)
- Redirect output of a command `>` (e.g. `ls > text.txt`)
- Chaining commands with a pipe `|` (e.g. `ls | grep "txt"`)
- Using a terminal text editor (`nano`)
  - `^X` means pressing `CTRL + x`
- Viewing history of previously used commands (`history`)
- Print content of a file (`cat`)
- Getting help
  - `man` to view the manual (man) page of a command
  - `help` to get information on shell built-ins (e.g. `help cd`)
    Try this if there is no man page available
  - `info` to view the info page of a command
    This can sometimes give you more information than the man page
  - `apropos` to search for man pages
  - Appending `--help` to a command (e.g. `ls --help`)

Next, read the following man pages to get a better understanding of your surroundings:

- Read `man intro`
- Skim over `man man-pages`, take note of the sections
- Skim over `man file-hierarchy`
- Figure out the difference between `man printf` and `man 3 printf`

Finally:

- Navigate to your home folder (`~`), create a new folder, create a new file in that folder
- List the content of your homefolder and redirect the output to your newly created file
- Figure out a way to copy the file from ZID-GPL to your local system and vice versa
- How can you retrace these steps in the future?
  - How can you search for previously used commands?

Put together a summary of all the commands and man pages mentioned above.
- Write the summary on ZID-GPL using `nano`

## Task 3

The program [`list_args.c`](task3/list_args.c) prints its commandline arguments when run, one per line. Use `make` to build the executable `list_args`.
Figure out how to run the program to produce the following output:

```
0: ./list_args
1: one
2: two words
3: "quotes"
4: 'more' "quotes"
```

- After that, use a single-line `bash` command that retrieves your program call from your shell history and writes it into a new file called `args.txt`. Have a look at the `history(1)` man page. After this step a file `args.txt` should have been created containing the commandline arguments to produce the expected output.
- Provide the used command in a new file `history.txt` and explain it (below the command).
- Finally, use a single-line `bash` command to read the program call from `args.txt` and execute it. Provide the used command in a new file `run.txt`. Have a look at the `eval(1)` man page.

After creating all three files (`args.txt`, `history.txt` and `run.txt`), use `make handin` to generate a file `task_3.txt` and verify that it contains your solutions.

--------------

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries.

```text
exc01_csXXXXXX.zip
├── group.txt            # optional
├── task_1
│   ├── Makefile
│   ├── addressbook.c
│   ├── addressbook.h
│   ├── expected_output.txt
│   ├── includes
│   │   ├── colorfull_print.h
│   │   ├── memory_tracking.c
│   │   ├── memory_tracking.h
│   │   ├── testing.c
│   │   └── testing.h
│   └── main.c
├── task_2.txt
└── task_3.txt
```

Requirements

- [ ] Auto-format all source files
- [ ] Check your submission on ZID-GPL
- [ ] Check your file structure
- [ ] Submit zip
- [ ] Mark solved exercises in OLAT
- [ ] Any implementation MUST NOT produce any additional output
- [ ] If you work in a group, create a `group.txt` file according to the format specified below.

If you worked in a group, the `group.txt` file must be present
and have one line per student which contains the matriculation number
in the beginning, followed by a space and the student's name.
For example, if the group consists of Jane Doe,
who has matriculation number 12345678,
and Max Mustermann, who has matriculation number 87654321,
the `group.txt` file should look like this:

```text
12345678 Jane Doe
87654321 Max Mustermann
```
