
#include "simple_json.hpp"

/**
 * @brief 获取字符串值和结束迭代器
 * Get string value and end iterator
 * @return 包含字符串和迭代器的元组 Tuple containing string and iterator
 */
tuple<string,UnicodeStringViewIterator> JsonValue::JsonValueBuilder::get_string_value_and_end() const
{
    UnicodeStringViewIterator start;
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

                key.resize(it - start);
                std::copy(start, it, key.begin());
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
    auto it = this->view_to_build_from.begin();
    while (it!= this->view_to_build_from.end())
    {

        switch (*it)
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
        case ' ':
        case '\n':
        case '\t':
        case '\r':
            throw LineError("Unexpected whitespace when parsing JSON value type", this->view_to_build_from.begin());
            
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
                                return Type::Float;
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
                                throw LineError("error when parsing JSON value type", this->view_to_build_from.begin());
                            }
                        }
                    }
                }
            }
            throw LineError("error when parsing JSON value type", this->view_to_build_from.begin());
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
    return view_to_build_from.first(this->expect_right_brace_or_right_bracket_with_embedded_condition(false)-this->view_to_build_from.begin());
    case Type::Object:
     return view_to_build_from.first(this->expect_right_brace_or_right_bracket_with_embedded_condition(true)-this->view_to_build_from.begin());
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

  std::tuple<JsonValue,UnicodeStringViewIterator> JsonValue::JsonValueBuilder:: build()//返回的iter指向被解析的值的下一个字符
  {//TODO
    skip_whitespace();
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
        JsonValueBuilder builder(value_range.subspan(1,value_range.size()-2));
        auto skip_comma =[](UnicodeStringViewIterator & iter)->bool{
            LoopGuard guard;
            while(true)
            {
                guard();
                if(auto c=*iter;c==',')
                {
                    ++iter;
                    return true;
                }
                else if(c==']')
                {
                    ++iter;
                    return false;
                }
                else if(!is_whitespace(c))
                {
                        throw LineError("Expecting ',' or ']' or any whitespace character", iter);
                }
                ++iter;
                }
            

        };
        LoopGuard guard;
        while(true)
    {
            guard();
 auto [value,iter]=    builder.build();
        array.emplace_back(std::move(value));
      if(!skip_comma(iter))
      {
        break;
      }
      builder.view_to_build_from=builder.view_to_build_from.subspan(iter-builder.view_to_build_from.begin());
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
        return [this]()->  std::tuple<JsonValue,UnicodeStringViewIterator>{ if( [this]()->bool{  
            
            return view_to_build_from[0]=='n' && view_to_build_from[1]=='u' && view_to_build_from[2]=='l' && view_to_build_from[3]=='l';
            
            }()   ) {return {JsonValue(null()),this->view_to_build_from.begin()+4};}else {throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());}    }();
    case Type::Integer:
    return [this]()->  std::tuple<JsonValue,UnicodeStringViewIterator>{ for(auto iter=this->view_to_build_from.begin();iter!=this->view_to_build_from.end();++iter)
    {
        if(is_whitespace(*iter)||is_end_char(*iter))
        {
            return {std::stoi(std::string(view_to_build_from.begin(),iter)),iter};
        }
    }
    throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());
    
    }();
    case Type::Float:
        return [this]()->  std::tuple<JsonValue,UnicodeStringViewIterator>{ for(auto iter=this->view_to_build_from.begin();iter!=this->view_to_build_from.end();++iter)
    {
        if(is_whitespace(*iter)||is_end_char(*iter))
        {
            return {std::stod(std::string(view_to_build_from.begin(),iter)),iter};
        }
    }
    throw LineError("error when parsing JSON value type",this->view_to_build_from.begin());
    
    }();
    case Type::Boolean:
    return [this]()->  std::tuple<JsonValue,UnicodeStringViewIterator>{auto & v=this->view_to_build_from;
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

  UnicodeStringViewIterator JsonValue::JsonValueBuilder:: expect_right_brace_or_right_bracket_with_embedded_condition( bool is_right_brace_expected)
{
    uint32_t l= is_right_brace_expected?'{':'[',r= is_right_brace_expected?'}':']';

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
