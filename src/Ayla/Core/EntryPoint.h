//
// Created by sparky on 4/16/2024.
//

#pragma once
#include "Ayla/Core/Application.h"



/**
 * @file main.cpp
 * @author sparky
 * @date 16 April 2024
 * @brief Main initialization for the application.
 */

#include "Ayla/Core/Application.h"

/**
 * @brief Main function.
 *
 * This is the main entry point of the application.
 * It creates an application, runs it, and then deletes the application.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Status code. A non-zero status code indicates an error.
 */
int main(int argc, char *argv[]) {
    //! Create the application
    auto client_app = Ayla::CreateApplication();

    //! Run the application
    client_app->Run();

    //! Delete the application
    delete client_app;

    //! If everything has run smoothly, return 0
    return 0;
}