# HTTP Web Server

This project is a simple HTTP web server implemented in C++. It's a great starting point for understanding the basics of socket programming and HTTP protocol.

## Features

- The server listens on port 8080 and accepts incoming connections.
- It supports GET requests and serves static files from the local filesystem.
- If a requested file is not found, it returns a 404 error page.
- The server uses multithreading to handle multiple connections concurrently.

## How to run

To compile and run the server, you need a C++ compiler that supports C++23. You can compile the server with the following command:

```bash
/usr/bin/g++ -fdiagnostics-color=always -std=c++23 -pthread -g src/main.cc src/router.cc -o build/main
```

Then, you can start the server with:

```bash
./build/main
```

The server will start listening on port 8080. You can test it by opening a web browser and navigating to [localhost](http://localhost:8080).

## Limitations

This is a simple server intended for educational purposes, and it has many limitations compared to a real-world web server:

- It only supports GET requests.
- It does not support HTTP/2 or HTTPS.
- It does not have any security features, such as rate limiting or request filtering.
- It does not support dynamic content or server-side scripting.

Despite these limitations, this server can be a useful tool for learning about HTTP and network programming in C++.
