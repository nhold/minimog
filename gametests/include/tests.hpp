#pragma once

#include <iostream>

#define TESTMSG(message) TestMessage(message, __func__, __FILE__)

namespace minimog
{
    template<typename T>
    bool IsEqual(const T value, const T expected, const char* name)
    {
        bool result = value == expected;
        auto resultString = result ? "Success! :-)" : "Failed :-(.";
        std::cout << "Expecting " << name << " to be '" << expected << "' and the actual value is '" << value << "'.  |  " << resultString << std::endl;

        return result;
    }

    inline void TestMessage(const char* message, const char* function, const char* file)
    {
        std::cout << "===============================================" << std::endl;
        std::cout << message << ": " << function << " in " << file << std::endl;
        std::cout << "===============================================" << std::endl;
    }
}