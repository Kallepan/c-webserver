#include "router.hh"

Router::Router(){};

Router::~Router(){};

int Router::translateFromBufferToPath(std::string &buffer, std::string &path)
{
    std::size_t found = buffer.find("GET");

    if (found == std::string::npos)
    {
        std::cout << "No GET request found\n";
        return 1;
    }

    found = buffer.find("HTTP/1.1");
    if (found == std::string::npos)
    {
        std::cout << "No end of GET request found\n";
        return 1;
    }

    path = buffer.substr(4, found - 5);

    this->cleanPath(path);

    return 0;
};

int Router::cleanPath(std::string &path)
{
    if (path.find("..") != std::string::npos)
    {
        std::cout << "Directory traversal detected\n";
        return 1;
    }

    if (path.find("//") != std::string::npos)
    {
        std::cout << "Directory traversal detected\n";
        return 1;
    }

    if (path.find("~") != std::string::npos)
    {
        std::cout << "Directory traversal detected\n";
        return 1;
    }

    if (path == "/" || path == "")
    {
        path = "index.html";
        return 0;
    }

    return 0;
};