
#include "simple_json.hpp"
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
