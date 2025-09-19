
#include "simple_json.hpp"
#include <regex>
/**
 * @brief 获取字符串值和结束迭代器
 * Get string value and end iterator
 * @return 包含字符串和迭代器的元组 Tuple containing string and iterator
 */
tuple<string,JsonStringViewIterator> JsonValue::JsonValueBuilder::get_string_value_and_end() const
{
    JsonStringViewIterator start;
    auto it = this->view_to_build_from.begin();
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
            throw LineError("Expecting '\"' or any whitespace character", this->view_to_build_from.begin());
        }
        it++;
    }
    while (it != this->view_to_build_from.end())
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
                return {key,it};
            }
            if (*it == '\\')
            {
                is_current_char_escaped = true;
            }
        }
        ++it;
    }
    throw LineError("Unexpected end of file", this->view_to_build_from.begin());
}
JsonValue::Type JsonValue::JsonValueBuilder:: getType()const 
{
    //the type returned  
switch (*view_to_build_from.begin())
        {
        case '"':
            return Type::String;
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
        default:
        {
            //([+-]?\d+)(\.\d+)?(e[+-]?\d+)?    
            return Type::Float;//TODO fix it
        }
    }
    }

JsonStringView JsonValue::JsonValueBuilder:: get_Value_range(Type type)
{
    switch (type)
    {
    case Type::Array:
    return view_to_build_from.substr(0, this->expect_right_brace_or_right_bracket_with_embedded_condition(false) - this->view_to_build_from.begin());
    case Type::Object:
     return view_to_build_from.substr(0, this->expect_right_brace_or_right_bracket_with_embedded_condition(true) - this->view_to_build_from.begin());
    case Type::String:
    case Type::Null:
    case Type::Integer:
    case Type::Float:
    case Type::Boolean:
    default:
        throw std::logic_error("this method is not designed for these types");
    break;
    }
}

  std::tuple<JsonValue,JsonStringViewIterator> JsonValue::JsonValueBuilder:: build()//返回的iter指向被解析的值的下一个字符
  {

    auto type = getType();
#ifdef DEBUG
   std::cout<<" \n\nview_to_build_from:" <<string (view_to_build_from.begin(),view_to_build_from.end())<<std::endl;
#endif

    switch (type)
    {
    case Type::Array:
    {auto value_range = get_Value_range(type);    
        JsonArray array;
        array.reserve(std::count(value_range.begin(),value_range.end(),','));
        JsonValueBuilder builder(value_range.substr(1, value_range.size()-2));

        while(true)
    {
 
 auto [value,iter]=    builder.build();
        array.emplace_back(std::move(value));
 if(auto c=*iter;c==',')
 {
 ++iter;
 }
 else if(c==']')
 {
  
break;
 }
 else
 {
    throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());
 }
    
      builder.view_to_build_from.remove_prefix(iter - builder.view_to_build_from.begin());
}


    return {array,value_range.end()};
    }

    case Type::Object:
    {auto value_range = get_Value_range(type);
    JsonObjectBuilder builder;
    builder.build_from(value_range);
#ifdef DEBUG
    std::cout<<"\n\nvalue range:"<<string (value_range.begin(),value_range.end())<<std::endl;
#endif
    return {*(builder.get_product()),value_range.end()};
    }
    case Type::String:
    return get_string_value_and_end();
        case Type::Null:
        return [this]()->  std::tuple<JsonValue,JsonStringViewIterator>{ if( [this]()->bool{  
            
            return view_to_build_from[0]=='n' && view_to_build_from[1]=='u' && view_to_build_from[2]=='l' && view_to_build_from[3]=='l';
            
            }()   ) {return {JsonValue(null()),this->view_to_build_from.begin()+4};}else {throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());}    }();
    case Type::Integer:
    return [this]()->  std::tuple<JsonValue,JsonStringViewIterator>{ for(auto iter=this->view_to_build_from.begin();iter!=this->view_to_build_from.end();++iter)
    {
        if(is_whitespace(*iter)||is_end_char(*iter))
        {
            return {std::stoi(std::string(view_to_build_from.begin(),iter)),iter};
        }
    }
    throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());
    
    }();
    case Type::Float:
        return [this]()->  std::tuple<JsonValue,JsonStringViewIterator>{ for(auto iter=this->view_to_build_from.begin();iter!=this->view_to_build_from.end();++iter)
    {
        if(is_whitespace(*iter)||is_end_char(*iter))
        {
            return {std::string(view_to_build_from.begin(),iter),iter};
        }
    }
    throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());
    
    }();
    case Type::Boolean:
    return [this]()->  std::tuple<JsonValue,JsonStringViewIterator>{auto & v=this->view_to_build_from;
    if(v[0]=='t'&&v[1]=='r'&&v[2]=='u'&&v[3]=='e')
    {
        return {JsonValue(true),this->view_to_build_from.begin()+4};
    }
     if(v[0]=='f'&&v[1]=='a'&&v[2]=='l'&&v[3]=='s'&&v[4]=='e')
    {
        return {JsonValue(false),this->view_to_build_from.begin()+5};
    }
    
    
        throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());
    

    }();
    break;
    default:
        throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());
    }

