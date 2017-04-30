#pragma once
#include "Json.h"
namespace ztJSON {

	class json_parse {
	public:
		json parse(const std::string& s) noexcept;
		json parse(const std::string& s, std::string& err) noexcept;
	private:
		//std::string err;
		json_parse(const std::string& s);
		json parse_value();
		json parse_null();
		json parse_boolean();
		json parse_number();
		json parse_array();
		json parse_object();
		std::string parse_string();
		void skip_whitespace();
	private:
		bool flag;
		const std::string& str;
		size_t i;
	};
}
