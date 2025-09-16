
#include "simple_json.hpp"
#include <mutex>
#include <span>
#include <fstream>
#include <functional>
#include <regex>
#include <iterator>
using std::ifstream;
#define ERROR         \
    do                \
    {                 \
        return false; \
    } while (0) // throw LineError("Unexpected character", iter)
#pragma region Public Methods
bool JsonObjectBuilder::reset(bool forcereset)
{
    build_cache = {};
    if (forcereset || !this->product_cache.has_value())
    {
        this->product_cache.reset();
        this->build_cache = {};
        return true;
    }
    else
    {
        return false;
    }
}
std::optional<JsonObject> JsonObjectBuilder::get_product()
{
    return this->product_cache.has_value() ? std::move(product_cache) : std::nullopt;
}
void JsonObjectBuilder::build_from(std::filesystem::path const &path)
{
    ifstream file(path);
    if (!(file.is_open() && std::filesystem::exists(path) && std::filesystem::is_regular_file(path)))
    {
        throw std::invalid_argument("Invalid file path");
    }
    size_t filesize = std::filesystem::file_size(path);
    std::string buffer(std::istreambuf_iterator<char>(file), {});
    file.close();
    if(buffer.size()!=filesize)
    {
        throw std::runtime_error("File size mismatch");
    }
    build_cache.json_file_str = UTF8Adaptor::decode(buffer);
    build_cache.json_obj_str = UnicodeStringView(build_cache.json_file_str);

    this->build_with_cache();
}
void JsonObjectBuilder::JsonObjectBuilder::build_from(UnicodeStringView json_str)
{

    build_cache.json_obj_str = UnicodeStringView(build_cache.json_file_str);
     this->build_with_cache();
}

#pragma endregion
#pragma region Private Methods

bool JsonObjectBuilder::build_with_cache() const
{

    this->product_cache.emplace(JsonObject());
    const auto &decoded_str = build_cache.json_obj_str; // for laziness

    auto iter = decoded_str.begin();
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
            ERROR;//throw "str ends not because of status: EXPECT_RIGHT_BRACE";
        }
        switch (status)
        {
        case IterStatus::EXPECT_LEFT_BRACE:

        {
            build_cache.expect_char(iter, '{');
            
                status = IterStatus::EXPECT_KEY;
            
        }
        break;
        case IterStatus::EXPECT_KEY:
        {
            
            
                key_cache = std::move(build_cache.get_key(iter));
            
            status = IterStatus::EXPECT_COLON;
            break;
        }
        case IterStatus::EXPECT_COLON:
        {
            build_cache.expect_char(iter, ':');
            
                status = IterStatus::EXPECT_VALUE;
            
            break;
        }
        case IterStatus::EXPECT_VALUE:
        {
                this->product_cache->operator[](key_cache) =std::move(build_cache.parse_value(iter));
         
            status = IterStatus::EXPECT_COMMA_OR_RIGHT_BRACE;
            break;
        }
        case IterStatus::EXPECT_COMMA_OR_RIGHT_BRACE:
        {
            if (build_cache.expect_comma_or_right_brace(iter,true))
            {
                status = IterStatus::EXPECT_KEY;
            }
            else if (build_cache.expect_comma_or_right_brace(iter, false))
            {
                status = IterStatus::READY_TO_EXIT;
            }
            else
            {
                throw LineError("Expecting ',' or '}'", iter);
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
    return true;
}

#pragma endregion

#pragma region BuildCache Methods
void JsonObjectBuilder::BuildCache::expect_char(UnicodeStringViewIterator &it, u_int32_t c) const
{
    while (it != this->json_obj_str.end())
    {
        if (!is_whitespace(*it))
        {
            if (*it == c)
            {
                ++it;
                return ;
            }
            throw LineError("Unexpected character", it);
        }
        ++it;
    }
    throw LineError("Unexpected end of file", it);
}
 bool JsonObjectBuilder::BuildCache:: expect_comma_or_right_brace(UnicodeStringViewIterator &it,bool is_comma_expected) const
 {
    u_int32_t c=is_comma_expected? ',':'}';
  while (it != this->json_obj_str.end())
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

string JsonObjectBuilder::BuildCache::get_key(UnicodeStringViewIterator &it) const
{

    UnicodeStringViewIterator start;
    string key;
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
            throw LineError("Expecting '\"' or any whitespace character", it);
        }
        it++;
    }
    while (it != this->json_obj_str.end())
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
                ++it;
                return key;
            }
            if (*it == '\\')
            {
                is_current_char_escaped = true;
            }
        }
        ++it;
    }
    throw LineError("Unexpected end of file", it);
}
JsonValue JsonObjectBuilder::BuildCache::parse_value(UnicodeStringViewIterator &it) const
{
auto && [result,end]=
     JsonValue::JsonValueBuilder(UnicodeStringView(it, this->json_obj_str.end())).build();
    it=end;
    return result;
    }

#pragma endregion