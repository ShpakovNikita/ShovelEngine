#pragma once

#include <csignal>
#include <cassert>
#include <map>
#include <string>
#include <iostream>

#define SHV_S1(x) #x
#define SHV_S2(x) SHV_S1(x)
#define SHV_LOCATION __FILE__ " : " SHV_S2(__LINE__)

static std::map<std::string, int> assertInvocationData = {};

// Returns if it's the first time
inline bool _assertInvoked(const std::string& assert) {
    auto it = assertInvocationData.find(assert);
    if (it != assertInvocationData.end()) {
        ++it->second;
        return false;
    }

    assertInvocationData[assert] = 1;
    return true;
}

#ifdef NDEBUG
#define AssertD(expression)                                               \
    {                                                                     \
        if (!(expression) && _assertInvoked(std::string(SHV_LOCATION))) { \
            std::cerr << "Assertion failed: " << #expression << ", file " \
                      << __FILE__ << ", line " << __LINE__ << std::endl;  \
        }                                                                 \
    }
#else
#define AssertD(expression)                                               \
    {                                                                     \
        if (!(expression) && _assertInvoked(std::string(SHV_LOCATION))) { \
            std::raise(SIGINT);                                           \
        }                                                                 \
    }
#endif

#define AssertE assert