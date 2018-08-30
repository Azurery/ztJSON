/*!
 * \file   Test.cpp
 *
 * \author Magicmanoooo
 * \mail   zt838713968@gmail.com
 * \date   2018/08/30 
 *
 * \brief  主要用于测试
 *
 *
 * // Copyright (c) 2018 The ztJSON Authors. All rights reserved.
 * // Use of this source code is governed by a BSD-style license that can be
 * // found in the LICENSE file. See the AUTHORS file for names of contributors.
 * 
 */

#if !ZTJSON_TEST_CONFIG
#define ZTJSON_TEST_CASE(name) static void name()
#define ZTJSON_TEST_ASSERT(b) assert(b)
#ifdef DEBUG
#undef DEBUG
#endif
#endif

#include "Json.h"
#include "Parse.h"
#include <cassert>
#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace ztJSON;



ZTJSON_TEST_CASE(ztjson_test) {
	///test string
	//std::string s1 = R"("   abcd")";
	//auto _1 = json_parse::parse(s1);
	//std::cout << std::boolalpha;
	//std::cout << _1.is_null() << std::endl;
	//std::cout << _1.is_bool() << std::endl;
	//std::cout << _1.is_array() << std::endl;
	//std::cout << _1.is_number() << std::endl;
	//std::cout << _1.is_string() << std::endl;
	//std::cout << _1.string_value() << std::endl;

	///test int
	//std::string s2 = R"(12345)";
	//auto _2 = json_parse::parse(s2);
	//std::cout << std::boolalpha;
	//std::cout << _2.is_null() << std::endl;
	//std::cout << _2.is_bool() << std::endl;
	//std::cout << _2.is_array() << std::endl;
	//std::cout << _2.is_number() << std::endl;
	//std::cout << _2.is_string() << std::endl;
	//std::cout << _2.int_value() << std::endl;
	//assert(_2.type() == json::ZT_NUMBER);

	///test double
	//std::string s2 = R"(12345)";
	//auto _2 = json_parse::parse(s2);
	//std::cout << std::boolalpha;
	//std::cout << _2.is_null() << std::endl;
	//std::cout << _2.is_bool() << std::endl;
	//std::cout << _2.is_array() << std::endl;
	//std::cout << _2.is_number() << std::endl;
	//std::cout << _2.is_string() << std::endl;
	//std::cout << _2.int_value() << std::endl;
	//assert(_2.type() == json::ZT_NUMBER);
	

	///test bool
	std::string s4 = R"(false)";
	auto _4 = json_parse::parse(s4);
	std::cout << std::boolalpha;
	std::cout << _4.is_null() << std::endl;
	std::cout << _4.is_bool() << std::endl;
	std::cout << _4.is_array() << std::endl;
	std::cout << _4.is_number() << std::endl;
	std::cout << _4.is_string() << std::endl;
	std::cout << _4.bool_value() << std::endl;
	assert(_4.type() == json::ZT_BOOL);

	///test array
	/*std::string s4 = R"(false)";
	auto _4 = json_parse::parse(s4);
	std::cout << std::boolalpha;
	std::cout << _4.is_null() << std::endl;
	std::cout << _4.is_bool() << std::endl;
	std::cout << _4.is_array() << std::endl;
	std::cout << _4.is_number() << std::endl;
	std::cout << _4.is_string() << std::endl;
	std::cout << _4.bool_value() << std::endl;*/
	//assert(_4.type() == json::ZT_BOOL);

	/*const std::string s5 = R"("k1":"v1")";
	std::string err;
	const auto json_ = json_parse::parse(s5, err);

	std::cout << "k1: " << json_["k1"].string_value() << "\n";*/
}

int main() {
	ztjson_test();
}


