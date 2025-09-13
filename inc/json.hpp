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
using std::string;
using std::variant;
using std::vector;
struct JsonObject;
struct JsonValue;

using JsonArray = vector<JsonValue>;
using null = std::monostate;

class JsonObject
{
    std::unordered_map<string, JsonValue> obj;
    protected:
    vector<u_int32_t> get_decoded_json_file(std::filesystem::path const& path);
    bool is_whitespace(u_int32_t c)const {return c==' ' || c=='\t' || c=='\n' || c=='\r';}

    //when failure ,iter wont be moved
    std::optional <string> get_key(vector<u_int32_t>::const_iterator & it);
    std::optional<JsonValue> parse_value(vector<u_int32_t>::const_iterator & it);
    bool expect_left_brace(vector<u_int32_t>::const_iterator & it);
    bool expect_right_brace(vector<u_int32_t>::const_iterator & it);
    bool expect_comma(vector<u_int32_t>::const_iterator & it);
    bool expect_colon(vector<u_int32_t>::const_iterator & it);
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
    JsonValue & operator=(const JsonValue& )=default;
    JsonValue() {
        v = null();
    }
    
};

#endif // JSON_HPP