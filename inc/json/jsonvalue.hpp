
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

using std::span;
using std::string;
using std::variant;
using std::vector;
#include "utf8handler.hpp"
#include "jsonobject.hpp"
using JsonRawValueType =variant<string, bool, double, int, JsonArray, JsonObject, null>;
struct JsonValue : variant<string, bool, double, int, JsonArray, JsonObject, null>
{

    enum class Type
    {
        String,
        Boolean,
        Float,
        Integer,
        Array,
        Object,
        Null
    };

    class JsonValueBuilder
    {
        UnicodeStringView view_to_build_from;

    public:
        JsonValueBuilder(UnicodeStringView str) : view_to_build_from(str) {}
        JsonValue build();
        Type getType();
        UnicodeStringView get_Value_range(Type type);
    };
    JsonValue &operator=(JsonValue &&) = default;
    JsonValue(const JsonValue &) = default;
    JsonValue(JsonValue &&) = default;
    JsonValue &operator=(const JsonValue &) = default;
    Type getType() const
    {
        return static_cast<Type>(index());
    }


private:
    /*
    template<typename Type,typename...Args>
    JsonValue(Type&& t,Args&&...args):v(t,std::forward<Args>(args)...) ;
    */
    using variant<string, bool, double, int, JsonArray, JsonObject, null>::  variant;
};

#endif // JSONVALUE_HPP