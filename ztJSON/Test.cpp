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

/*
*如果将ZTJSON_TEST_CUSTOM_CONFIG定义为1，则可以将你自己的tester集成
*到此测试之中。
*/

// #if !ZTJSON_TEST_CUSTOM_CONFIG
// #define ZTJSON_TEST_CPP_PREFIX_CODE
// #define ZTJSON_TEST_CPP_SUFFIX_CODE
// #define ZTJSON_TEST_CASE(name) static void name()
// #define ZTJSON_TEST_ASSERT(b) assert(b)
// #ifdef DEBUG
// #undef DEBUG
// #endif
// #endif


#include "Json.h"
#include "Parse.h"
#include <cassert>
#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <cstdio>

//在此处可以insert自定义的prefix code（includes，function，declarations...）
//以满足your own requestments
/*ZTJSON_TEST_CPP_PREFIX_CODE*/

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

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")
#define EXPECT_EQ_STRING(expect, actual, length)\
		EXPECT_EQ_BASE(sizeof(expect) == length && memcmp(expect, actual, length) ==0, expect, actual, "%s")
#define EXPECT_TRUE(actual) EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s")
#define EXPECT_FALSE(actual) EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s")

	static void test_parse_null() {
		EXPECT_EQ_INT(json::json_type::ZT_BOOL, json_parse::parse("false").type());
	}

// 	ZTJSON_TEST_CASE(ztjson_test) {
// 		test_parse_null();
// 	}

	static void test_parse() {
		test_parse_null();
	}

	int main() {
		std::cout << std::boolalpha;
		test_parse_null ();
		//EXPECT_EQ_INT(json::json_type::ZT_NULL, json_parse::parse("null").type());
		/*printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);*/
		return 0;
	}