throw std::logic_error("never reach here");
  }

string JsonValue::to_string(bool fill_whitespaces ) const
{
    string result;

    if(fill_whitespaces)
    {
    std::visit([&](auto && arg)
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, string>)
        {
            result="\""+arg+"\"";
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            result=arg?"true":"false";
        }
        else if constexpr (std::is_same_v<T, Accurate_Float>)
        {
            result=arg;
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            result=std::to_string(arg);
        }
        else if constexpr (std::is_same_v<T, JsonArray>)
        {
            result.push_back('[');
            result.push_back('\n');
            bool first=true;
            for (const auto & v:arg)
            {
                if(first)
                {
                    first=false;
                }
                else
                {
                    result += ",\n";
                }
               result+= v.to_string(true);
            }
            result.push_back('\n');
            result.push_back(']');
        }
        else if constexpr (std::is_same_v<T, JsonObject>)
        {
            result= simple_json::walk_through(arg,true);
        }
        else
        {
            result="null";
        }
    },*(static_cast<const JsonRawValueType  *>(this)));


    }

    else
    {
    std::visit([&](auto && arg)
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, string>)
        {
            result="\""+arg+"\"";
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            result=arg?"true":"false";
        }
        else if constexpr (std::is_same_v<T, Accurate_Float>)
        {
            result=arg;
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            result=std::to_string(arg);
        }
        else if constexpr (std::is_same_v<T, JsonArray>)
        {
            result.push_back('[');
            bool first=true;
            for (const auto & v:arg)
            {
                if(first)
                {
                    first=false;
                }
                else
                {
                    result += ',';
                }
               result+= v.to_string();
            }
            result.push_back(']');
        }
        else if constexpr (std::is_same_v<T, JsonObject>)
        {
            result= simple_json::walk_through(arg);
        }
        else
        {
            result="null";
        }
    },*(static_cast<const JsonRawValueType  *>(this)));


    }



    return result;
}

JsonStringViewIterator JsonValue::JsonValueBuilder:: expect_right_brace_or_right_bracket_with_embedded_condition( bool is_right_brace_expected)
{
    char l= is_right_brace_expected?'{':'[',r= is_right_brace_expected?'}':']';

    bool is_current_char_escaped = false;
    bool is_in_string = false;
    uint32_t l_count = 0;
    uint32_t r_encountered_count = 0;
    for(auto it=this->view_to_build_from.begin();it!=this->view_to_build_from.end();++it)
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
                else if(c==l)
                {
                    l_count++;
                }
                else if(c==r)
                {
                    r_encountered_count++;
                    if(r_encountered_count==l_count)
                    {
                        return it+1;
                    }
                }
            }

        }

    }
    throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());
}
