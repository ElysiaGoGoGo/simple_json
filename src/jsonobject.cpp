
#include "simple_json.hpp"
#include <mutex>
#include <span>
#include <fstream>
#include <functional>
#include <regex>
#include <iterator>
using std::ifstream;
#pragma region Public Methods 
        bool JsonObjectBuilder:: reset(bool forcereset)
        {
            return true;
        }
           
        void JsonObjectBuilder::rebuild(size_t version)
        {
            
        }
        std::optional<JsonObject> JsonObjectBuilder:: get_product(size_t version)
        {
            return std::nullopt;
        }
        size_t JsonObjectBuilder:: build_from(std::filesystem::path const& path)
        {
            ifstream file(path);
            if(!(file.is_open() && std::filesystem::exists(path) && std::filesystem::is_regular_file(path)))
            {
                return ERROR;
            }
            size_t filesize=std::filesystem::file_size(path);
            std::string buffer(std::istreambuf_iterator<char>(file),{});
            file.close();
            
            this->build_cache.push_back(BuildCache{UTF8Adaptor::decode(buffer),std::span<UnicodeType>()});
            build_cache.back().json_obj_str=std::span<UnicodeType>(build_cache.back().json_file_str);
            available_version_count++;
            
            //set cache
            //TODO
            return this->build_with_cache(build_cache.size() - 1); 
        }
        size_t JsonObjectBuilder::JsonObjectBuilder::build_from(UnicodeStringView json_str)
        {
            this->build_cache.push_back(BuildCache{{},json_str});
            available_version_count++;
            //set cache
            //TODO
            return this->build_with_cache(build_cache.size() - 1); 
        }
        size_t JsonObjectBuilder::build_from(size_t old_version)
        {
            this->build_cache.push_back(build_cache[old_version]);
            available_version_count++;
            //set cache
            //TODO
            return this->build_with_cache(build_cache.size() - 1); 
        }

#pragma endregion
#pragma region Private Methods 

        size_t JsonObjectBuilder::build_with_cache(size_t version)
        {
            const auto & cache=build_cache[version];
            
            this->product_cache.push_back(std::make_unique<JsonObject>());
            const auto & decoded_str=cache.json_obj_str;//for laziness
            auto &obj=*this->product_cache.back();

        
    #define ERROR do{return(this->ERROR);}while(0) //throw LineError("Unexpected character", iter)

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
            if (cache.expect_char(iter, '{'))
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
            if (auto k = cache.get_key(iter); k)
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
            if (cache.expect_char(iter, ':'))
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
            if (auto v = cache.parse_value(iter); v)
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
            if (cache.expect_char(iter, ','))
            {
                status = IterStatus::EXPECT_KEY;
            }
            else if (cache.expect_char(iter, '}'))
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
        return version;
        
        }



#pragma endregion


bool JsonObjectBuilder::BuildCache::expect_char(UnicodeStringView::const_iterator & it, u_int32_t c)const
{
    while (it!=this->json_obj_str.end())
    {
        if (!is_whitespace(*it))
        {
            if (*it == c)
            {
                ++it;
                return true;
            }
            return false;
        }
        ++it;
    }
    return false;
}

std::optional<string> JsonObjectBuilder::BuildCache::get_key(UnicodeStringView::const_iterator &it)const
{

    UnicodeStringView::const_iterator start;
    string key;
    std::optional<string> result;
    bool is_current_char_escaped = false;
    while (true)
    {
        if (*it == '"')
        {
            start = it + 1;
            ++it;
            break;
        }
        if (!is_whitespace(*it))
        {
            return std::nullopt;
        }
        it++;
    }
    while (it!= this->json_obj_str.end())
    {
        if (is_current_char_escaped)
        {
            is_current_char_escaped = false;
        }
        else
        {
            if (*it == '"')
            {

                key.resize(it - start);
                std::copy(start, it, key.begin());
                result.emplace(std::move(key));
                ++it;
                return result;
            }
            if (*it == '\\')
            {
                is_current_char_escaped = true;
            }
        }
        ++it;
    }
    return std::nullopt;
}
std::optional<JsonValue> JsonObjectBuilder::BuildCache::parse_value(UnicodeStringView::const_iterator &it)const
{
    try
    {
        JsonValue jv(it);
        std::optional<JsonValue> op(std::move(jv));
        return op;
    }
    catch (...)
    {
        return std::nullopt;
    }
}
