//
// Created by Andrew Fagan on 7/12/24.
//

#include "Ayla/Core/DebugMacros.h"


namespace Ayla::Core::Macros
{

    void macro_AY_TRACE(const std::string&& title)
    {
        /** Gets the current time */
        const std::time_t t = std::time(nullptr);
        const std::tm* currentTime = std::localtime(&t); // Puts the information of the time into a struct that is separated into min, sec, hr, day, etc.

        /** Calculates the elapsed milliseconds as they were not included in the std::tm struct */
        long long millisecondsSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        long long elapsedMilliseconds = millisecondsSinceEpoch % 1000;

        /** Outputs the message with the time info prefixing it */
        std::cout << "\n[" << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << ":" << elapsedMilliseconds << "] " << title;
    }


    void macro_AY_ASSERT(const bool expression, const std::ostream& errorMessage)
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


    void macro_AY_ERROR(const std::ostream& errorMessage)
    {
        std::ostringstream oss;
        oss << "\n\n" << errorMessage.rdbuf();
        throw std::runtime_error(oss.str());
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

} // namespace Ayla::Core::Macros