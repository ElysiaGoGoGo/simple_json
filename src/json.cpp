
#include "simple_json.hpp"
#include <mutex>
#include <span>
#include <fstream>
#include <functional>
#include <regex>
#include <iterator>

 JsonObject simple_json::parse(std::filesystem::path const& path)
 {
    JsonObjectBuilder builder;
  auto version=  builder.build_from(path);
    return builder.get_product(version).value();
 }
void simple_json::dump(JsonObject const& obj, std::filesystem::path const& path)
{
    //TODO
}




bool expect_right_brace_with_embedded_condition( span<u_int32_t>::const_iterator it)
{
    bool is_current_char_escaped = false;
    bool is_in_string = false;
    uint32_t extra_left_brace_count = 0;
    uint32_t right_brace_encountered_count = 0;
    while(true)
    {
        if (is_current_char_escaped)
        {
            is_current_char_escaped = false;
        }
        else
        {
            if(auto c=*it;is_in_string)
            {
                if(c=='"')
                {
                    is_in_string=false;
                }
                else if(c=='\\')
                {
                    is_current_char_escaped=true;
                }
            }
            else
            {
                if(c=='"')
                {
                    is_in_string=true;
                }
                else if(c=='{')
                {
                    extra_left_brace_count++;
                }
                else if(c=='}')
                {
                    right_brace_encountered_count++;
                    if(right_brace_encountered_count+1==extra_left_brace_count)
                    {
                        return true;
                    }
                }
            }

        }
        ++it ;
    }
}
bool expect_right_bracket_with_embedded_condition(span<u_int32_t>::const_iterator it)
{

}
