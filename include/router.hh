// Router header file
#pragma once

#ifndef ROUTER_HH
#define ROUTER_HH
#include <string>
#include <iostream>

class Router
{
public:
    /**
     * @brief Constructor
     *
     * This constructor initializes the router with the given port.
     *
     * @param port The port to listen on
     */
    Router();

    /**
     * @brief Destructor
     *
     * This destructor destroys the router.
     */
    ~Router();

    /**
     * @brief Translate the given path
     *
     * This function translates the given path to the correct file.
     */
    int translateFromBufferToPath(std::string &buffer, std::string &path);

private:
    int cleanPath(std::string &path);
};

#endif // ROUTER_HH