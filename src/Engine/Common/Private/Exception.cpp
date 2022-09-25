#include "Engine/Common/Exception.hpp"

#include "Engine/Utils/String.hpp"

SHV::Exception::Exception(std::string aMessage) : message(aMessage) {}

const char* SHV::Exception::what() const noexcept { return message.data(); }
