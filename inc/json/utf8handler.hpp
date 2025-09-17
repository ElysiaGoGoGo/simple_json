//位
#ifndef UTF8HANDLER_HPP
#define UTF8HANDLER_HPP
#pragma once
#include <string>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <tuple>
#include <span>
/**
 * @brief UTF-8适配器类，用于处理UTF-8字符串的编码和解码
 * UTF-8 adaptor class for handling UTF-8 string encoding and decoding
 */
class UTF8Adaptor
{
    std::string &raw_str;
    std::vector<u_int32_t> decoded_str;
protected:
    
    /**
     * @brief 解码单个UTF-8字符（迭代器版本）
     * Decode single UTF-8 character (iterator version)
     * @param it 字符串迭代器 String iterator
     * @return 包含字符长度和Unicode码点的元组 Tuple containing character length and Unicode code point
     */
    static std::tuple <size_t,u_int32_t> decode_one_char(std::string_view::const_iterator it) ;
    
    /**
     * @brief 解码单个UTF-8字符（字符串和位置版本）
     * Decode single UTF-8 character (string and position version)
     * @param str 字符串视图 String view
     * @param pos 字符位置 Character position
     * @return 包含字符长度和Unicode码点的元组 Tuple containing character length and Unicode code point
     */
    static std::tuple <size_t,u_int32_t> decode_one_char(std::string_view str, size_t pos) 
    {
        return decode_one_char(str.begin() + pos);
    }
public:
    /**
     * @brief 检查字符串是否为有效的UTF-8编码
     * Check if string is valid UTF-8 encoding
     * @param str 要检查的字符串 String to check
     * @return 如果有效返回true，否则返回false Returns true if valid, false otherwise
     */
    static bool IsValidUTF8(std::string_view str);
    
    /**
     * @brief 将Unicode码点向量编码为UTF-8字符串
     * Encode Unicode code points vector to UTF-8 string
     * @param vec Unicode码点向量 Unicode code points vector
     * @return 编码后的UTF-8字符串 Encoded UTF-8 string
     */
    static std::string encode(const std::vector<u_int32_t> & vec);
    static std::string encode(std::span<const u_int32_t> vec);
    
    /**
     * @brief 将UTF-8字符串解码为Unicode码点向量
     * Decode UTF-8 string to Unicode code points vector
     * @param str UTF-8字符串 UTF-8 string
     * @return 解码后的Unicode码点向量 Decoded Unicode code points vector
     */
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
