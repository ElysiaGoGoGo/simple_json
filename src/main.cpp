#include <type_traits>
#include <iostream>
#include "simple_json.hpp"
int main() {
    static_assert(sizeof(uint32_t) == 4, "uint32_t is not 4 bytes long");
    static_assert(std::is_integral_v<uint32_t>, "uint32_t is not an integral type");
    static_assert(std::is_unsigned_v<uint32_t>, "uint32_t is not an integral type");
    
    std::cout << "Hello, World!" << std::endl;
   auto obj = simple_json::parse("./CmakePresets.json");
    return 0;
}