#ifndef STRING_H
#define STRING_H

#include <string>

class String
{
public:
    String();
    String(const std::string& str);
    String(char c);
    String(const char *c_arr);
    String(uint8_t n);
    String(int16_t n);
    String(uint16_t n);
    String(int32_t n);
    String(uint32_t n);
    String(double n);

    int32_t toInt() const;

    const char *c_str() const;

    std::string getString();

private:
    std::string internalString;
};

#endif // STRING_H
