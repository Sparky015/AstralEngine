//
// Created by Andrew Fagan on 4/14/24.
//

#pragma once

#include "Ayla/aypch.h"
#ifdef AYLA_PLATFORM_WINDOWS
    #ifdef AYLA_BUILD_SHARED_LIB
        #define AYLA_API __declspec(dllexport)        // Export
    #else
        #define AYLA_API __declspec(dllimport)        // Import
    #endif
#elif AYLA_PLATFORM_APPLE
    #ifdef AYLA_BUILD_SHARED_LIB
        #define AYLA_API __attribute__((visibility("default")))       // Export
    #else
        #define AYLA_API        // Import
    #endif
#else
    #error Something went wrong with defining Ayla API symbols (Platform not found)
#endif



namespace {
    void macro_AY_TRACE(std::string&& title);
    void macro_AY_ASSERT(bool expression, std::string&& errorMessage);
    void macro_AY_ASSERT_SS(bool expression, std::ostream& errorMessage);
    void macro_AY_LOG(std::string&& message);
    void macro_AY_ERROR(std::string&& message);

    class AY_PROFILER {
    public:
        explicit AY_PROFILER(std::string&& title = "Profiler");
        ~AY_PROFILER();
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_startTime;
        std::chrono::time_point<std::chrono::steady_clock> m_endTime;
        std::string m_title;
    };

}

#define AY_TRACE(title) macro_AY_TRACE(title)
#define AY_ASSERT(expression, errorMessage) macro_AY_ASSERT(expression, errorMessage)
#define AY_ASSERT_SS(expression, errorMessage) { std::stringstream ss; macro_AY_ASSERT_SS(expression, ss << errorMessage); } // Stringstream alternative assert, so you can use an ostream for the debug message
#define AY_LOG(message) macro_AY_LOG(message)
#define AY_PROFILE_SCOPE(title) AY_PROFILER(title)
#define AY_ERROR(errorMessage) macro_AY_ERROR(errorMessage)

#include <iomanip>
namespace {

    void macro_AY_TRACE(std::string&& title){
        std::time_t time;
        std::time(&time);

        std::cout << "\n" << title; //<< std::setw(40) << "\t : " << std::ctime(&time);
    }

    void macro_AY_ASSERT(bool expression, std::string&& errorMessage){
        if (!expression){
            throw std::runtime_error("\n\n" + errorMessage);
        }
    }

    void macro_AY_ASSERT_SS(bool expression, std::ostream& errorMessage){
        if (!expression){
            std::ostringstream oss;
            oss << "\n\n" << errorMessage.rdbuf();
            throw std::runtime_error(oss.str());
        }
    }

    void macro_AY_LOG(std::string&& message){
        std::cout << "\n" << message;
    }

    AY_PROFILER::AY_PROFILER(std::string&& title) {
        m_title = title;
        //m_startTime = std::chrono::high_resolution_clock::now();
    }

    AY_PROFILER::~AY_PROFILER() {
        //m_endTime = std::chrono::high_resolution_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime);
        //std::cout << "\n" << m_title << " time: " << duration.count() << " microseconds\n" << std::endl;
    }

    void macro_AY_ERROR(std::string&& errorMessage){
        throw std::runtime_error("\n\n" + errorMessage);
    }

}