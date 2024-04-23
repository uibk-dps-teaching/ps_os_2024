# UIBK Operating Systems Lab 2024

This repository contains material required to complete exercises for the OS lab in the 2024 summer semester, including exercise sheets and associated resources.

**Make sure to read this document carefully, as it contains vital information regarding the grading of exercises.**

## Schedule

This schedule gives you an overview of the topics we will discuss on each date. It will be updated continuously throughout the semester.
Note that exercises associated with a given date are to be discussed on that day, i.e., solutions for that exercise have to be handed in via OLAT by 17:00 on the previous day (Tuesdays).

| Date       | Exc              | Topic                                                 | Notes               |
| ---------- | ---------------- | -------------------------------------                 | ------------------- |
| 2024-03-06 | -                |                                                       | C recap slides      |
| 2024-03-13 | [1](exercise01/) | Self-assessment, ssh, commandline aruments            | Makefile slides     |
| 2024-03-20 | [2](exercise02/) | Makefile, signal, fork                                |                     |
| 2024-03-27 | -                |                                                       | Break               |
| 2024-04-03 | -                |                                                       | Break               |
| 2024-04-10 | [3](exercise03/) | Permissions, exit codes, pthreads                     |                     |
| 2024-04-17 | [4](exercise04/) | Valgrind, pipes, fifos                                |                     |
| 2024-04-24 | [5](exercise05/) | Message queues, shared memory, synchronization        |                     |
| 2024-05-08 | [6](exercise06/) | Atomics, mutex, condition variables                   |                     |

## Coding Guidelines

All programming exercises are conducted in C.

Your code should always compile without warnings, when passing the flags `-Wall -Wextra -std=c11`.

Make sure your code is properly formatted using either your IDE/Text editor of choice, or by using a tool such as clang-format.
You can find an example [`.clang-format`](.clang-format) file in this repository.
Failure to consistently format code may result in lower scores.

Make sure to choose descriptive variable and function names.
**All names and comments must be written in English.**

## Working From Home with ZID-GPL

All programming exercise solutions you hand in **must** compile properly on the [ZID-GPL server](https://www.uibk.ac.at/zid/systeme/linux/lpccs_4/benutzeranleitung_zid-gpl.html).
The easiest way of ensuring this is by developing on ZID-GPL directly, either through a terminal or using an editor/IDE with support for remote development via SSH (such as [Visual Studio Code](https://code.visualstudio.com/docs/remote/ssh)).

**Important**: Facing issues with your system is not a valid excuse for only partially completing exercise sheets or avoiding them entirely. If you encounter difficulties, use ZID-GPL.

## Connecting to ZID-GPL via SSH

To log into ZID-GPL via SSH, use the following command on Linux/Mac OSX:

`ssh -l <username> zid-gpl.uibk.ac.at`

On Windows, you can use Powershell (Windows 10) or a third-party SSH client
such as [PuTTY](https://www.putty.org/).
