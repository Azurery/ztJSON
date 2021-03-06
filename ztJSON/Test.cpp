﻿/*!
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

#include "Json.h"
#include "Parse.h"
#include <cassert>
#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <cstdio>

using namespace ztJSON;

	static int main_ret = 0;
	static int test_count = 0;
	static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)\
	do{\
		test_count++;\
		if(equality)\
			test_pass++;\
		else{\
			fprintf(stderr, "%s:%d: expect: " format ", actual: " format "\n", __FILE__, __LINE__, expect, actual);\
			main_ret = 1;\
		}\
	} while(0)

#define EXPECT_EQ_INT(expect, actual)		EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
#define EXPECT_EQ_DOUBLE(expect, actual)	EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")
#define EXPECT_TRUE(actual)					EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s")
#define EXPECT_FALSE(actual)				EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s")
#define EXPECT_EQ_STRING(expect, actual)\
		EXPECT_EQ_BASE(strcmp(expect, actual.data()) == 0, expect, actual.data(), "%s")

#if defined(_MSC_VER)
#define EXPECT_EQ_SIZE_T(expect, actual)	EXPECT_EQ_BASE((expect) == (actual), (size_t)expect, (size_t)actual, "%Iu")
#else
#define EXPECT_EQ_SIZE_T(expect, actual)	EXPECT_EQ_BASE((expect) == (actual), (size_t)expect, (size_t)actual, "%zu")
#endif

	static void test_parse_null() {
		auto ret = json_parse::parse("null");
		EXPECT_EQ_INT(json::json_type::ZT_NULL, ret.type());
	}
	
	static void test_parse_true() {
		EXPECT_EQ_INT(json::json_type::ZT_BOOL, json_parse::parse("true").type());
	}

	static void test_parse_false() {
		EXPECT_EQ_INT(json::json_type::ZT_BOOL, json_parse::parse("false").type());
	}

#define TEST_NUMBER(expect, input)\
	do{\
		auto& ret = json_parse::parse(input);\
		auto val = ret.double_value();\
		EXPECT_EQ_INT(json::json_type::ZT_NUMBER, (ret.type()));\
		EXPECT_EQ_DOUBLE(expect, val);\
	} while(0)

	static void test_parse_number() {
		TEST_NUMBER(0.0, "0");
		TEST_NUMBER(0.0, "-0");
		TEST_NUMBER(0.0, "-0.0");
		TEST_NUMBER(1.0, "1");
		TEST_NUMBER(-1.0, "-1");
		TEST_NUMBER(1.5, "1.5");
		TEST_NUMBER(-1.5, "-1.5");
		TEST_NUMBER(3.1415, "3.1415");
		TEST_NUMBER(1E10, "1E10");
		TEST_NUMBER(1e10, "1e10");
		TEST_NUMBER(1E+10, "1E+10");
		TEST_NUMBER(1E-10, "1E-10");
		TEST_NUMBER(-1E10, "-1E10");
		TEST_NUMBER(-1e10, "-1e10");
		TEST_NUMBER(-1E+10, "-1E+10");
		TEST_NUMBER(-1E-10, "-1E-10");
		TEST_NUMBER(1.2345E10, "1.2345E10");
		TEST_NUMBER(1.2345E-10, "1.2345E-10");

		TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
		TEST_NUMBER(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
		TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
		TEST_NUMBER(2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
		TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
		TEST_NUMBER(2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
		TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
		TEST_NUMBER(1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
		TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
	}

#define TEST_STRING(expect, input)\
	do{\
		auto ret = (json_parse::parse(input));\
		auto val = (ret.string_value());\
		size_t len = (val.size());\
		EXPECT_EQ_STRING(expect, val);\
	} while(0)

	//FIXME:
	static void test_parse_string() {
// 		TEST_STRING("abcd", R"("abcd")");
//  		TEST_STRING("", R"("")");
//  		TEST_STRING("hello world", R"("hello world")");
// 		TEST_STRING("\x24", R"("\u0024")");		//$
// 		TEST_STRING("\xC2\xA2", R"("\U00A2")");
		TEST_STRING("张", R"("\u5F20")");
//		std::cout << R"("\u0024")" << std::endl;
	}

	static void test_parse_array() {
		auto ret = json_parse::parse("[ ]");
		EXPECT_EQ_INT(json::json_type::ZT_ARRAY, (ret.type()));
		EXPECT_EQ_SIZE_T(0, ret.get_array_size());

		std::string  str = R"([false,true,123,"ABC"])";
		auto ret1 = json_parse::parse(str);
		EXPECT_EQ_INT(json::json_type::ZT_ARRAY, (ret1.type()));
		EXPECT_EQ_SIZE_T(4, ret1.get_array_size());
		EXPECT_EQ_INT(json::json_type::ZT_BOOL, ret1.get_array_element(0).type());
		EXPECT_EQ_INT(json::json_type::ZT_NUMBER, ret1.get_array_element(2).type());
		EXPECT_EQ_INT(json::json_type::ZT_STRING, ret1.get_array_element(3).type());
		EXPECT_EQ_INT(123.0, ret1.get_array_element(2).double_value());
		EXPECT_EQ_INT("ABC", ret1.get_array_element(3).string_value());
	
		std::string str1 = R"([[],[0],[0,1],[0,1,2]])";
		auto ret2 = json_parse::parse(str1);
		EXPECT_EQ_INT(json::json_type::ZT_ARRAY, ret2.type());
		EXPECT_EQ_SIZE_T(4, ret2.get_array_size());
		for (int i = 0; i < 4; i++) {
			auto iter = ret2.get_array_element(i);
			EXPECT_EQ_INT(json::json_type::ZT_ARRAY, iter.type());
			for (int j = 0; j < i; j++) {
				auto s = iter.get_array_element(j);
				EXPECT_EQ_INT(json::json_type::ZT_NUMBER, s.type());
				EXPECT_EQ_DOUBLE(j, s.double_value());
			}
		}
	}

	static void test_parse_object() {
		auto ret = json_parse::parse("{}");
		EXPECT_EQ_INT(json::json_type::ZT_OBJECT, ret.type());
		EXPECT_EQ_INT(0, ret.get_object_size());
	
		std::string str = 
			R"({"f":false,"t":true,"i":123,"s":"abcd","a":[1,2,3],"n":null,"o":{"1":1,"2":2,"3":3}})";
		auto ret1 = json_parse::parse(str);
		EXPECT_EQ_INT(json::json_type::ZT_OBJECT, ret.type());
		EXPECT_EQ_INT(7, ret1.get_object_size());
		EXPECT_EQ_STRING("n", (ret1.get_object_key("n")));
		EXPECT_EQ_STRING("f", (ret1.get_object_key("f")));
		EXPECT_EQ_INT(json::json_type::ZT_BOOL, ret1.get_object_value("f").type());
		EXPECT_EQ_STRING("t", (ret1.get_object_key("t")));
		EXPECT_EQ_INT(json::json_type::ZT_BOOL, ret1.get_object_value("t").type());
		
		EXPECT_EQ_STRING("i", (ret1.get_object_key("i")));
		EXPECT_EQ_INT(json::json_type::ZT_NUMBER, ret1.get_object_value("i").type());
		EXPECT_EQ_DOUBLE(123.0, (ret1.get_object_value("i").double_value()));

		EXPECT_EQ_STRING("s", ret1.get_object_key("s"));
		EXPECT_EQ_INT(json::json_type::ZT_STRING, ret1.get_object_value("s").type());
		EXPECT_EQ_STRING("abcd", ret1.get_object_value("s").string_value());

		EXPECT_EQ_STRING("a", ret1.get_object_key("a"));
		EXPECT_EQ_INT(json::json_type::ZT_ARRAY, ret1.get_object_value("a").type());
		EXPECT_EQ_SIZE_T(3, ret1.get_object_value("a").get_array_size());
		for (int i = 0; i < 3; i++) {
			auto result = ret1.get_object_value("a").get_array_element(i);
			EXPECT_EQ_INT(json::json_type::ZT_NUMBER, result.type());
			EXPECT_EQ_DOUBLE(i + 1.0, result.double_value());
		}
		EXPECT_EQ_STRING("o", ret1.get_object_key("o"));
		EXPECT_EQ_INT(json::json_type::ZT_OBJECT, ret1.get_object_value("o").type());
		for (int i = 1; i < 4; i++) {
			auto val = ret1.get_object_value("o").get_object_value(std::to_string(i));
			//EXPECT_EQ_SIZE_T(1, val.get_object_key_size(std::to_string(i)));
			EXPECT_EQ_INT(json::json_type::ZT_NUMBER, val.type());
			EXPECT_EQ_DOUBLE(i, val.double_value());
		}
		EXPECT_EQ_INT(json::json_type::ZT_NULL, ret1.get_object_value("n").type());
	}

	
		

	static void test_parse_comments() {
		std::string comment_test = R"({
      // comment /* with nested comment */
	"a": 1,
		// comment
		// continued
		"b" : "text",
		/* multi
		line
		comment
		// line-comment-inside-multiline-comment
		*/
		// and single-line comment
		// and single-line comment /* multiline inside single line */
		"c": [1, 2, 3]
		// and single-line comment at end of object
	})";
		auto ret = json_parse::parse(comment_test);
		EXPECT_EQ_INT(json::json_type::ZT_NUMBER, ret.get_object_value("a").type());
		EXPECT_EQ_INT(1, ret.get_object_value("a").int_value());
		EXPECT_EQ_INT(json::json_type::ZT_STRING, ret.get_object_value("b").type());
		EXPECT_EQ_STRING("text", ret.get_object_value("b").string_value());
		EXPECT_EQ_STRING("c", ret.get_object_key("c"));
		EXPECT_EQ_INT(json::json_type::ZT_ARRAY, ret.get_object_value("c").type());
		EXPECT_EQ_INT(1, ret.get_object_value("c").get_array_element(0).int_value());
		EXPECT_EQ_INT(2.0, ret.get_object_value("c").get_array_element(1).double_value());
	}



	static void test_parse() {
//  	test_parse_null();
//  	test_parse_true();
// 		test_parse_false();
// 		test_parse_number();
//		test_parse_string();
// 		test_parse_array();
// 		test_parse_object();
// 		test_parse_comments();
	}

	int main() {
		std::cout << std::boolalpha;
		test_parse();
		printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
	}

