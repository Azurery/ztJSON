#include "Json.h"
#include <string>
namespace ztJSON {
	class json_value;
	//static const int MAX_DEPTH = 200;	//表示最多可处理的嵌套的深度
	
	json::json() noexcept :					ptr(std::make_shared<json_null>()) {}
	json::json(std::nullptr_t) :			json() {}
	json::json(int value) :					ptr(std::make_shared<json_int>(value)) {}
	json::json(double value) :				ptr(std::make_shared<json_double>(value)){}
	json::json(const std::string& value) :	ptr(std::make_shared<json_string>(value)) {}	
	json::json(std::string&& value) :		ptr(std::make_shared<json_string>(std::move(value))) {}
	
 	std::shared_ptr<json_value> true_ = std::make_shared<json_boolean>(true);
	std::shared_ptr<json_value> false_ = std::make_shared<json_boolean>(false);
	json::json(bool value) :                ptr(value ? true_ : false_) {}
	json::json(const char* value) :			ptr(std::make_shared<json_string>(value)) {}
	json::json(const array& value) :		ptr(std::make_shared<json_array>(value)){}
	json::json(array&& value) :				ptr(std::make_shared<json_array>(std::move(value))) {}
	json::json(const object& value) :		ptr(std::make_shared<json_object>(value)) {}
	json::json(object&& value) :			ptr(std::make_shared<json_object>(std::move(value))) {}

	/*json::json() noexcept : json(nullptr) {}
	json::json(std::nullptr_t) : json(json_value::generate_null_instance()) {}
	json::json(int value) : json(json_value::generate_int_instance(value)) {}
	json::json(double value) : json(json_value::generate_double_instance(value)) {}
	json::json(const std::string& value) : json(json_value::generate_string_instance(value)) {}
	json::json(std::string&& value) : json(json_value::generate_string_instance(std::move(value))) {}
	json::json(bool value) : json(value ? json_value::generate_true_instance() : json_value::generate_false_instance()) {}
	json::json(const char* value) : json(json_value::generate_string_instance(value)) {}
	json::json(const array& value) : json(json_value::generate_array_instance(value)) {}
	json::json(array&& value) : json(json_value::generate_array_instance(std::move(value))) {}
	json::json(const object& value) : json(json_value::generate_object_instance(value)) {}
	json::json(object&& value) : json(json_value::generate_object_instance(std::move(value))) {}*/


	json_value* json_value::generate_null_instance() {
		static json_null null_instance{};
		return &null_instance;
	}

	json_value* json_value::generate_true_instance() {
		static json_boolean true_instance{ true };
		return &true_instance;
	}

	json_value* json_value::generate_false_instance() {
		static json_boolean false_instance{ false };
		return &false_instance;
	}

	json_value* json_value::generate_int_instance(int value) {
		return new json_int(value);
	}

	json_value* json_value::generate_double_instance(double value) {
		return new json_double(value);
	}

	json_value* json_value::generate_string_instance(const std::string& str) {
		return new json_string(str);
	}

	json_value* json_value::generate_string_instance(std::string&& str) {
		return new json_string(std::move(str));
	}

	json_value* json_value::generate_object_instance(const json::object& obj) {
		return new json_object(obj);
	}

	json_value* json_value::generate_object_instance(json::object&& obj) {
		return new json_object(std::move(obj));
	}

	json_value* json_value::generate_array_instance(const json::array& arr) {
		return new json_array(arr);
	}

