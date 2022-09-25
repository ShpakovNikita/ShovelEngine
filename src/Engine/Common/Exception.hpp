#pragma once

#include <exception>
#include <string>

#include "Engine/Utils/String.hpp"

namespace SHV {

class Exception : public std::exception {
public:
    Exception(std::string message);
    
    template<typename... Args>
    Exception(const std::string& format, Args... args): message(SHV::Format(format, args...)) {}
    
    virtual const char * what () const noexcept;
    
private:
    std::string message;
};
}
