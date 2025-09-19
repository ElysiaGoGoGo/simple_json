#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

// Define DEBUG macro for debug output
// Comment out or undef to disable debug output in release builds
#define DEBUG
#undef DEBUG
#ifdef DEBUG
inline void debug_print(JsonStringViewIterator iter)
{
    auto forward=iter,backward=iter;
    while(*forward!='\n')
    {
        forward++;
    }
    while(*backward!='\n')
    {
        backward--;
    }
    std::cout<<"debug print:\n\ncurrent case:\n"<<std::string(backward+1,forward)<<"\n iter points to the"<<iter-backward<<" value:"<<(char)(*iter)<<'\n\n\n';
}
#endif

#include <stdexcept>
#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <span>
#include <variant>
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <string_view>
#include <tuple>
#include <ranges>
#include <span>
#include <iostream>
#include <simple_json.hpp>

/**
 * @brief 检查字符是否为JSON结束字符
 * Check if character is a JSON end character
 * @param c 要检查的字符 The character to check
 * @return 如果是结束字符返回true，否则返回false Returns true if it's an end character, false otherwise
 */
inline bool is_end_char(char c) {return c==',' || c=='}' || c==']' ;}

/**
 * @brief 检查字符是否为空白字符
 * Check if character is a whitespace character
 * @param c 要检查的字符 The character to check
 * @return 如果是空白字符返回true，否则返回false Returns true if it's a whitespace character, false otherwise
 */
inline bool is_whitespace(char c) {return c==' ' || c=='\t' || c=='\n' || c=='\r';}

using std::string;

/**
 * @brief 文件保护类（预留）
 * File guard class (reserved)
 */
class FileGuard
{


};

/**
 * @brief 循环保护类，防止无限循环
 * Loop guard class to prevent infinite loops
 */


/**
 * @brief 行错误类，提供详细的错误信息和上下文
 * Line error class providing detailed error information and context
 */
class LineError: std::runtime_error
{
    JsonStringViewIterator iter;
    string msg;
public:
    /**
     * @brief 获取错误信息
     * Get error message
     * @return 错误信息字符串指针 Pointer to error message string
     */
    const char * what() const noexcept override
    {
        return msg.c_str();
    }
    
    /**
     * @brief 构造函数，创建带有上下文信息的行错误
     * Constructor, creates line error with context information
     * @param ms 错误消息 Error message
     * @param iter 字符串迭代器位置 String iterator position
     */
    LineError(const std::string& ms,JsonStringViewIterator iter):std::runtime_error(ms),iter(iter),msg(ms){


    }
};


#endif // COMMON_HPP
