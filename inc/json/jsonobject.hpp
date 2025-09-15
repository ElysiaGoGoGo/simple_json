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

using std::string;
using std::variant;
using std::vector;
using std::span;
#include "utf8handler.hpp"


struct JsonValue;
using JsonArray = vector<JsonValue>;
using null = std::monostate;

class JsonObjectBuilder;

using JsonObject=std::unordered_map<string, JsonValue> ;
using UnicodeType=uint32_t;
using UnicodeString=std::vector<UnicodeType>;
using UnicodeStringView=std::span<UnicodeType>;
using UnicodeStringViewIterator=UnicodeStringView::const_iterator;
namespace fs=std::filesystem ;
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
    bool expect_char(UnicodeStringViewIterator &it, u_int32_t c)const;
    std::optional<string> get_key(UnicodeStringViewIterator &it)const;
    std::optional<JsonValue> parse_value(UnicodeStringViewIterator &it)const;

    };
    std::vector<BuildCache> build_cache;
    std::vector<std::unique_ptr<JsonObject>> product_cache;
    size_t available_version_count=0;
    
    
    size_t build_with_cache(size_t version);

    public:
        bool reset(bool forcereset=false);
        static constexpr size_t ERROR =std::numeric_limits<size_t>::max();
        size_t build_from(std::filesystem::path const& path);
        size_t build_from(UnicodeStringView json_str);
        size_t build_from(size_t old_version);
        void rebuild(size_t version);
        std::optional<JsonObject> get_product(size_t version);
};

    


#endif // JSONOBJECT_HPP