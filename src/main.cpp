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
    auto && obj = simple_json::parse("/home/baka/文档/code/simple_json/CMakePresets.json");
  #endif

    auto & cp =std::get<JsonArray>(obj["configurePresets"]);
    const auto & name = std::get<JsonObject>(cp[0])["cacheVairiables"].to_string();

   std::cout<<"name:\n"<< name<<"\nobj1: \n"<<obj<<std::endl;

return 0;
}
