#ifndef _ZT_JSON_H
#define _ZT_JSON_H

#include <string>
#include <map>
#include <vector>

namespace ztJSON {
	class json final {
		//JSON的六种类型
		enum json_type {
			ZT_NULL,
			ZT_FALSE,
			ZT_TRUE,
			ZT_NUMBER,
			ZT_STRING,
			ZT_OBJECT
		};
		//ARRAY类型
		using array=std::vector<json>;
		//OBJECT类型
		using object=std::map<std::string, json>;

		//构造函数
		json() noexcept;	//NUL类型
		json(int value);	//NUMBER
		json(double vlaue); //NUMBER
		json(bool value);	//BOOL
		json(const std::string& value);	//SRING
		json(std::string&& value);
		json(const char* value);
		json(const array& value);	//ARRAY
		json(array&& value);
		json(const object& value);	//OBJECT
		json(object&& value);

		json_type type() const;
		bool is_null() const { return type() == json_type::ZT_NULL; }
		bool is_false() const { return type() == json_type::ZT_FALSE; }

	};
}

#endif
