# Exercise Sheet 8

## Task 1

In this task we will implement a simple "donation server" using TCP sockets.

Create a program called `server`, that receives as its single argument a TCP port to listen on.
After startup, the server opens a TCP socket (see below), prints `"Listening on port <port>."` and continuously listens for incoming connections.
The server is a so called donation server, which expects a single message from the client in the format `<number>`, where `<number>` is a positive `double`.
Upon a client connecting, the server attempts to read incoming messages from the client and responds (using the established socket) with `"Donation: <sum_of_donations>"`, either until the client disconnects or the shutdown command (see below) is received.
The server must not print the clients message on the command line.

For this task, you don't need to create a separate client application.
Instead, you can use the `netcat` (`nc`) utility to establish a TCP connection to your server.
The command `nc localhost <port>` will open a prompt where you can send messages to your server and see its response.
You can exit `netcat` by pressing `CTRL+C`.

The server should be able to handle multiple clients connecting, but only one at a time (i.e., the current connection has to be closed before a new one can be accepted). Using multiple threads or processes is therefore not required.

Upon receiving the message `/shutdown`, the server should print `"Shutting down."`, stop accepting new connections, clean up and exit.

Begin by skimming through `man 7 ip` and `man 7 socket`. Here is a quick TLDR:

1. To create a TCP/IP socket, use the `socket()` function with `domain = PF_INET` and `type = SOCK_STREAM`.
2. Next, fill out the `sockaddr_in` struct to inform the socket where to bind to, like so:

   ```c
   const struct sockaddr_in addr = {
     .sin_family = AF_INET,
     .sin_addr {
       .s_addr = htonl(INADDR_ANY),  
     },
     .sin_port = htons(port),
   };
   ```

3. Next, `bind()` the socket to the address, and `listen()` for incoming connections.
4. Use `accept()` to accept incoming connections, and to receive a unique communication socket for each client.

**Additional notes and hints:**

- The `dprintf` function may be useful for writing to a socket.
- Make sure to properly `close()` all sockets before exiting.
- `bind()` returning `EADDRINUSE` ("Address already in use"), may indicate that you did not properly close a socket.
  You can use a different port or find a way to circumvent this for testing.
  Ultimately you should however try to find the root cause of the problem.
- You **must not** use any global variables.
  However, you are allowed to `#define` preprocessor constants.
- Use `-D_DEFAULT_SOURCE` to compile on ZID-GPL.

Example output of the server:

```text
Listening on port 1169.
Established connection!
Shutting down.
Closing connection ...
Connection closed.
```

Example output of `nc`:

```text
$ nc localhost 1169
10
Donations: 10.00
10.20
Donations: 20.20
0.10
Donations: 20.30
ffff 
ffff is not a valid amount
^C
```

Answer the following questions:

- Why is it important to use network byte order with TCP/IP?
- The example uses `INADDR_ANY`. Could we also use `INADDR_LOOPBACK`?
- The example uses `SOCK_STREAM`. Name two other types of sockets and briefly explain their difference compared to `SOCK_STREAM`.
- What is the range of possible ports that we assign to `addr.sin_port`.
- Why is it a good idea to use a port greater or equal to 1024?

## Task 2

In this task, we will write a client application for the "donation server" from the previous task. This application has to have the following behavior (similar to the previous use of `nc`):

- start it with `./client <port>`
- continuously prompt for input which is sent to the server (without any checks for input validity)
- when `/quit` is entered or `CTRL+C` is pressed: clean up and exit
- when `/shutdown` is entered: send a shutdown signal to the server, clean up, and then exit
- print the messages received from the server: after connecting to the server, spawn a *listener thread* that implements this behavior; terminate it by using `pthread_cancel(3)` as part of your clean up code

To initialize a TCP socket on the client, follow the same steps to create a socket and initialize the `sockaddr_in` structure, however this time setting

```c
addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // on client
```

Then, use the `connect` function to establish a connection to the server through your socket.

After your client is functional, extend your server by error checking mechanisms that check all messages received from the client for validity. All donations must be non-negative floating point numbers (see `strtod(3)`) and sensible (e.g. the fractional part, if given, should contain either 1 or 2 decimal places to represent a multiple of 1 cent). If desired, you may use a comma as a thousands separator (e.g. `100,000.00`).

**Hint:** You can assume that every message sent over a socket is exactly `MSG_SIZE = 1024` bytes long. Ensure proper null-termination of your strings.

## Task 3

In this task we will turn the simple _donation server_ from the previous task into a simple _HTTP web server_.

- The server receives two arguments:
    - `port` - the port it should listen on
    - `N` - the number of _request handlers_
