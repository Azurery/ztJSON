#include "Json.h"
namespace ztJSON {

	class json_parse {
	public:
		json static parse(const std::string& s) noexcept;
		json static parse(const std::string& s, std::string& err) noexcept;
		template <typename T> T print_err(std::string&& msg, const T msg_err);
		json print_err(std::string&& msg);
	private:
		//std::string err;
		json_parse(const std::string& s)
			:str(s), i(0) {}
		json parse_value();
		json parse_null();
		json parse_boolean();
		json parse_number();
		json parse_array();
		json parse_object();
		std::string parse_string();
		void skip_whitespace();
		void skip_commits();
	private:
		bool flag;
		const std::string& str;
		size_t i;
		
	};
}
