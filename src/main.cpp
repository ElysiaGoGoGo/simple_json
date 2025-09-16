#include <type_traits>
#include <iostream>
#include "simple_json.hpp"
int main() {    
    std::cout << "Hello, World!" << std::endl;

     auto && obj = simple_json::parse("./test.json");
    auto & cp =std::get<JsonArray>(obj["configurePresets"]);
    auto & name = std::get<string>( std::get<JsonObject>(cp[0])["name"]);

   std::cout<<"name: "<<name<<std::endl;

  

return 0;
}