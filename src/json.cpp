
#include "simple_json.hpp"
#include <mutex>
#include <span>
#include <fstream>
#include <functional>
#include <regex>
#include <iterator>

/**
 * @brief 从文件路径解析JSON对象
 * Parse JSON object from file path
 * @param path 文件路径 File path
 * @return 解析后的JSON对象 Parsed JSON object
 */
JsonObject simple_json::parse(std::filesystem::path const &path)
{

  return [&path] -> JsonObject{    JsonObjectBuilder builder;builder.build_from(path);return builder.get_product().value();  }();
}

/**
 * @brief 将JSON对象转储到文件
 * Dump JSON object to file
 * @param obj JSON对象 JSON object
 * @param path 文件路径 File path
 */
void simple_json::dump(JsonObject const &obj, std::filesystem::path const &path)
{
  std::ofstream file(path);
  file << obj;
  file.close();
}
    string simple_json:: walk_through(JsonObject const& obj,bool fill_whitespaces,size_t indent_count,bool is_colon_before)
{
  if(fill_whitespaces)
  {

    if(obj.empty())
    {
      return "{}";
    }
    string result;
    
    if(!is_colon_before)
    {
      result+=(string(indent_count*indent_size,' '));
    }
    result+="{\n";
    
  bool is_first_pair=true;
  for ( const auto & [key, value] : obj)
  {
    if ( is_first_pair )
    {
      is_first_pair=false;
    }
    else
    {
      result.push_back(',');
      result.push_back('\n');      
    }
    result+= ( string(    (is_colon_before?indent_count:(indent_count+1))*indent_size,' ')+ "\""+key+"\": "+value.to_string(true,(indent_count+1),true))  ;
    
  }
  if(!obj.empty())
{  result.push_back('\n');
}
if(is_colon_before)
{
result+=string((indent_count-1)*indent_size,' ');



}
else
{
  result+=string(indent_count*indent_size,' ');

}
  result.push_back('}');
return  result;
  }
  else
  {
string result="{";
  bool is_first_pair=true;
  for ( const auto & [key, value] : obj)
  {
    if ( is_first_pair )
    {
      is_first_pair=false;
    }
    else
    {
      result.push_back(','); 
    }
    result+= ("\""+key+"\":"+value.to_string())  ;
    
  }
  result.push_back('}');
return  result;
  }
  
}
ostream& operator<<(ostream& os, const JsonObject& obj)
{
os<<simple_json:: walk_through(obj,true,0,false);
  return os;
}
ifstream& operator>>(ifstream& is, JsonObject& obj)
{
  const std::string buffer{std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>()};
  JsonObjectBuilder::add(obj, buffer);
  return is;
}
