#pragma once
#ifndef JSONOBJECT_HPP
#define JSONOBJECT_HPP
#include <variant>
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <string_view>
#include <tuple>
#include <ranges>
#include <span>
#include <iterator>
#include <iostream>
using std::ostream;
using std::ifstream;
using std::span;
using std::string;
using std::variant;
using std::vector;
#include "utf8handler.hpp"

struct JsonValue;
/**
 * @brief JSON数组类型定义
 * JSON array type definition
 */
using JsonArray = vector<JsonValue>;
/**
 * @brief JSON null类型定义
 * JSON null type definition
 */
using null = std::monostate;

class JsonObjectBuilder;

/**
 * @brief JSON对象类型定义
 * JSON object type definition
 */
using JsonObject = std::unordered_map<string, JsonValue>;
/**
 * @brief Json字符串类型定义（已弃用，使用string代替）
 * Json string type definition (deprecated, use string instead)
 */
using JsonString = std::string;
/**
 * @brief Json字符串视图类型定义（已弃用，使用string_view代替）
 * Json string view type definition (deprecated, use string_view instead)
 */
using JsonStringView = std::string_view;
/**
 * @brief Json字符串视图迭代器类型定义（已弃用，使用string_view::const_iterator代替）
 * Json string view iterator type definition (deprecated, use string_view::const_iterator instead)
 */
using JsonStringViewIterator = std::string_view::const_iterator;
struct Accurate_Float : string
{
operator double()const
{
    return std::stod(*this);
}

};
namespace fs = std::filesystem;


/**
 * @brief JSON对象构建器类，用于构建和解析JSON对象
 * JSON object builder class for building and parsing JSON objects
 */

class JsonObjectBuilder
{
    /**
     * @brief 构建缓存结构体，用于存储解析过程中的临时数据
     * Build cache structure for storing temporary data during parsing
     */
    struct BuildCache
    {
        JsonString json_file_str;
        JsonStringView json_obj_str;
        
        /**
         * @brief 期望特定字符，如果不是则抛出异常
         * Expect a specific character, throw exception if not found
         * @param it 迭代器引用 Iterator reference
         * @param c 期望的字符 Expected character
         */
        void expect_char(JsonStringViewIterator &it, char c) const;
        
   
        /**
         * @brief 获取键名
         * Get key name
         * @param it 迭代器引用 Iterator reference
         * @return 键名字符串 Key name string
         */
        string get_key(JsonStringViewIterator &it) const;
        
        /**
         * @brief 解析JSON值
         * Parse JSON value
         * @param it 迭代器引用 Iterator reference
         * @return 解析后的JSON值 Parsed JSON value
         */
        JsonValue parse_value(JsonStringViewIterator &it) const;
    };
    BuildCache build_cache;
    mutable std::optional<JsonObject> product_cache;

    /**
     * @brief 使用缓存构建JSON对象
     * Build JSON object using cache
     */
    void build_with_cache() const;

public:
    /**
     * @brief 重置构建器状态
     * Reset builder state
     * @param force_reset 是否强制重置 Whether to force reset
     * @return 重置是否成功 Whether reset was successful
     */
    bool reset(bool force_reset = false);
    
    /**
     * @brief 从文件路径构建JSON对象
     * Build JSON object from file path
     * @param path 文件路径 File path
     */

    void build_from(std::filesystem::path const &path);
    
    /**
     * @brief 从Json字符串视图构建JSON对象
     * Build JSON object from Json string view
     * @param json_str Json字符串视图 Json string view
     */
    void build_from(const JsonStringView json_str);
 static     void add(JsonObject & obj,JsonStringView json_str);

    /**
     * @brief 获取构建的产品（JSON对象）
     * Get the built product (JSON object)
     * @return 可选的JSON对象 Optional JSON object
     */
    std::optional<JsonObject> get_product();
};

#endif // JSONOBJECT_HPP
