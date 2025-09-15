
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

using std::string;
using std::variant;
using std::vector;
using std::span;
#include "utf8handler.hpp"
#include "jsonobject.hpp"
struct JsonValue
{
    variant<string, bool, double, int, JsonArray, JsonObject, null> v;
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
    Type getType() const
    {
       return static_cast<Type>(v.index());
    }

    JsonValue & operator=(JsonValue&& )=default;
    JsonValue (const JsonValue& )=default;
    JsonValue(JsonValue&& )=default;


    JsonValue (UnicodeStringView decoded_str);
    JsonValue (UnicodeStringView::const_iterator iter);
    static Type getType(UnicodeStringView::const_iterator iter);
    UnicodeStringView get_Valuerange(UnicodeStringView::const_iterator iter);

    JsonValue & operator=(const JsonValue& )=default;
    JsonValue() {
        v = null();
    }
    
};

#endif // JSONVALUE_HPP