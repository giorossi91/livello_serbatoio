#include "String.h"


String::String()
{

}

String::String(const std::string &str)
{
    internalString = str;
}

String::String(char c)
{
    internalString = c;
}

String::String(const char *c_arr)
{
    internalString = std::string(c_arr);
}

String::String(uint8_t n)
{
    internalString = std::to_string(n);
}

String::String(int16_t n)
{
    internalString = std::to_string(n);
}

String::String(uint16_t n)
{
    internalString = std::to_string(n);
}

String::String(int32_t n)
{
    internalString = std::to_string(n);
}

String::String(uint32_t n)
{
    internalString = std::to_string(n);
}

String::String(double n)
{
    internalString = std::to_string(n);
}

int32_t String::toInt() const
{
    return std::stoi(internalString);
}

const char *String::c_str() const
{
    return internalString.c_str();
}

std::string String::getString()
{
    return internalString;
}
