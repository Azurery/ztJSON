#ifdef DEBUG
#undef DEBUG
#endif

#include "Json.h"
#include "Parse.h"
#include <cassert>
#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include <algorithm>
#include "stdafx.h"
using namespace ztJSON;
#define JSON_TEST_ASSERT(b) assert(b); 
#define JSON_TEST_CASE(name) static void name()
#define ASSERT_TRAITS(x) static_assert(std::x::value,#x)
	ASSERT_TRAITS(is_nothrow_constructible<json>);
	ASSERT_TRAITS(is_nothrow_default_constructible<json>);
	ASSERT_TRAITS(is_copy_constructible<json>);
	ASSERT_TRAITS(is_nothrow_move_constructible<json>);
	ASSERT_TRAITS(is_copy_assignable<json>);
	ASSERT_TRAITS(is_nothrow_move_assignable<json>);
	ASSERT_TRAITS(is_nothrow_destructible<json>);

	JSON_TEST_CASE(json_test)
	{
		const std::string sample =
			R"({"key1":"value1", "key2":42, "key3":["hello",123,true,false,null]})";
		std::string err;
		auto json = json_parse::parse(sample, err);
		//std::cout << "key1: " << json["key1"].string_value << "\n";
		std::cout << "key3: " << json["key3"].serialize() << "\n";
		for (auto& item : json["key3"].array_value()) {
			std::cout << " - " << item.serialize();
		}

		std::list<int> l1{ 1, 2, 3 };
		std::vector<int> l2{ 1, 2, 3 };
		std::set<int> l3{ 1, 2, 3 };
		JSON_TEST_ASSERT(json(l1) == json(l2));
		JSON_TEST_ASSERT(json(l2) == json(l3));
	}

	int main(int argc, char** argv) {
		json_test();
		return 0;
		
	}


