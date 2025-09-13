#ifndef JSON_HPP
#define JSON_HPP
#pragma once
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
struct JsonObject;
struct JsonValue;

using JsonArray = vector<JsonValue>;
using null = std::monostate;
 inline bool is_whitespace(u_int32_t c) {return c==' ' || c=='\t' || c=='\n' || c=='\r';}
  bool expect_char(span<u_int32_t>::const_iterator & it, u_int32_t c);
    bool expect_right_brace_with_embedded_condition(u_int32_t c, span<u_int32_t>::const_iterator  it);
     bool expect_right_bracket_with_embedded_condition(u_int32_t c, span<u_int32_t>::const_iterator  it);
   
class JsonObject
{
    std::unordered_map<string, JsonValue> obj;
    protected:
    vector<u_int32_t> get_decoded_json_file(std::filesystem::path const& path);
  

    //when failure ,iter wont be moved
    std::optional <string> get_key(span<u_int32_t>::const_iterator & it);
    std::optional<JsonValue> parse_value(span<u_int32_t>::const_iterator & it);

   JsonObject( std::span<uint32_t> decoded_str);
    public:
    JsonObject() = default;
    JsonObject(std::filesystem::path const& path);
    JsonObject(JsonObject&& )=default;
    JsonObject(const JsonObject& )=default;
    JsonObject& operator=(JsonObject&& )=default;
    JsonObject& operator=(const JsonObject& )=default;

    JsonValue & operator[](string&& key);
    void dump(std::filesystem::path const& path);
    static JsonObject parse(std::filesystem::path const& path)
    {
        return std::move(JsonObject(path));
    }
};

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


    JsonValue (std::span<uint32_t> decoded_str);
    JsonValue (span<uint32_t>::const_iterator iter);
    static Type getType(std::span<uint32_t>::const_iterator iter);
    std::span<uint32_t> get_Valuerange(std::span<uint32_t>::const_iterator iter);

    JsonValue & operator=(const JsonValue& )=default;
    JsonValue() {
        v = null();
    }
    
};

#endif // JSON_HPP