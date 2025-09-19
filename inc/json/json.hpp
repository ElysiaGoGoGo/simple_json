#pragma once
#ifndef JSON_HPP
#define JSON_HPP
#include "jsonobject.hpp"
#include "jsonvalue.hpp"
#include "functional"
namespace simple_json
{
/**
 * @brief 从文件路径解析JSON对象
 * Parse JSON object from file path
 * @param path 文件路径 File path
 * @return 解析后的JSON对象 Parsed JSON object
 */
JsonObject parse(std::filesystem::path const& path);

/**
 * @brief 将JSON对象转储到文件
 * Dump JSON object to file
 * @param obj JSON对象 JSON object
 * @param path 文件路径 File path
 */
void dump(JsonObject const& obj, std::filesystem::path const& path);
    string walk_through(JsonObject const& obj,bool fill_whitespaces=false);



};
ostream& operator<<(ostream& os, const JsonObject& obj);
ifstream& operator>>(ifstream& is, JsonObject& obj);

#endif // JSON_HPP
