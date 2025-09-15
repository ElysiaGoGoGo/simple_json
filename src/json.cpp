
#include "simple_json.hpp"
#include <mutex>
#include <span>
#include <fstream>
#include <functional>
#include <regex>
#include <iterator>

JsonObject simple_json::parse(std::filesystem::path const &path)
{

  return [&path] -> JsonObject{    JsonObjectBuilder builder;builder.build_from(path);return builder.get_product().value();  }();
}
void simple_json::dump(JsonObject const &obj, std::filesystem::path const &path)
{
  // TODO
}
