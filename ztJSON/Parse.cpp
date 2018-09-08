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

	uint32_t json_parse::parse_hex(const std::string& from) {
		uint32_t ret = 0;
		for (int i = 0; i < 4; i++) {
			char ch = from[i];
			assert((ch >= '0'&&ch <= '9')
				|| (ch >= 'a'&&ch <= 'f')
				|| (ch >= 'A'&&ch <= 'F'));
			ret <<= 4;
			if (ch >= '0'&&ch <= '9')
				ret |= ch - '0';
			else if (ch >= 'A'&&ch <= 'F')
				ret |= ch - ('A' - 10);
			else if (ch >= 'a'&&ch <= 'f')
				ret |= ch - ('a' - 10);
		}
		return ret;
	}

	//************************************
	// \method name:    encode_utf8
	// \parameter:		uint32_t c			输入的字符
	// \parameter:		std::string & ret	经过UTF-8编码后的结果
	//
	// \brief:			主要用于处理\uXXXX这种转义序列
	//************************************

// 	码点范围			码点位数		 字节1		 字节2        字节3       字节4
// 	U+0000 ~ U+0007F   7		0xxxxxxx
// 	U+0080 ~ U+07FF	   11		110xxxxx    10xxxxxx
// 	U+0800 ~ U+FFFF	   16		1110xxxx    10xxxxxx     10xxxxxx
// 	U+10000 ~ U+10FFFF 21		11110xxx    10xxxxxx     10xxxxxx    10xxxxxx
// 	
	void json_parse::encode_utf8(uint32_t code_point, std::string& ret) {
		if (code_point <= 0x7F) {
			ret += static_cast<char>(code_point);
		} else if (code_point <= 0x7FF) {
			//0xC0 ===> 1100 0000
			//0x80 ===> 1000 0000
			//0x3F ===> 0011 1111
			 
			//U+0080 ~ U+07FF 
			ret += static_cast<char>(0xC0 | ((code_point >> 6) & 0xFF));	  //将字节1进行补全	
			ret += static_cast<char>(0x80 | (code_point & 0x3F));	      //c & 0x3F ===> 获取c的低6位，再|0x80 ====> 得到完整的字节2
		} else if (code_point <= 0xFFFF) {
			ret += static_cast<char>(0xE0 | ((code_point >> 12) & 0xFF));
			ret += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
			ret += static_cast<char>(0x80 | (code_point & 0x3F));
		} else {
			//assert(code_point <= 0x10FFFF && "the code out of the range");
			ret += static_cast<char>(0xF0 | ((code_point >> 18) & 0xFF));
			ret += static_cast<char>(0x80 | ((code_point >> 12) & 0xFF));
			ret += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
			ret += static_cast<char>(0x80 | (code_point & 0x3F));
		}
	}

	json json_parse::parse_value() {
		char ch = str[i];
		if (ch == '-' || (ch >= '0'&&ch <= '9')) {
			return parse_number();
		}

		if (ch == 't') {
			if (str.compare(i, 4, "true") == 0) {
				i += 4;
				return json_value::generate_true_instance();
			}
		}

		if (ch == 'f') {
			if (str.compare(i, 5, "false") == 0) {
				i += 5;
				return json_value::generate_false_instance();
			}
		}

		if (ch == 'n') {
			if (str.compare(i, 4, "null") == 0 ) {
				i += 4;
				return json_value::generate_null_instance();
			}
		}
		
		if (ch == '"') {
			return parse_string();
		}

		if (ch == '{') {
			return parse_object();
		}

		if (ch = '[') {
			return parse_array();
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
		std::string ret;
		long code_point = -1;
// 		for (;;) {
// 			if (i == str.size())
// 				return print_err("incorrect input in string", "");
// 			char ch = str[i++];
// 			if (ch == '"') {
// 				encode_utf8(code_point, ret);
// 				return ret;
// 			}
// 
// 			//ASCII处于0~31范围内的字符是非法的
// 			if (ch >= 0 && ch <= 0x1f) {
// 				return print_err("unescaped " + std::to_string(ch) + " in string", "");
// 			}
// 
// 			// 普通的ASCII码值，则不需要进行转换
// 			if (ch != '\\') {
// 				encode_utf8(code_point, ret);
// 				code_point = -1;
// 				ret.push_back(ch);
// 				continue;
// 			}
// 
// 			if (i == str.size())
// 				return print_err("incorrect input in string", "");
// 
// 			ch = str[i++];
// 			if (ch == 'u') {
// 				std::string high_seq = str.substr(i, 4);
// 				if (high_seq.length() < 4)
// 					return print_err("bad \\u escape: " + std::to_string(ch), "");
// 				
// 				uint32_t high_surrogate = parse_hex(high_seq);
// 				if (code_point >= 0xD800 && code_point <= 0xD8FF) {
// 					if (str[i++] != '\\') {
// 						print_err("parse invalid unicode surrogate");
// 					}
// 					if (str[i++] != 'u') {
// 						print_err("parse invalid unicode surrogate");
// 					}
// 
// 					std::string low_seq = str.substr(i, 4);
// 					long low_surrogate = parse_hex(low_seq);
// 					if (low_surrogate > 0xDFFF || low_surrogate < 0xDC00) {
// 						print_err("parse invalid unicode surrogate");
// 					}
// 					code_point = 0x10000 + (((high_surrogate - 0xD800) << 10) | (low_surrogate - 0xDC00));
// 					encode_utf8(code_point, ret);
// 				}
// 				i += 4;
// 				continue;
// 			}
// 		}
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
		while(true) {
			if (str[i] != '"') {
				return print_err("expected '\"' in object, got " + str[i]);
			}
			std::string key = parse_string();
			if (obj.find(key) != obj.end()) {
				print_err("Duplicated key in the object");
			}
			skip_whitespace();
			if (str[i] != ':') {
				print_err("Expected ':' in object, but got "+str[i]);
			}
			++i;
			auto value = parse_value();
			//构造一个object对象，并将其压入object对象之中
			obj.emplace(std::move(key), std::move(value));
			skip_whitespace();
			//++i;
			if (str[i] == ',') {
				/*return print_err("expected ',' in object, but got " + str[i]);*/
				++i;
			}else if (str[i] == '}') {
				//++i;
				return obj;
			}else
				print_err("Expected '}");
		}
		print_err("unexpected end of input");
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
	
//	array = %x5B ws[value *(ws %x2C ws value)] ws %x5D


	//************************************
	// \method name:    parse_array
	// \returns:   		ztJSON::json
	//
	// \brief:			
	//************************************
	json json_parse::parse_array() {
		skip_whitespace();
		json::array arr;
		assert(str[i] == '[');
		++i;
		skip_whitespace();
		if (str[i] == ']') {
			++i;
			return arr;
		}
		while (str[i]) {
			arr.push_back(parse_value());
			skip_whitespace();
			if (str[i] == ',')
				++i;
			else if (str[i] == ']') {
				++i;
				return arr;
			}else {
				print_err("Error");
			}
		}
		print_err("unexpectd end of input");
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