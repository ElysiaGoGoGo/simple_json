
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
          Type getType()const;
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
        UnicodeStringView get_Value_range(Type type);
        UnicodeStringViewIterator expect_right_brace_or_right_bracket_with_embedded_condition( bool is_right_brace_expected);
             tuple <string,UnicodeStringViewIterator> get_string_value_and_end() const;

    public:
        JsonValueBuilder(UnicodeStringView str) : view_to_build_from(str) {}
   std::tuple<JsonValue,UnicodeStringViewIterator> build();
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