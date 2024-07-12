//
// Created by Andrew Fagan on 7/12/24.
//

#include "Ayla/Core/DebugMacros.h"

#include <iomanip>

namespace {

    void macro_AY_TRACE(const std::string&& title)
    {
        std::time_t time;
        std::time(&time);

        // TODO: Figure out a way to make the time appear nice and pretty
        std::cout << "\n" << title; //<< std::setw(40) << "\t : " << std::ctime(&time);
    }


    void macro_AY_ASSERT(const bool expression, const std::string&& errorMessage)
    {
        if (!expression){
            throw std::runtime_error("\n\n" + errorMessage);
        }
    }


    void macro_AY_ASSERT_SS(const bool expression, const std::ostream& errorMessage)
    {
        if (!expression){
            std::ostringstream oss;
            oss << "\n\n" << errorMessage.rdbuf();
            throw std::runtime_error(oss.str());
        }
    }


    void macro_AY_LOG(const std::string&& message)
    {
        std::cout << "\n" << message;
    }


    AY_PROFILER::AY_PROFILER(std::string&& title)
    {
        m_title = title;
        //m_startTime = std::chrono::high_resolution_clock::now();
    }


    AY_PROFILER::~AY_PROFILER()
    {
        //m_endTime = std::chrono::high_resolution_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime);
        //std::cout << "\n" << m_title << " time: " << duration.count() << " microseconds\n" << std::endl;
    }


    void macro_AY_ERROR(const std::string&& errorMessage)
    {
        throw std::runtime_error("\n\n" + errorMessage);
    }


    void macro_AY_ERROR_SS(const std::ostream& errorMessage)
    {
        std::ostringstream oss;
        oss << "\n\n" << errorMessage.rdbuf();
        throw std::runtime_error(oss.str());
    }

}