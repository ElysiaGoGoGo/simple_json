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
using JsonArray = vector<JsonValue>;
using null = std::monostate;

class JsonObjectBuilder;

using JsonObject = std::unordered_map<string, JsonValue>;
using UnicodeType = uint32_t;
using UnicodeString = std::vector<UnicodeType>;
using UnicodeStringView = std::span<const UnicodeType>;
using UnicodeStringViewIterator = UnicodeStringView::iterator;
namespace fs = std::filesystem;
/*
给定参数可以生成jsonobject,生成失败则cache存放空对象
build_from 总是生成新的cache和对象
如果所有product已经空了，或者设置了force为true，则reset将所有cache与product丢弃
rebuild基于原来的cache,在原来cahce对应放置产品的位置放置产生的新产品
get_product返回对应版本的product,如果版本不存在则返回空对象，且解除product_cache对其的引用



*/

class JsonObjectBuilder
{
    struct BuildCache
    {
        UnicodeString json_file_str;
        UnicodeStringView json_obj_str;
        void expect_char(UnicodeStringViewIterator &it, u_int32_t c) const;
        bool expect_comma_or_right_brace(UnicodeStringViewIterator &it,bool is_comma_expected) const;
        string get_key(UnicodeStringViewIterator &it) const;
        JsonValue parse_value(UnicodeStringViewIterator &it) const;
    };
    BuildCache build_cache;
    mutable std::optional<JsonObject> product_cache;

    bool build_with_cache() const;

public:
    bool reset(bool forcereset = false);
    void build_from(std::filesystem::path const &path);
    void build_from(UnicodeStringView json_str);
    std::optional<JsonObject> get_product();
};

#endif // JSONOBJECT_HPP