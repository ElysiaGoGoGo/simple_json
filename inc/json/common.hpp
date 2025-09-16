#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP
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
inline bool is_end_char(u_int32_t c) {return c==',' || c=='}' || c==']' ;}
inline   bool is_whitespace(u_int32_t c) {return c==' ' || c=='\t' || c=='\n' || c=='\r';}

using std::string;
class FileGuard
{


};
class LoopGuard
{
    
    int counter=0;
    constexpr static int max_counter=10000;
    public:
   void operator()(){++counter;
    if(counter>max_counter)
    {
        throw std::runtime_error("LoopGuard: too many loops");
    }
}
};
class LineError: std::runtime_error
{
    UnicodeStringViewIterator iter;
    string msg;
    public:
    const char * what() const noexcept override
    {
        return msg.c_str();
    }
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