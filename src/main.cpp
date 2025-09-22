//我 e6 88 91
#include <type_traits>
#include <iostream>
#include "simple_json.hpp"
#include <bitset>


int main() {    
    std::cout << "Hello, World!" << std::endl;
    std::cout<<std::filesystem::current_path()<<std::endl;
#ifdef _WIN32
    auto && obj = simple_json::parse("./CMakePresets.json");
#else
  //  auto && obj = simple_json::parse("/home/baka/.config/Code/User/globalStorage/github.copilot-chat/commandEmbeddings.json");
    auto && obj = simple_json::parse("/home/baka/文档/code/simple_json/CMakeUserPresets.json");
  #endif

    auto & cp =std::get<JsonArray>(obj["configurePresets"]);
  auto & a=cp[0];
   std::cout<<"name:\n"<<"\nobj1: \n"<<simple_json::walk_through(obj)<<"\nobj2: \n"<<cp[0].to_string(true)<<std::endl;
   simple_json::dump(obj,"/home/baka/文档/code/simple_json/CMakePresetttest.json") ;
return 0;
}