	json_value* json_value::generate_array_instance(json::array&& arr) {
		return new json_array(std::move(arr));
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

	json::json_type json::type() const {
		return ptr->type();
	}

	bool json::operator== (const json& other) const {
		if (other.ptr == ptr)
			return true;
		if (!ptr) {
			if (ptr->type() != other.ptr->type())
				return false;
		}
		return ptr->equal(other.ptr.get());
	}

	
	/*bool json::operator< (const json &other) const {
		if (ptr == other.ptr)
			return false;
		if (ptr->type() != other.ptr->type())
			return ptr->type() < other.ptr->type();

		return ptr->less(other.ptr.get());
	}*/


	const int json_value::get_int_value() const {
		return static_cast<const json_number*>(this)->get_int();
	}
	const double json_value::get_double_value() const { 
		return static_cast<const json_number*>(this)->get_double();
	}
	const bool json_value::get_bool_value() const { 
		return static_cast<const json_boolean*>(this)->value;
	}
	const std::string& json_value::get_string_value() const { 
		return reinterpret_cast<const json_string*>(this)->value;
	}
	const json::array& json_value::get_array_value() const { 
		return static_cast<const json_array*>(this)->arr;
	}
	const json::object& json_value::get_object_vlaue() const { 
		return static_cast<const json_object*>(this)->obj;
	}
	const size_t json_value::get_array_nums() const {
		return static_cast<const json_array*>(this)->get_array_nums();
	}

	const size_t json_value::get_object_nums() const {
		return static_cast<const json_object*>(this)->get_object_nums();
	}

	const json json_value::get_array_items(size_t index) const {
		return static_cast<const json_array*>(this)->get_array_items(index);
	}

	const json& json_value::get_value(size_t i) const {
		auto ret = static_cast<const json_array*>(this)->arr;
		if (i < ret.size())
			return ret[i];
		return json_value::generate_null_instance();
	}
	const json& json_value::get_value(const std::string& key) const {
		auto obj = static_cast<const json_object*>(this)->obj;
		auto ret = obj.find(key);
		if (ret == obj.end())
			return json_value::generate_null_instance();
		return ret->second;
		
	}

	bool json::is_null() const { return get_json()->type() == json_type::ZT_NULL; }
	bool json::is_bool() const { return get_json()->type() == json_type::ZT_BOOL; }
	bool json::is_number() const { return get_json()->type() == json_type::ZT_NUMBER; }
	bool json::is_string() const { return get_json()->type() == json_type::ZT_STRING; }
	bool json::is_array() const { return get_json()->type() == json_type::ZT_ARRAY; }
	/*void deleter::self_delete() const{
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
		switch (other->type()) {
		case json::ZT_ARRAY:
		case json::ZT_OBJECT:
		case json::ZT_STRING:
		case json::ZT_NUMBER:
			pool.push_back(other);
			self_delete();
		default:;
		}
	}*/

	//序列化：将一个对象变成Json格式的字符串
	//将int类型的值序列化为一个json对象
	static void serialize(int val,std::string& ret) {
		//用于检查参数val是否为阿拉伯数字0到9。
		if (isdigit(val)) { 
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
		snprintf(buf, sizeof(buf), "16", val);
		ret += val;
	}

	static void serialize(bool val, std::string& ret) {
		if (val == true)	val += "true";
		else if (val == false) val += "false";
		else val += "null";
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
	//将array序列化为json对象
	static void serialize(const ztJSON::json::array& val, std::string ret) {
		bool flag = true;
		ret += "[";
		for (int i = 0; i < val.size();++i) {
			if (flag) {
				ret += val[i].string_value();
				flag = false;
			}	
			else {
				ret += ",";
				flag = true;
			}		
		}
		ret+="]";
	}
	//将object对象转化为一个json对象
	static void serialize(const ztJSON::json::object& val, std::string ret) {
		std::map<std::string, json>::const_iterator iter = val.cbegin();
		ret += "{";
		++iter;
		while (iter!=val.cend()) {
			if (iter->first == std::to_string('\"')) {
				ret += "\"";
				iter++;
				ret += iter->first;
				ret += "\"";
				ret += ":";
				ret += "{";
				ret += iter->second.string_value();
			}
			else {
				return;
			}
		}
	}

	//const json& json::operator[](size_t i) const {
	//	if (type() == ZT_ARRAY)
	//		return get_json()->get_value(i);
	//	json null;
	//	return null;
	//}

	//json& json::operator[](size_t i) {
	//	if (type() == ZT_ARRAY) {
	//		auto ret = static_cast<json_array*>(get_json())->value;
	//		if (i < ret.size())
	//			return ret[i];
	//	}
	//	/*if(ptr.use_count()!=1){
	//	ptr.reset(get_json()->clone(),deleter{});
	//	}*/
	//	auto arr = static_cast<json_array*>(get_json())->value;
	//	arr.insert(arr.end(), i - arr.size() + 1, json{});
	//	return arr[i];
	//}
	const json& json::operator[](size_t i) const {
		return (*ptr)[i];
	}

	const json& json::operator[](const std::string& key) const {
		if(ptr!=nullptr){
			return (*ptr)[key];
		}
	}

	const size_t json::get_array_size() const {
		return ptr->get_array_nums();
	}

	const json json::get_array_element(size_t index) const{
		return ptr->get_array_items(index);
	}

	const size_t json::get_object_size() const {
		return ptr->get_object_nums();
	}

	const json& json_value::operator[](size_t) const {
		json json_null;
		return json_null;
	}

	const json& json_value::operator[](const std::string&) const {
		json json_null;
		return json_null;
	}

	const json& json_object::operator[](const std::string& key) const {
		auto iter = obj.find(key);
		return /*(iter == value.end()) ? json() : */iter->second;
	}

	const json& json_array::operator[](size_t i) const {
		if (i >= arr.size()) {
			json json_null;
			return json_null;
		}else {
			return arr[i];
		}
	}

	

}