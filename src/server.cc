// Server side Cpp program to demonstrate Socket programming

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>
#include <vector>

#define PORT 8080

int main()
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // AF_INET: IPv4, SOCK_STREAM: TCP, 0: IP
    {
        throw std::runtime_error("socket failed");
    }

    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Any address
    address.sin_port = htons(PORT);       // Port

    std::fill(std::begin(address.sin_zero), std::end(address.sin_zero), '\0'); // Fill with 0

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) // Bind socket to address
    {
        throw std::runtime_error("bind failed");
    }
    if (listen(server_fd, 10) < 0) // Listen for connections
    {
        throw std::runtime_error("listen");
    }
    while (true)
    {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) // Accept connection
        {
            throw std::runtime_error("accept");
        }

        std::string buffer(30000, '\0');                       // Buffer for message
        valread = read(new_socket, &buffer[0], buffer.size()); // Read message into buffer
        buffer.resize(valread);                                // Resize buffer to message size
        std::cout << buffer << '\n';                           // Print message
        write(new_socket, hello.c_str(), hello.length());      // Send message
        std::cout << "------------------Hello message sent-------------------\n";
        close(new_socket); // Close connection
    }

    return 0;
}