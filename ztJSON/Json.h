#ifndef _ZT_JSON_H
#define _ZT_JSON_H

#include <string>
#include <map>
#include <vector>

namespace ztJSON {
	class json final {
		//JSON的六种类型
		enum type {
			ZT_NULL,
			ZT_FALSE,
			ZT_TRUE,
			ZT_NUMBER,
			ZT_STRING,
			ZT_OBJECT
		};
		//array:用于存储JSON数据的vector
		typedef std::vector<json> array;
		//object:用于存储JSON对象
		typedef std::map<std::string, json> object;

		//各种不同类型的JSON构造器
		//NULL
		json() noexcept;	
		//NUMBER
		json(double value);	
		json(int value);
		//STRING
		json(const std::string& value);
		//ARRAY
		json(const array& values);
		//OBJECT
		json(const object& values);
	};
}

#endif
