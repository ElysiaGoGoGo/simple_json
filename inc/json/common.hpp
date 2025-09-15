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
inline bool is_whitespace(u_int32_t c) {return c==' ' || c=='\t' || c=='\n' || c=='\r';}

using std::string;
class FileGuard
{


};
class LineError: std::runtime_error
{
    UnicodeStringView where;
    string msg;
    public:
    const char * what() const noexcept override
    {
        return msg.c_str();
    }
    LineError(const std::string& ms,UnicodeStringView where):std::runtime_error(ms),msg(ms),where(where){

        auto forward_iter=where.begin(),back_iter=where.begin();
        while(true)
        {
            --forward_iter;
            if(*forward_iter=='\n')//dangerous
            {
                break;
            }
        }
        while(true)
        {
            ++back_iter;
            if(*back_iter=='\n')
            {
                break;
            }
        }
        msg+="\n"+std::string(forward_iter,back_iter);
    }
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

#endif // COMMON_HPP