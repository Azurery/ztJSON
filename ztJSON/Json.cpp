#include "Json.h"
#include "stdafx.h"
#include <string>
namespace ztJSON {
	static const int MAX_DEPTH = 200;	//表示最多可处理的嵌套的深度
	
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
		return ptr->get_int_value();
	}
	double json::double_value() const { 
		return ptr->get_double_value();
	}
	bool json::bool_value() const { 
		return ptr->get_bool_value(); 
	}
	const std::string& json::string_value() const { 
		return ptr->get_string_value(); 
	}
	const json::array& json::array_value() const { 
		return ptr->get_array_value(); 
	}
	const json::object& json::object_value() const { 
		return ptr->get_object_vlaue(); 
	}

	const int json_value::get_int_value() const {
		return static_cast<const json_number*>(this)->get_int();
	}
	const double json_value::get_double_value() const { 
		return static_cast<const json_number*>(this)->get_double_value();
	}
	const bool json_value::get_bool_value() const { 
		return static_cast<const json_boolean*>(this)->value;
	}
	const std::string& json_value::get_string_value() const { 
		return static_cast<const json_string*>(this)->value;
	}
	const json::array& json_value::get_array_value() const { 
		return static_cast<const json_array*>(this)->value;
	}
	const json::object& json_value::get_object_vlaue() const { 
		return static_cast<const json_object*>(this)->value;
	}
	const json& json_value::operator[](size_t i) const { 
		return json(); 
	}
	const json& json_value::get_value(size_t i) const { 
		auto ret = static_cast<const json_array*>(this)->value;
		if (i < ret.size())
			return ret[i];
		return json::ZT_NULL;
	}
	const json& json_value::get_value(const std::string& key) const {
		auto obj = static_cast<const json_object*>(this)->value;
		auto ret = obj.find(key);
		if (ret == obj.end())
			return json::ZT_NULL;
		return ret->second;
		
	}

	void deleter::self_delete() const{
		if (flag) return;
		flag = true;
		size_t i = 0;
		while (i != pool.size()) {
			delete pool[i];
			++i;
		}
		pool.clear();
		flag = false;
	}
	void deleter::operator()(const json_value* other) noexcept {
		if (other == nullptr)	return;
		switch (other->type())
		{
		case json::ZT_ARRAY:
		case json::ZT_OBJECT:
		case json::ZT_STRING:
		case json::ZT_NUMBER:
			pool.push_back(other);
			self_delete();
		default:;
		}
	}
	
	const json& json::operator[](size_t i) const {
		if (type() == ZT_ARRAY)
			return get_json()->get_value(i);
		json null;
		return null;
	}

	json& json::operator[](size_t i){
		if (type() == ZT_ARRAY) {
			auto ret = static_cast<json_array*>(get_json())->value;
			if (i < ret.size())
				return ret[i];
		}
		if(ptr.use_count()!=1){
			ptr.reset(get_json()->duplicate(),deleter{});
		}
		auto arr = static_cast<json_array*>(get_json())->value;
		arr.insert(arr.end(), i - arr.size() + 1, json{});
		return arr[i];
	}
	//序列化：将一个对象变成Json格式的字符串
	static void serialize(int val,std::string& ret) {
		if (isdigit(val)) { //用于检查参数val是否为阿拉伯数字0到9。
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