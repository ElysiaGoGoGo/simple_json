
#include "simple_json.hpp"
#include <mutex>
#include <span>
#include <fstream>
#include <functional>
#include <regex>
#include <iterator>
using std::ifstream;
#pragma region helper_method
void read_json_without_external_whitespaces(std::string &result, std::ifstream &file) {
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    result.reserve(content.size());
    bool in_string = false;
    bool escaped = false;
    
    for (size_t i = 0; i < content.size(); ++i) {
        char c = content[i];
        
        if (!in_string && is_whitespace(c)) {
            continue; 
        }
        result += c;
        if (escaped) {
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else if (c == '\"') {
            in_string = !in_string;
        }
    }
}


#pragma endregion


#pragma region Public Methods
bool JsonObjectBuilder::reset(bool force_reset)
{
    build_cache = {};
    if (force_reset || !this->product_cache.has_value())
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
    
    std::string buffer;
read_json_without_external_whitespaces(buffer,file);

    file.close();

    build_cache.json_file_str = buffer;
    build_cache.json_obj_str = JsonStringView(build_cache.json_file_str);

    this->build_with_cache();
}
void JsonObjectBuilder::JsonObjectBuilder::build_from(const JsonStringView json_str)
{

    build_cache.json_obj_str = json_str;
    this->build_with_cache();
}

void JsonObjectBuilder::add(JsonObject& obj, const JsonStringView json_str)
{
    //TODO

}




#pragma endregion
#pragma region Private Methods

void JsonObjectBuilder::build_with_cache() const
{
#ifdef DEBUG
    std::cout << "building with cache:\n"
              << string(build_cache.json_obj_str.begin(), build_cache.json_obj_str.end()) << std::endl;
#endif
    this->product_cache.emplace(JsonObject());
    const auto &str = build_cache.json_obj_str; // for laziness

    auto iter = str.begin();
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
    if(*iter =='{'&&*(iter+1)=='}')
    {
        return ;//empty object
    }
    auto end=str.end();
    while (true)
    {
        if (iter >= end)
        {
            if (iter == end && status == IterStatus::READY_TO_EXIT)
            {
                break;
            }
            throw LineError("Unexpected end of obj str", iter); // throw "str ends not because of status: EXPECT_RIGHT_BRACE";
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
            this->product_cache->operator[](key_cache) = std::move(build_cache.parse_value(iter));
#ifdef DEBUG
            auto index = iter - build_cache.json_obj_str.begin();

            std::cout << "parsed value at index " << index << std::endl;
#endif
            status = IterStatus::EXPECT_COMMA_OR_RIGHT_BRACE;
            break;
        }
        case IterStatus::EXPECT_COMMA_OR_RIGHT_BRACE:
        {
            if(auto c=*iter;c==',')
            {
                status=IterStatus::EXPECT_KEY;
            }
            else if(c=='}')
            {
                status=IterStatus::READY_TO_EXIT;
            }
            else
            {
                throw LineError("Expecting ',' or '}'", iter);
            }
            ++iter;
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

#pragma endregion

#pragma region BuildCache Methods
void JsonObjectBuilder::BuildCache::expect_char(JsonStringViewIterator &it, char c) const
{
if(*it==c)
{
    ++it;
}
else
{
    throw LineError("Expecting '"+std::string(1,c)+"'", it);
}
}


string JsonObjectBuilder::BuildCache::get_key(JsonStringViewIterator &it) const
{

    JsonStringViewIterator start;
    string key;
    bool is_current_char_escaped = false;
        if (*it == '"')
        {
            start = it + 1;
            ++it;
        }
        else
{            throw LineError("Expecting '\"' or any whitespace character", it);
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
                key = string(start, it);
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
JsonValue JsonObjectBuilder::BuildCache::parse_value(JsonStringViewIterator &it) const
{
    auto &&[result, end] =
        JsonValue::JsonValueBuilder(JsonStringView(it, this->json_obj_str.end())).build();
    it = end;
    return result;
}

#pragma endregion
