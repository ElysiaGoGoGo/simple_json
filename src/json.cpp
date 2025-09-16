
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
  // TODO: 实现JSON对象转储功能 Implement JSON object dump functionality
}
