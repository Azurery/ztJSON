#pragma once
#include "Json.h"
namespace ztJSON {

	class json_parse {
	public:
		json parse(const std::string& s) noexcept;
	private:
		json_parse(const std::string& s);
		json parse_value();
		json parse_boolean();
		json parse_number();
		json parse_array();
		json parse_object();
		json parse_string();

	};
}
