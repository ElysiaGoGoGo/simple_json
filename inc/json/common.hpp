#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

// Define DEBUG macro for debug output
// Comment out or undef to disable debug output in release builds
//#define DEBUG

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
inline bool is_end_char(uint32_t c) {return c==',' || c=='}' || c==']' ;}

/**
 * @brief 检查字符是否为空白字符
 * Check if character is a whitespace character
 * @param c 要检查的字符 The character to check
 * @return 如果是空白字符返回true，否则返回false Returns true if it's a whitespace character, false otherwise
 */
inline bool is_whitespace(uint32_t c) {return c==' ' || c=='\t' || c=='\n' || c=='\r';}

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
class LoopGuard
{
    int counter=0;
    constexpr static int max_counter=10000;
public:
    /**
     * @brief 调用操作符，增加计数器并检查是否超过最大限制
     * Call operator, increments counter and checks if exceeds maximum limit
     */
    void operator()(){
        ++counter;
        if(counter>max_counter)
        {
            throw std::runtime_error("LoopGuard: too many loops");
        }
    }
};

/**
 * @brief 行错误类，提供详细的错误信息和上下文
 * Line error class providing detailed error information and context
 */
class LineError: std::runtime_error
{
    UnicodeStringViewIterator iter;
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
    LineError(const std::string& ms,UnicodeStringViewIterator iter):std::runtime_error(ms),iter(iter),msg(ms){
        auto forward_iter=iter,back_iter=iter;
        LoopGuard guard1,guard2;
        while(true)
        {
            guard1();
            --forward_iter;
            if(*forward_iter=='\n')//dangerous
            {
                break;
            }
        }
        while(true)
        {
            guard2();
            ++back_iter;
            if(*back_iter=='\n')
            {
                break;
            }
        }
        msg+="\n cuurent condition:\n before:\n"+std::string(forward_iter,iter)+"\nafter:\n" +string(iter,back_iter)+'\n';
    }
};


#endif // COMMON_HPP
