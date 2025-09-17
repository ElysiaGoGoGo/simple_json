//我 e6 88 91
#include <type_traits>
#include <iostream>
#include "simple_json.hpp"


int main() {    

    std::cout << "Hello, World!" << std::endl;
    UTF8Adaptor::encode({0x6211});
    UTF8Adaptor::decode("我");

    std::cout<<UTF8Adaptor::encode(UTF8Adaptor::decode("你好，世界！\n"));

       auto && obj = simple_json::parse("./CMakePresets.json");
    auto & cp =std::get<JsonArray>(obj["configurePresets"]);
    auto & name = std::get<string>( std::get<JsonObject>(cp[0])["name"]);


   std::cout<<"name: "<<name<<std::endl;

return 0;
}
