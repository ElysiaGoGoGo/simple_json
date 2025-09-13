#include "json.hpp"
#include "utf8handler.hpp"
#include "common.hpp"
#include <mutex>
#include <span>
#include <fstream>
#include <functional>
#include <regex>
vector<u_int32_t> JsonObject::get_decoded_json_file(std::filesystem::path const &path)
{
    auto file = std::ifstream(path);
    if (!(file.is_open() && std::filesystem::exists(path) && std::filesystem::is_regular_file(path)))
    {
        throw std::runtime_error("Failed to open file");
    }
    size_t filesize = std::filesystem::file_size(path);

    std::string buffer(std::istreambuf_iterator<char>(file), {});
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

JsonObject::JsonObject(std::span<uint32_t> decoded_str)
{

#define ERROR throw LineError("Unexpected character", iter)

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
            if (expect_char(iter, '{'))
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
            if (expect_char(iter, ':'))
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
            if (expect_char(iter, ','))
            {
                status = IterStatus::EXPECT_KEY;
            }
            else if (expect_char(iter, '}'))
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

JsonObject::JsonObject(std::filesystem::path const &path)
{

    auto temp = get_decoded_json_file(path);

    JsonObject(std::span<uint32_t>(temp));
}

bool expect_char(span<u_int32_t>::const_iterator & it, u_int32_t c)
{
    auto iter = it;
    while (true)
    {
        if (!is_whitespace(*iter))
        {
            if (*iter == c)
            {
                it = iter + 1;
                return true;
            }
            return false;
        }
        iter++;
    }
}

std::optional<string> JsonObject::get_key(span<u_int32_t>::const_iterator &it)
{

    auto iter = it;
    decltype(iter) start;
    string key;
    std::optional<string> result;
    bool is_current_char_escaped = false;
    while (true)
    {
        if (*iter == '"')
        {
            start = iter + 1;
            ++iter;
            break;
        }
        if (!is_whitespace(*iter))
        {
            return std::nullopt;
        }
        iter++;
    }
    while (true)
    {
        if (is_current_char_escaped)
        {
            is_current_char_escaped = false;
        }
        else
        {
            if (*iter == '"')
            {

                key.resize(iter - start);
                std::copy(start, iter, key.begin());
                result.emplace(std::move(key));
                it = iter + 1;
                return result;
            }
            if (*iter == '\\')
            {
                is_current_char_escaped = true;
            }
        }
        iter++;
    }
}
std::optional<JsonValue> JsonObject::parse_value(span<u_int32_t>::const_iterator &it)
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

bool expect_right_brace_with_embedded_condition(u_int32_t c, span<u_int32_t>::const_iterator it)
{
    bool is_current_char_escaped = false;
    bool is_in_string = false;
    while(true)
    {
        if (is_current_char_escaped)
        {
            is_current_char_escaped = false;
        }
        else
        {
            if(is_in_string)
            {

            }
            else
            {
                //TODO
            }

        }
        ++it ;
    }
}
bool expect_right_bracket_with_embedded_condition(u_int32_t c, span<u_int32_t>::const_iterator it)
{

}
JsonValue::Type JsonValue::getType(std::span<uint32_t>::const_iterator iter)
{
    auto it = iter;
    while (true)
    {

        switch (*it)
        {
        case '"':
            return Type::Array;
        case '{':
            return Type::Object;
        case '[':
            return Type::Array;
        case 't':
            return Type::Boolean;
        case 'f':
            return Type::Boolean;
        case 'n':
            return Type::Null;
        case ' ':
        case '\n':
        case '\t':
        case '\r':
            break;
        default:
        {
            if (*it >= '0' && *it <= '9')
            {
                while (true)
                {
                    ++it;
                    if (*it < '0' || *it > '9')
                    {
                        if (*it == '.')
                        {
                            ++it;
                            if (!(*it < '0' || *it > '9'))
                            {
                                return Type::Float;
                            }
                            else
                            {
                                throw LineError("error when parsing JSON value type", it);
                            }
                        }
                        else if (*it == ',')
                        {
                            return Type::Integer;
                        }
                        else
                        {
                            if (is_whitespace(*it))
                            {
                                return Type::Integer;
                            }
                            else
                            {
                                throw LineError("error when parsing JSON value type", it);
                            }
                        }
                    }
                }
            }
            throw LineError("error when parsing JSON value type", it);
        }
        }

        ++it;
    }
    throw std::runtime_error("u should not reach here");
}

JsonValue::JsonValue(span<uint32_t>::const_iterator iter) : JsonValue(get_Valuerange(iter)) {}

std::span<uint32_t> JsonValue::get_Valuerange(std::span<uint32_t>::const_iterator iter)
{

    auto type = getType(iter);
    switch (type)
    {
    case Type::Array:
    case Type::Object:
    case Type::String:
    case Type::Null:
    case Type::Integer:
    case Type::Float:
    case Type::Boolean:
    break;
    }
}

JsonValue::JsonValue(std::span<uint32_t> decoded_str)
{
}
