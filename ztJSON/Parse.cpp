#include "Parse.h"


namespace ztJSON {
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

	void json_parse::skip_whitespace(){
		for (; str[i] == ' ' || str[i] == '\r' || str[i] == '\n' || str[i] == '\t'; ++i)
			;
	}

	json json_parse::parse_string() {
		skip_whitespace();
		++i;
		if (str[i] == '\"') {
			++i;
			return {};
		}
		std::string container;
		for (; i < str.size(); ++i) {
			char ch = str[i];
			if (ch == '\\') {
				++i;
				switch (str[i]) {
				case '\"':
					++i;
					container.push_back('\"');
					continue;
				case '\\':
					++i;
					container.push_back('\\');
					continue;
				case '/':
					++i;
					container.push_back('/');
					continue;
				case 'b':
					++i;
					container.push_back('\b');
					continue;
				case 'f':
					++i;
					container.push_back('\f');
					continue;
				case 't':
					++i;
					container.push_back('\t');
					continue;
				case 'r':
					++i;
					container.push_back('\r');
					continue;
				default:;
				}
			}
			if (str[i] == '\"') {
				++i;
				return container;
			}else {
				container.push_back(str[i]);
				++i;
			}
		}
		return print_err("invalid input","");
	}
	json json_parse::parse_object() {
		skip_whitespace();
	}
	json json_parse::parse_number() {
		skip_whitespace();
		size_t start_pos = i;
		if(str[i]=='-')
			++i;
		if (str[i] == '0')
			++i;
		else if (isdigit(str[i])) {	//当为数字1-9时
			++i;
			while (isdigit(str[i]))
				++i;
		}else {
			print_err("invalid number " + str[i]);
		}
		//小数点部分
		if (str[i] == '.') {
			++i;
			if (!isdigit(str[i])) {
				print_err("Unexpected digit at position");
			}
			while (isdigit(str[i]))
				++i;
		}
		//指数部分
		if (str[i] == 'e' || str[i] == 'E') {

		}
	}
	
}