# Exercise Sheet 9

This week we continue working with TCP sockets by implementing a simple chat application.

## Task 1

As a first step, implement a basic client/server application, where multiple clients can dynamically connect, disconnect and send messages to the server.
The server then prints the received messages to the console, similar to the "math server" of [task 3 of exercise sheet 4](../exercise04/README.md#task-3).
Your solution should consist of two executables, one for the server, and another for the client.


The server:

- Receives at least 2 arguments, 
  - The first is the `port` it should listen on.
  - The second and following are usernames representing the admins of the chat.
  - You can assume a maximum of 5 different admin usernames.
- The server starts with creating and binding the socket.
- Next, a list of admins is saved and the server spawns a _listener thread_ for accepting incoming connections.
  - The listener thread should terminate once the `/shutdown` command has been received from an admin.
    A good way of implementing this is to use the `pthread_cancel(3)` function.
- For each client that connects, a new _client thread_ is spawned:
  - The server receives the username of the client and sets a flag if the user is an admin.
  - The client thread continuously waits for incoming messages, and upon receiving them, prints them to the console, in the form `"<username>: <message>"`.
- The server waits until the listener thread and all client threads have terminated, cleans up and exits.

The client:

- Receives two command line arguments: The `port` to connect to, as well as the `username` it will be identified by. 
- In this first version, multiple users can have the same username (yes, they could be all admins)
- Upon connecting, the client first sends its username to the server.
- It then continuously prompts messages from the user, and sends them to the server.

Your chat application should support special commands:

- `/shutdown` informs the server to shut down, no longer accepting new connections. In addition, the client should automatically exit. 
- If the client is admin, the server prints `"Server is shutting down."`, upon receiving the message. If some clients are still connected, the server should further print `"Waiting for <N> clients to disconnect."`, where `N` is the number of remaining clients.
  It then waits for all remaining clients to disconnect, cleans up and exits.
- If the client is not an admin, it only cleans up and exits.

**Example output:**

```text
TERMINAL 1                      TERMINAL 2                      TERMINAL 3                          TERMINAL 4 

> ./server 24831 jakob
Listening on port 24831.
                                > ./client 24831 alice
alice connected.
                                                                > ./client 24831 bob
bob connected.
                                                                                                    ./client 24831 jakob
jakob connected. 
                                > hi all
alice: hi all
                                                                > whats up?
bob: whats up?
                                > /shutdown
alice disconnected.
                                                                                                    > she only disconnected
jakob: she only disconnected
                                                                                                    ./shutdown
jakob disconnected.
Server is shutting down. Waiting for 1 clients to disconnected.
                                                                > /shutdown
bob disconnected.
```

**Additional notes & hints:**

- You can assume that every message sent over a socket (including usernames) is exactly `MSG_SIZE = 128` bytes long.
- As always, ensure proper synchronization for all shared data structures.

## Task 2

Now turn your application from a "one-way shoutbox" into a real chat: Instead of printing messages on the server, each message should be broadcasted to all other clients.

Notably, your client now needs to be able to both read user input, as well as receive incoming messages broadcasted by the server.
Use threads to implement this functionality.


**Example output (server omitted):**

```text
TERMINAL 1                      TERMINAL 2                      TERMINAL 3

> ./client 24831 alice
                                > ./client 24831 bob
bob connected.                                                  > ./client 24831 charlie
charlie connected.              charlie connected.
                                > hi all
bob: hi all                                                     bob: hi all
                                                                > whats up?
charlie: whats up?              charlie: whats up?
```

**Additional notes & hints:**

- Again, make sure to properly synchronize access to any shared data.
- Don't overthink your solution, make use of the fact that a single socket can be simultaneously used for reading and writing from within different threads.

## Task 3

Extend your chat application to support a _timeout_ functionality for admins.
By typing `/timeout <username>`, the message user with the `<username>` should be timeouted for indefinitely. 
A timeouted user should not be able to send messages, but should still be able to receive them.
Additionally a timeouted user should receive a message `"You have been timeouted by <admin>."`.

**Example output (server omitted, admin: jakob):**

```text
TERMINAL 1                          TERMINAL 2                    TERMINAL 3
> ./client 24831 alice
                                    > ./client 24831 jakob
jakob connected.                                                  > ./client 24831 charlie
charlie connected.                  charlie connected.
                                    > hi all
jakob: hi all                                                     jakob: hi all
                                    > /timeout alice
You have been timeouted by jakob.
                                    > hi all
jakob: hi all                                                     jakob: hi all
> test
                                                                  > hi you
charlie: hi you                     charlie: hi you   
```

---

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive must not contain binaries.

```text
exc09_csXXXXXX.zip
├── Makefile             # optional
├── group.txt            # optional
├── task1
│   ├── Makefile
│   ├── client.c
│   ├── common.h         # optional
│   └── server.c
├── task2
│   ├── Makefile
│   ├── client.c
│   ├── common.h         # optional
│   └── server.c
└── task3
    ├── Makefile
    ├── client.c
    ├── common.h         # optional
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

