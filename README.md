# Mini Terminal Server

This project is a simple non-blocking TCP server implemented in C using epoll for efficient I/O multiplexing. It demonstrates how to handle multiple client connections concurrently without blocking the main thread.

## Features

-   Handles multiple clients using epoll
-   Non-blocking sockets
-   Prints client messages to the server console
-   Responds to each client with a static message

## How It Works

-   The server listens on port 1234 (default)
-   When a client connects, the server accepts the connection and adds it to the epoll event loop
-   For each client message, the server prints the message and replies with "Hello Client!"
-   If a client disconnects, the server cleans up the connection

## Build

To compile the server, run:

```bash
gcc sock.c -o a.out
```

## Run

To start the server:

```bash
./a.out
```

The server will print a message when it starts and whenever a client connects, sends a message, or disconnects.

## Test with netcat

You can test the server using `nc` (netcat):

```bash
nc localhost 1234
```

Type a message and press Enter. The server will respond with "Hello Client!".

## Files

-   `sock.c`: Main server source code
-   `.gitignore`: Ignores the compiled binary `a.out`

## Requirements

-   Linux OS
-   GCC compiler

## License

This project is for educational purposes.
