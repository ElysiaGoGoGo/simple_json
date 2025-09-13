#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP
#include <stdexcept>
#include <iostream>
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
class Logger
{
    Logger()=default;
    decltype(std::cerr) &err=std::cerr;
    decltype(std::cout) &out=std::cout;
public:

static Logger & getInstance(){static Logger instance; return instance;}

operator decltype(std::cerr)&() {return err;}
operator decltype(std::cout)&() {return out;}

};


#endif // COMMON_HPP