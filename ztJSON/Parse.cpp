#include "Parse.h"


namespace ztJSON {
	json_parse::json_parse(const std::string& s){

	}
	json json_parse::parse(const std::string& s, std::string& err) noexcept {
		try {
			json_parse parser(s);
			return parser.parse_value();
		}
		catch(std::runtime_error& err_mag){
			err = err_mag.what();
			json null;
			return null;
		}
	}
	json json_parse::parse(const std::string& s) noexcept {
		std::string err;
		return parse(s, err);
	}

	template <typename T>
	T print_err(std::string&& msg, const T msg_err) {
		if (!flag)
			err = std::move(msg);
		flag = true;
		return msg_err;
	}
	//以json形式返回输出错误
	json print_err(std::string&& msg) {
		return print_err(std::move(msg), json());
	}

#define SKIP_WTSPACE(){\
	while(str[i]==' '||str[i]=='\r'||str=='\n'||str=='\t')\
		i++;\
	}

	json json_parse::parse_string() {
		std::string ret;
		while (true) {
			if (i == str.size())
				return print_err("unexpected end of input in string", "");

		}
	}
	
}