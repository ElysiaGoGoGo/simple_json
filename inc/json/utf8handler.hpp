//位
#ifndef UTF8HANDLER_HPP
#define UTF8HANDLER_HPP
#pragma once
#include <string>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <tuple>

class UTF8Adaptor
{
    std::string &raw_str;
    std::vector<u_int32_t> decoded_str;
protected:
    
  static std::tuple <size_t,u_int32_t> decode_one_char(std::string_view::const_iterator it) ;
   static std::tuple <size_t,u_int32_t> decode_one_char(std::string_view str, size_t pos) 
    {
        return decode_one_char(str.begin() + pos);
    }
public:
    static bool IsValidUTF8(std::string_view str);
    static std::string encode(std::vector<u_int32_t> vec);
    static std::vector<u_int32_t> decode(std::string_view str);

    UTF8Adaptor(std::string &str) : raw_str(str)
    {
        if (!IsValidUTF8(raw_str))
        {
            throw std::runtime_error("Invalid UTF-8 string");
        }
        decoded_str = decode(str);
    }
    const std::vector<u_int32_t> &get_decoded_str() const
    {
        return decoded_str;
    }
    std::string_view get_utf8_str() const
    {
        return raw_str;
    }
};
#endif // UTF8HANDLER_HPP