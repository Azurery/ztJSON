#include "Parse.h"
#include "stdafx.h"

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
			++i;
			if (str[i] == '-' || str[i] == '+')
				++i;
			if (!isdigit(str[i]))
				print_err("Unexcepted digit at position" + i);
			while (isdigit(str[i]))
				++i;
		}
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
		if (strncmp(str.c_str(), "true", 4) == 0) {
			i += 4;
			return true;
		}
		if (strncmp(str.c_str(), "false", 5) == 0) {
			i += 5;
			return false;
		}
		print_err("Excepted true or false error");
	}
}