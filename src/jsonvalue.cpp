
#include "simple_json.hpp"
JsonValue::Type JsonValue::JsonValueBuilder:: getType()
{//TODO
    auto it = this->view_to_build_from.begin();
    while (it!= this->view_to_build_from.end())
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
                                throw LineError("error when parsing JSON value type", this->view_to_build_from);
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
                                throw LineError("error when parsing JSON value type", this->view_to_build_from);
                            }
                        }
                    }
                }
            }
            throw LineError("error when parsing JSON value type", this->view_to_build_from);
        }
        }

        ++it;
    }
    
    throw std::runtime_error("u should not reach here");
}

UnicodeStringView JsonValue::JsonValueBuilder:: get_Value_range(Type type)
{//TODO
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

  JsonValue JsonValue::JsonValueBuilder:: build()
  {//TODO
    auto type = getType();
auto value_range = get_Value_range(type);
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
    default:
        throw LineError("error when parsing JSON value type",this->view_to_build_from);
    }


  }

bool expect_right_brace_with_embedded_condition( UnicodeStringViewIterator it)
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
bool expect_right_bracket_with_embedded_condition(UnicodeStringViewIterator it)
{
return false;
}
