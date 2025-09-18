#pragma once
#ifndef JSONVALUE_HPP
#define JSONVALUE_HPP
#include <variant>
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <string_view>
#include <tuple>
#include <ranges>
#include <span>
#include <iostream>
#include <json/common.hpp>
using std::span;
using std::string;
using std::variant;
using std::vector;
using std::tuple;
#include "utf8handler.hpp"
#include "algorithm"
#include "jsonobject.hpp"

/**
 * @brief JSON原始值类型定义
 * JSON raw value type definition
 */
using JsonRawValueType = variant<string, bool, Accurate_Float, int, JsonArray, JsonObject, null>;

/**
 * @brief JSON值类，继承自std::variant，表示JSON的各种可能值类型
 * JSON value class, inherits from std::variant, represents various possible JSON value types
 */
struct JsonValue : variant<string, bool, Accurate_Float, int, JsonArray, JsonObject, null>
{
    /**
     * @brief JSON值类型枚举
     * JSON value type enumeration
     */
    enum class Type
    {
        String,     ///< 字符串类型 String type
        Boolean,    ///< 布尔类型 Boolean type
        Float,      ///< 浮点数类型 Float type
        Integer,    ///< 整数类型 Integer type
        Array,      ///< 数组类型 Array type
        Object,     ///< 对象类型 Object type
        Null        ///< 空类型 Null type
    };

    /**
     * @brief JSON值构建器类，用于解析和构建JSON值
     * JSON value builder class for parsing and building JSON values
     */
    class JsonValueBuilder
    {
        UnicodeStringView view_to_build_from;
        
        /**
         * @brief 获取值的类型
         * Get the type of value
         * @return 值类型 Value type
         */
        Type getType() const;
        
        /**
         * @brief 跳过空白字符
         * Skip whitespace characters
         */
        void skip_whitespace()
        {
            for(auto iter=this->view_to_build_from.begin();iter!=this->view_to_build_from.end();++iter)
            {
                if(!is_whitespace(*iter))
                {
                    this->view_to_build_from=this->view_to_build_from.subspan(iter-this->view_to_build_from.begin());
                    break;
                }
            }
        }
        
        /**
         * @brief 获取值的范围
         * Get the range of value
         * @param type 值类型 Value type
         * @return Unicode字符串视图 Unicode string view
         */
        UnicodeStringView get_Value_range(Type type);
        
        /**
         * @brief 期望右大括号或右中括号（带嵌入条件）
         * Expect right brace or right bracket with embedded condition
         * @param is_right_brace_expected 是否期望右大括号 Whether right brace is expected
         * @return Unicode字符串视图迭代器 Unicode string view iterator
         */
        UnicodeStringViewIterator expect_right_brace_or_right_bracket_with_embedded_condition(bool is_right_brace_expected);
        
        /**
         * @brief 获取字符串值和结束迭代器
         * Get string value and end iterator
         * @return 包含字符串和迭代器的元组 Tuple containing string and iterator
         */
        tuple<string, UnicodeStringViewIterator> get_string_value_and_end() const;

    public:
        /**
         * @brief 构造函数
         * Constructor
         * @param str Unicode字符串视图 Unicode string view
         */
        explicit JsonValueBuilder(const UnicodeStringView str) : view_to_build_from(str) {}
        
        /**
         * @brief 构建JSON值
         * Build JSON value
         * @return 包含JSON值和迭代器的元组 Tuple containing JSON value and iterator
         */
        std::tuple<JsonValue, UnicodeStringViewIterator> build();
    };
    
    JsonValue &operator=(JsonValue &&) = default;
    JsonValue(const JsonValue &) = default;
    JsonValue(JsonValue &&) = default;
    JsonValue &operator=(const JsonValue &) = default;
    string to_string() const;
    /**
     * @brief 获取JSON值的类型
     * Get the type of JSON value
     * @return 值类型 Value type
     */
    Type getType() const
    {
        return static_cast<Type>(index());
    }
    string getTypeString()const
    {
    switch (getType())
    {
        case Type::String:
        return "string";
        case Type::Boolean:
        return "boolean";
        case Type::Float:
        return "float";
        case Type::Array:
        return "array";
        case Type::Object:
        return "object";
        case Type::Null:
        return "null";
        default:
        throw std::logic_error("never reach here");
    }

    }

private:
    /*
    template<typename Type,typename...Args>
    JsonValue(Type&& t,Args&&...args):v(t,std::forward<Args>(args)...) ;
    */
    using variant<string, bool, Accurate_Float, int, JsonArray, JsonObject, null>::variant;
};

#endif // JSONVALUE_HPP
