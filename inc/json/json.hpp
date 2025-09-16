#pragma once
#ifndef JSON_HPP
#define JSON_HPP
#include "jsonobject.hpp"
#include "jsonvalue.hpp"
#include "functional"
namespace simple_json
{
JsonObject parse(std::filesystem::path const& path);
void dump(JsonObject const& obj, std::filesystem::path const& path);



};


#endif // JSON_HPP