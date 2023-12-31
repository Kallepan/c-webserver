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
#include "router.hh"

#define PORT 8080

int setHeader(std::string &input, int status_code, std::string status_message, int content_length, std::string content_type)
{
    /**
     * @brief Get the header from the request
     * @param input The request
     * @param status_code The status code
     * @param status_message The status message
     * @param content_length The content length
     * @param content_type The content type
     * @return error code
     */

    // clear header
    input = "";

    // set Default header
    // consider std::stringstream if this gets too messy
    input += "HTTP/1.1 " + std::to_string(status_code) + " " + status_message + "\n";
    input += "Content-Type: " + content_type + "\n";
    input += "Content-Length: " + std::to_string(content_length) + "\n\n"; // note the double line feed (RFC 2616 requires this)

    return 0;
}

int addContent(std::string &input, std::string content)
{
    /**
     * @brief Add content to the request
     * @param input The request
     * @param content The content to add
     * @return error code
     */

    input += content;

    return 0;
}

int main()
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // AF_INET: IPv4, SOCK_STREAM: TCP, 0: IP
    {
        throw std::runtime_error("socket failed");
    }

    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Any address
    address.sin_port = htons(PORT);       // Port

    std::fill(std::begin(address.sin_zero), std::end(address.sin_zero), '\0'); // Fill with 0

    // Forcefully attaching socket to the defined port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) // Bind socket to address
    {
        throw std::runtime_error("port bind failed");
    }
    if (listen(server_fd, 10) < 0) // Listen for connections
    {
        throw std::runtime_error("listen");
    }

    // initialize router
    Router router;
    router = Router();

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
        std::cout << buffer << '\n';                           // Print buffer

        // Parse buffer and get GET request
        std::string path;
        router.translateFromBufferToPath(buffer, path);

        // using the path, get the content
        std::ifstream file("res/" + path);
        if (!file)
        {

            // file not found
            std::string message, content = "File not found: " + path + "\n";

            // quick logs :
            std::cout << content;

            setHeader(message, 404, "Not Found", content.length(), "text/plain");
            addContent(message, content);

            // send header and content
            write(new_socket, message.c_str(), message.length());
            close(new_socket);

            continue;
        }

        // get file size
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);

        // read file into buffer
        std::vector<char> buffer_file(length);
        file.read(&buffer_file[0], length);

        // convert buffer to string
        std::string content(buffer_file.begin(), buffer_file.end());

        // set header
        std::string header;
        setHeader(header, 200, "OK", content.length(), "text/html");
        addContent(header, content);

        // send header and content
        write(new_socket, header.c_str(), header.length());

        std::cout << "------------------ Message sent -------------------\n";
        close(new_socket); // Close connection
    }

    return 0;
}