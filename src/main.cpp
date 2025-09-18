//我 e6 88 91
#include <type_traits>
#include <iostream>
#include "simple_json.hpp"
#include <bitset>


int main() {    
    std::cout << "Hello, World!" << std::endl;
  auto a=  UTF8Adaptor::encode({0x6211});
  string s="我";
  std::vector<char> v(s.begin(),s.end());
  std::vector<u_char> v2(v.begin(),v.end());
  u_int32_t u=0b0110'001000'010'001;
   auto b=  UTF8Adaptor::decode(s);

    std::cout<<UTF8Adaptor::encode(UTF8Adaptor::decode("你好，世界！\n"));
auto s1="\u6211";
    std::cout<<std::filesystem::current_path()<<std::endl;
#ifdef _WIN32
    auto && obj = simple_json::parse("./CMakePresets.json");
#else
    auto && obj = simple_json::parse("/home/baka/.config/Code/User/globalStorage/github.copilot-chat/commandEmbeddings.json");
#endif


    auto & cp =std::get<JsonArray>(obj["configurePresets"]);
    const auto & name = std::get<string>( std::get<JsonObject>(cp[0])["name"]);


   std::cout<<"obj1: \n"<<obj<<std::endl;


return 0;
}
