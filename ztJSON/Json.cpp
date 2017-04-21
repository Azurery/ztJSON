#include "Json.h"
#include <string>
namespace ztJSON {
	static const int MAX_DEPTH = 200;	//��ʾ���ɴ����Ƕ�׵����
	
	//json::json():ptr(nullptr){}
	//json::json(int value) :ptr(std::make_shared<json_int>(value)){}
	//json::json(double value):ptr(std::make_shared<json_double>()){}
	json::json() noexcept : json() {}
	json::json(int value) :json(){
		new json_int(value);
	}
	json::json(double value) : json() {
		new json_double(value);
	}
	json::json(const std::string& value) : json() {
		new json_string(value);
	}	
	json::json(std::string&& value) : json() {
		json::json(std::move(value));
	}
	json::json(bool value) : json() {
		new json_boolean(value);
	}
	json::json(const char* value) : json() {
		new json_string(value);
	}
	json::json(const array& value) : json() {
		new json_array(value);
	}
	json::json(array&& value) : json() {
		json::json(std::move(value));
	}
	json::json(const object& value) : json() {
		new json_object(value);
	}
	json::json(object&& value) : json() {
		json::json(std::move(value));
	}

	int json::int_value() const { 
		return ptr->get_int_value; 
	}
	double json::double_value() const { 
		return ptr->get_double_value;
	}
	bool json::bool_value() const { 
		return ptr->get_bool_value; 
	}
	std::string& json::string_value() const { 
		return ptr->get_string_value; 
	}
	json::array& json::array_value() const { 
		return ptr->get_array_value; 
	}
	json::object& json::object_value() const { 
		return ptr->get_object_vlaue; 
	}

	const int json_value::get_int_value() const {
		return 0; 
	}
	const double json_value::get_double_value() const { 
		return 0; 
	}
	const bool json_value::get_bool_value() const { 
		return false; 
	}
	const std::string& json_value::get_string_value() const { 
		return std::string(); 
	}
	const json::array& json_value::get_array_value() const { 
		return std::vector<json>(); 
	}
	const json::object& json_value::get_object_vlaue() const { 
		return std::map<std::string, json>(); 
	}
	const json& json_value::operator[](size_t i) const { 
		return json(); 
	}
	const json& json_value::operator[](std::string& key) const { 
		return json(); 
	
	}
	
	//���л�����һ��������Json��ʽ���ַ���
	static void serialize(int val,std::string& ret) {
		if (isdigit(val)) { //���ڼ�����val�Ƿ�Ϊ����������0��9��
			char buf[32];
			snprintf(buf, sizeof(buf), "%d", val);
			ret += val;
		}
		else {
			ret += "null";
		}	
	}

	static void serialize(double val, std::string& ret) {
		char buf[32];
		snprintf(buf, sizeof(buf), ".16", val);
		ret += val;
	}

	static void serialize(bool val, std::string& ret) {
		if (val == true)	val += "true";
		else if (val == false) val += "false";
	}

	static void serialize(const std::string& val, std::string& ret) {
		for (int i = 0; i < val.size(); ++i) {
			const char ch = val[i];
			if (ch == '\\')
				ret += "\\\\";
			if (ch == '"')
				ret += "\\\"";
			else if (ch == '\f')
				ret += "\\f";
			else if (ch == '\n')
				ret += "\\n";
			else if (ch == '\t')
				ret += "\\t";
			else if (ch == '\b')
				ret += "\\b";
			else if (ch == '\r')
				ret += "\\r";
			else if (static_cast<uint8_t>(ch) <= 0x1f) {
				char buf[8];
				snprintf(buf, sizeof(buf), "u04x",ch);
			}
			else {
				ret += ch;
			}
		}
		ret += '"';
	}
	
	static void serialize(const ztJSON::json::array& val, std::string ret) {
		bool flag = true;
		ret += "[";
		for (const auto &c : val) {
			if (!flag)
				ret += ",";
			
		}
		ret+="]";
	}

	static void serialize(const ztJSON::json::object& val, std::string ret) {
		ret += "{";
	}




}