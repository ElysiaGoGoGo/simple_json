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
 * @brief Unicode字符类型定义
 * Unicode character type definition
 */
using UnicodeType = uint32_t;
/**
 * @brief Unicode字符串类型定义
 * Unicode string type definition
 */
using UnicodeString = std::vector<UnicodeType>;
/**
 * @brief Unicode字符串视图类型定义
 * Unicode string view type definition
 */
using UnicodeStringView = std::span<const UnicodeType>;
/**
 * @brief Unicode字符串视图迭代器类型定义
 * Unicode string view iterator type definition
 */
using UnicodeStringViewIterator = UnicodeStringView::iterator;
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
        UnicodeString json_file_str;
        UnicodeStringView json_obj_str;
        
        /**
         * @brief 期望特定字符，如果不是则抛出异常
         * Expect a specific character, throw exception if not found
         * @param it 迭代器引用 Iterator reference
         * @param c 期望的字符 Expected character
         */
        void expect_char(UnicodeStringViewIterator &it, u_int32_t c) const;
        
        /**
         * @brief 期望逗号或右大括号
         * Expect comma or right brace
         * @param it 迭代器引用 Iterator reference
         * @param is_comma_expected 是否期望逗号 Whether comma is expected
         * @return 如果找到期望的字符返回true，否则false Returns true if expected character found, false otherwise
         */
        bool expect_comma_or_right_brace(UnicodeStringViewIterator &it,bool is_comma_expected) const;
        
        /**
         * @brief 获取键名
         * Get key name
         * @param it 迭代器引用 Iterator reference
         * @return 键名字符串 Key name string
         */
        string get_key(UnicodeStringViewIterator &it) const;
        
        /**
         * @brief 解析JSON值
         * Parse JSON value
         * @param it 迭代器引用 Iterator reference
         * @return 解析后的JSON值 Parsed JSON value
         */
        JsonValue parse_value(UnicodeStringViewIterator &it) const;
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
     * @param forcereset 是否强制重置 Whether to force reset
     * @return 重置是否成功 Whether reset was successful
     */
    bool reset(bool forcereset = false);
    
    /**
     * @brief 从文件路径构建JSON对象
     * Build JSON object from file path
     * @param path 文件路径 File path
     */
    void build_from(std::filesystem::path const &path);
    
    /**
     * @brief 从Unicode字符串视图构建JSON对象
     * Build JSON object from Unicode string view
     * @param json_str Unicode字符串视图 Unicode string view
     */
    void build_from(UnicodeStringView json_str);
    
    /**
     * @brief 获取构建的产品（JSON对象）
     * Get the built product (JSON object)
     * @return 可选的JSON对象 Optional JSON object
     */
    std::optional<JsonObject> get_product();
};

#endif // JSONOBJECT_HPP
