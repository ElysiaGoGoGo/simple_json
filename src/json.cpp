#include "json.hpp"
#include "utf8handler.hpp"
#include <mutex>
#include <fstream>
vector<u_int32_t> JsonObject::get_decoded_json_file(std::filesystem::path const &path)
{
    auto file = std::ifstream(path);
    if (!(file.is_open() && std::filesystem::exists(path) && std::filesystem::is_regular_file(path)))
    {
        throw std::runtime_error("Failed to open file");
    }
    size_t filesize = std::filesystem::file_size(path);
    string buffer;
    buffer.reserve(filesize + 1);
    while (!file.eof() && file.good())
    {
        file >> buffer;
    }
    file.close();
    return UTF8Adaptor::decode(buffer);
}
JsonValue &JsonObject::operator[](string &&key)
{
    return obj.operator[](std::forward<string>(key));
}
void JsonObject::dump(std::filesystem::path const &path)
{
}
JsonObject::JsonObject(std::filesystem::path const &path)
{
#define ERROR throw std::runtime_error("Unexpected character")

    auto decoded_str = get_decoded_json_file(path);
    auto iter = decoded_str.cbegin();
    enum class IterStatus
    {
        EXPECT_LEFT_BRACE,
        EXPECT_KEY,
        EXPECT_COLON,
        EXPECT_VALUE,
        EXPECT_COMMA_OR_RIGHT_BRACE,
        READY_TO_EXIT
    };
    IterStatus status = IterStatus::EXPECT_LEFT_BRACE;
    string key_cache;
    while (true)
    {
        if (iter >= decoded_str.end())
        {
            throw "str ends not because of status: EXPECT_RIGHT_BRACE";
        }
        switch (status)
        {
        case IterStatus::EXPECT_LEFT_BRACE:

        {
            if (expect_left_brace(iter))
            {
                status = IterStatus::EXPECT_KEY;
            }
            else
            {
                ERROR;
            }
        }
        break;
        case IterStatus::EXPECT_KEY:
        {
            if (auto k = get_key(iter); k)
            {
                key_cache = *k;
            }
            else
            {
                ERROR;
            }
            status = IterStatus::EXPECT_COLON;
            break;
        }
        case IterStatus::EXPECT_COLON:
        {
            if (expect_colon(iter))
            {
                status = IterStatus::EXPECT_VALUE;
            }
            else
            {
                ERROR;
            }
            break;
        }
        case IterStatus::EXPECT_VALUE:
        {
            if (auto v = parse_value(iter); v)
            {
                obj[key_cache] = *v;
            }
            else
            {
                ERROR;
            }

            status = IterStatus::EXPECT_COMMA_OR_RIGHT_BRACE;
            break;
        }
        case IterStatus::EXPECT_COMMA_OR_RIGHT_BRACE:
        {
            if (expect_comma(iter))
            {
                status = IterStatus::EXPECT_KEY;
            }
            else if (expect_right_brace(iter))
            {
                status = IterStatus::READY_TO_EXIT;
            }
            else
            {
                ERROR;
            }
            break;
        }
        case IterStatus::READY_TO_EXIT:
        {
            goto OUT_OF_LOOP;
        }
        }
    }

OUT_OF_LOOP:
#if CXX_STANDARD <= 23
;
#endif


}

bool JsonObject::expect_left_brace(vector<u_int32_t>::const_iterator &it)
{

    auto iter=it;
    while(true)
    {
        if(*iter=='{')
        {
            it=iter+1;
            return true;
        }
        if(!is_whitespace(*iter))
        {
        return false;    
        }
        iter++;
    }    
}

bool JsonObject::expect_right_brace(vector<u_int32_t>::const_iterator &it)
{
   
    auto iter=it;
    while(true)
    {
        if(*iter=='}')
        {
            it=iter+1;
            return true;
        }
        if(!is_whitespace(*iter))
        {
        return false;    
        }
        iter++;
    }
}

bool JsonObject::expect_comma(vector<u_int32_t>::const_iterator &it)
{

    auto iter=it;
    while(true)
    {
        if(*iter==',')
        {
            it=iter+1;
            return true;
        }
        if(!is_whitespace(*iter))
        {
        return false;    
        }
        iter++;
    }



}
bool JsonObject::expect_colon(vector<u_int32_t>::const_iterator &it)
{
   
    auto iter=it;
    while(true)
    {
        if(*iter==':')
        {
            it=iter+1;
            return true;
        }
        if(!is_whitespace(*iter))
        {
        return false;    
        }
        iter++;
    }
}

std::optional<string> JsonObject::get_key(vector<u_int32_t>::const_iterator &it)
{

auto iter=it;
decltype(iter) start;
string key;
std::optional<string> result;
bool is_current_char_escaped=false;
    while(true)
    {
        if(*iter=='"')
        {
            start=iter+1;
            break;
        }
        if(!is_whitespace(*iter))
        {
            return std::nullopt;
        }
        iter++;
    }
    while(true)
    {
        if(is_current_char_escaped)
        {
            is_current_char_escaped=false;
        }
        else
        {
            if(*iter=='"')
            {
                key.resize(iter-start);
                std::copy(start,iter,key.begin());
                result.emplace(std::move(key));
                it=iter+1;
                return result;
            }
            if(*iter=='\\')
            {
                is_current_char_escaped=true;
            }
        }
        iter++;
    }


}
    std::optional<JsonValue> JsonObject::parse_value(vector<u_int32_t>::const_iterator & it)
    {

    }