#include "Parse.h"

namespace ztJSON {
	json json_parse::parse(const std::string& s, std::string& err) noexcept {
		try {
			json_parse parser(s);
			return parser.parse_value();
		}catch(std::runtime_error& err_mag){
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
	T json_parse::print_err(std::string&& msg, const T msg_err) {
		std::string err;
		if (!flag)
			err = std::move(msg);
		flag = true;
		return msg_err;
	}
	//以json形式返回输出错误
	json json_parse::print_err(std::string&& msg) {
		return print_err(std::move(msg), json());
	}

	void json_parse::skip_whitespace(){
		for (; str[i] == ' ' || str[i] == '\r' || str[i] == '\n' || str[i] == '\t'; ++i)
			;
	}
	void json_parse::skip_commits() {
		if (str[i] == '/') {
			i++;
			if (i = str.size())
				print_err("unexpected end of input");
			if (str[i] == '/') {
				i++;
				if (i = str.size())
					print_err("unexpected end of input");
				while (str[i] != '\n') {
					i++;
					if(i=str.size())
						print_err("unexpected end of input");
				}

			}
		}
		else if(str[i]=='*'){
			i++;
			if (i = str.size())
				print_err("unexpected end of input");
			while (str[i] != '*'&&str[i++] != '/') {
				i++;
				if (i>str.size() + 1)
					print_err("unexpected end of input");
			}
			i += 2;
			if (i = str.size())
				print_err("unexpected end of input");	
		}
		else {
			print_err("unexpected end of input");
		}
		
	}

	json json_parse::parse_value() {
		char ch = str[i++];
		if (ch == '-' || (ch >= '0'&&ch <= '9')) {
			i--;
			return parse_number();
		}

		if (ch == 't') {
			if (str.compare(0, 4, "true") == 0) {
				i += 4;
				return json_value::generate_true_instance();
			}
		}

		if (ch == 'f') {
			if (str.compare(0, 5, "false") == 0) {
				i += 5;
				return json_value::generate_false_instance();
			}
		}

		if (ch == 'n') {
			if (str.compare(0, 4, "null") == 0 ) {
				return json_value::generate_null_instance();
			}
		}
		
		if (ch == '"') {
			return parse_string();
		}

		//if (ch == '{') {
		//	std::map<std::string, json> data;
		//	ch = str[i++];
		//	if (ch == '}') return data;

		//	while (true) {
		//		if (ch != '"') {

		//		}
		//	}
		//}
	}
	json json_parse::parse_null() {
		if (strncmp(str.c_str(), "null", 4) == 0) {
			i += 4;
			json null;
			return null;
		}
		print_err("Expected string null error");
	}
	std::string json_parse::parse_string() {
		skip_whitespace();
		//++i;
		if (str[i] == '\"') {
			++i;
			return {};
		}
		std::string ret;
		for (; i < str.size(); ++i) {
			char ch = str[i];
			if (ch == '\\') {
				++i;
				switch (str[i]) {
				case '\"':
					++i;
					ret.push_back('\"');
					continue;
				case '\\':
					++i;
					ret.push_back('\\');
					continue;
				case '/':
					++i;
					ret.push_back('/');
					continue;
				case 'b':
					++i;
					ret.push_back('\b');
					continue;
				case 'f':
					++i;
					ret.push_back('\f');
					continue;
				case 't':
					++i;
					ret.push_back('\t');
					continue;
				case 'r':
					++i;
					ret.push_back('\r');
					continue;
				default:;
				}
			}
			if (str[i] == '\"') {
				++i;
				return ret;
			}else {
				ret.push_back(str[i]);
			}
		}
		return print_err("invalid input","");
	}

	json json_parse::parse_object() {
		skip_whitespace();
		assert(str[i] == '{');
		++i;
		json::object obj;
		skip_whitespace();
		if (str[i] == '}') {
			++i;
			return obj;
		}
		for (; i < str.size(); ++i) {
			auto temp = parse_string();
			if (obj.find(temp) != obj.end()) {
				print_err("Duplicated key in the object");
			}
			skip_whitespace();
			if (str[i] != ':') {
				print_err("Expected ':' error");
			}
			++i;
			auto value = parse_value();
			//构造一个object对象，并将其压入object对象之中
			obj.emplace(std::move(temp), std::move(value));
			skip_whitespace();
			if (str[i] == ',') {
				++i;
			}
			else if (str[i] == '}') {
				++i;
				return obj;
			}
			else
				print_err("Expected '}");
		}
	}
	json json_parse::parse_number() {
		skip_whitespace();
		size_t start_pos = i;
		if (str[i] == '-')
			++i;
		if (str[i] == '0') {
			++i;
			if (str[i] >= '0'&&str[i] <= '9') {
				print_err("leading 0 is not allowed");
			}
		}else if (str[i] >= '1'&&str[i] <= '9') {	//当为数字1-9时
			++i;
			while (isdigit(str[i]))
				++i;
		}else {
			print_err("invalid number " + str[i]);
		}
	
		if (str[i] != '.'&&str[i] != 'e'&&str[i] != 'E' && (i - start_pos) <= static_cast<size_t>(std::numeric_limits<int>::digits10)){
			return std::atoi(str.c_str() + start_pos);
		}
		
		//小数点部分
		if (str[i] == '.') {
			++i;
			if (!isdigit(str[i])/*(str[i] >= '0'&&str[i] <= '9')*/) {
				return print_err("Unexpected digit at position");
				//std::cout << "unexpected digit at position" << std::endl;
			}
			while (isdigit(str[i])/*str[i] >= '0'&&str[i] <= '9'*/) 
				++i;
		}
		//指数部分
		if (str[i] == 'e' || str[i] == 'E') {
			++i;
			if (str[i] == '-' || str[i] == '+')
				++i;
			if (!isdigit(str[i]))
				return print_err("Unexcepted digit at position" + i);
			while (isdigit(str[i]))
				++i;
		}
		//std::cout << std::strtod(str.c_str() + start_pos, nullptr);
		return std::strtod(str.c_str() + start_pos, nullptr);
	}
	
	json json_parse::parse_array() {
		skip_whitespace();
		json::array container;
		assert(str[i] == '[');
		++i;
		skip_whitespace();
		if (str[i] == ']') {
			++i;
			return container;
		}
		while (str[i]) {
			container.push_back(parse_value());
			skip_whitespace();
			if (str[i] == ',')
				++i;
			else if (str[i] == ']') {
				++i;
				return container;
			}else {
				print_err("Error");
			}
		}
	}
	json json_parse::parse_boolean() {
		skip_whitespace();
		/*if (strncmp(str.c_str(), "true", 4) == 0) {
			i += 4;
			return true;
		}
		if (strncmp(str.c_str(), "false", 5) == 0) {
			i += 5;
			return false;
		}*/
		return print_err("Excepted true or false error");
	}
}