- The server creates a queue for client connections. You may use the queue implementation from exercise sheet 06.
- The server spawns `N` _request handler_ threads containing a loop:
  - Pop a client connection from the queue.
    - If the popped connection is a _poison value_ (e.g. `-1`), return.
    - Otherwise, receive and “parse” the HTTP request. You only need the first line of the request for this (see below).
  - To simulate a real workload, sleep for 100 milliseconds.
  - Send a response to the client:
    - When a `GET /` request is received, sends a response with a small HTML response body of your own choosing.
      Be sure to set the correct size in the `Content-Length` header field. (see below)
    - When a `POST /donate` request is received, the server incements a double variable by the donation amount received 
      in the POST body. Subsequently, it returns a message acknowledging the donation and stating the current balance 
      (e.g. `Thank you very much for donating $13.500000! The balance is now $49.500000.`).
    - When a `POST /shutdown` request is received, the _listener_ thread is terminated.
      Since `accept` is blocking, one way of implementing this is to use the `pthread_cancel(3)` function.
    - Otherwise, you can either send the same response as for `GET /` or a custom error response (e.g. with HTTP status code `501 Not Implemented`).
  - Close the connection.
- The server creates and binds the socket as in the previous task, and then spawns a _listener_ thread for accepting incoming connections:
  - Each accepted connection is added to the client connection queue.
- The server waits for the _listener_ thread to finish.
- The server pushes `N` _poison values_ into the queue.
- The server waits for all _request handler_ threads to finish, cleans up and exits.

**Alternative implementation:** Instead of using a queue and `N` _request handler_ threads that handle requests in a loop, you can use your thread pool (with size `N`) from the previous exercise sheet. The _listener_ thread then submits a _request handler_ job to the thread pool for every client connection. You will need to add a variant of `pool_submit` which does not return a `job_id` since there is no good place to call `pool_await`.

If everything works correctly, you should be able to navigate your browser to `http://localhost:<port>/` and see your web server's response.
To shut it down, you can use `curl -X POST http://localhost:<port>/shutdown`.

### The HTTP Protocol

To properly communicate with a web browser, the web server needs to be able to “speak” the _HTTP protocol_, or at least a very simple subset thereof.
Most HTTP messages (and all that we care about here) consist of at most two parts, a _header section_ and optionally a _response body_.
The _header section_ contains multiple _header fields_ with meta-information about the current request or response.
Importantly, the HTTP protocol uses CRLF (`\r\n`) for line endings instead of just LF (`\n`).
A final empty newline marks the end of the header section.

When sending a `GET` request by opening `http://localhost:<port>/` in a browser or using `curl http://localhost:<port>/`, the server will receive a request that looks something like this:

```http
GET / HTTP/1.1\r\n
Host: localhost:<port>\r\n
Accept: */*\r\n
\r\n
```

When sending a `POST` using `curl -X POST http://localhost:<port>/shutdown`, the server receives a request like

```http
POST /shutdown HTTP/1.1\r\n
Host: localhost:<port>\r\n
Accept: */*\r\n
\r\n
```

To respond to a client, the server needs to write a header section and a response body, which typically looks like

```http
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
Content-Length: <number of bytes in response body, including newlines>\r\n
\r\n
<response body>
```

Example output:

```text
┌──────────────────────────────────┬────────────────────────────────────────────────────┬───────────────────────────────┐
│ TERMINAL 1                       │ TERMINAL 2                                         │ TERMINAL 3                    │
├──────────────────────────────────┼────────────────────────────────────────────────────┼───────────────────────────────┤
│ $ ./server 8888 5                │                                                    │                               │
│ Listening on port 8888.          │                                                    │                               │
│                                  │ $ curl http://localhost:8888/                      │ $ curl http://localhost:8888/ │
│                                  │ <response body>                                    │ <response body>               │
│                                  │                                                    │                               │
│                                  │                                                    │                               │
│                                  │ $ curl -X POST http://localhost:8888/donate -d 2.1 │                               │
│                                  │ <response body>                                    │                               │
│                                  │ $ curl -X POST http://localhost:8888/shutdown      │ $ curl http://localhost:8888/ │
│ Shutting down.                   │                                                    │  <response body>              │
│                                  │                                                    │                               │
└──────────────────────────────────┴────────────────────────────────────────────────────┴───────────────────────────────┘
```

**Additional notes and hints:**

- Use `curl -v http://localhost:<port>/` to see the request and response headers sent to and from your server.
  `curl` will complain if the response is malformed.
- For more information on the HTTP protocol, you may browse through [RFC 7230](https://datatracker.ietf.org/doc/html/rfc7230).
- When working on ZID-GPL, you can still connect to your web server with your local web browser by creating an _SSH tunnel_ using `ssh csXXXX@zid-gpl.uibk.ac.at -N -L <port>:localhost:<port>` where `<port>` refers to the port your server is listening on (make sure to choose a unique port to avoid collisions with other students).
- Use `-D_DEFAULT_SOURCE` on ZID.
- Hint: If you are implementing to ZID-GPL, you can use `ssh -L <port>:localhost:<port> cxxxxx@zid-gpl.uibk.ac.at` to establish an SSH tunnel and check results in your browser.

---

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries.

```text
exc08_csXXXXXX.zip
├── Makefile             # optional
├── group.txt            # optional
├── task1
│   ├── Makefile
│   ├── server.c
│   └── solution.txt
├── task2
│   ├── Makefile
│   ├── client.c
│   └── server.c         # extended by error checking
└── task3
    ├── Makefile
    ├── client_queue.h   # optional if using thread pool
    ├── server.c
    ├── thread_pool.c    # optional if using client queue
    └── thread_pool.h    # optional if using client queue
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

