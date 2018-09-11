#ifndef _ZT_JSON_H
#define _ZT_JSON_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <cstdlib>

namespace ztJSON {
	class json_value;

	class json final {
		friend class json_value;
	public:
		//JSON的六种类型
		enum class json_type : uint8_t {
			ZT_NULL,
			ZT_BOOL,
			ZT_ARRAY,
			ZT_NUMBER,
			ZT_STRING,
			ZT_OBJECT
		};

		//ARRAY类型
		using array=std::vector<json>;
		//OBJECT类型
		using object=std::map<std::string, json>;

		//构造函数
		json() noexcept;				//NUL类型
		json(std::nullptr_t);
		json(int value);				//NUMBER
		json(double vlaue);				//NUMBER
		json(bool value);				//BOOL
		json(const std::string& value);	//STRING
		json(std::string&& value);
		json(const char* value);
		json(const array& value);		//ARRAY
		json(array&& value);
		json(const object& value);		//OBJECT
		json(object&& value);

		//get_json()函数用于返回json对象
		json_value* get_json() const { return ptr.get(); }
		
	public:
		//返回json的类型
		json_type type() const;

		bool is_null() const;
		bool is_bool() const;
		bool is_number() const; 
		bool is_string() const;
		bool is_array() const;
		
		//如果json类型是array，则返回一个arr[i]
		const json& operator[](size_t i) const;

		//如果这是一个object，返回一个obj[key]的引用
		//json& operator[](size_t i);

		const json& operator[](const std::string& key) const;

		//value wrapper
		int int_value() const;
		double double_value() const;
		bool bool_value() const;
		const std::string& string_value() const;
		const json::array& array_value() const;
		const json::object& object_value() const;
		const json get_array_element(size_t index) const;
		const size_t get_array_size() const;
		const size_t get_object_size() const;
		const std::string get_object_key(const std::string key) const;
		const size_t get_object_key_size(const std::string key) const;
		const json get_object_value(const std::string key) const;
		std::string serialize() const {
			std::string ret;
			serialize(ret);
			return ret;
		}

		bool operator== (const json& other) const;
		bool operator< (const json &other) const;
		bool operator!= (const json &rhs) const { return !(*this == rhs); }
		bool operator<= (const json &rhs) const { return !(rhs < *this); }
		bool operator>  (const json &rhs) const { return  (rhs < *this); }
		bool operator>= (const json &rhs) const { return !(*this < rhs); }


		//serialize()函数用于将string序列化为一个json对象
		void serialize(const std::string& str) const {}
	private:
		//ptr为指向内部实际类型json_value的智能指针
		std::shared_ptr<json_value> ptr;
	};

	
	//json_value是一个内部使用的类型，表示对各种json类型的值的抽象
	//提供一些获取实际类型、获取实际值等抽象接口
	
	class json_value {
		friend class json;
	public:
		json_value() = default;					     //default ctor
		/*json_value(const json_value&) = delete;
		json_value& operator =(json_value&) = delete;*/

		//通过显示调用，禁止使用void*类型，避免隐式地转化成一个bool类型
		json_value(void *) = delete;	
		json_value(json_value&&) = delete;
	public:
		//const virtual json_value* clone() const = 0;
		virtual bool equal(const json_value* value) const = 0;
		virtual json::json_type type() const = 0;
		//virtual bool less(const json_value * other) const = 0;

		const virtual json& get_value(size_t i) const;
		const virtual json& get_value(const std::string& kry) const;
		const virtual std::string& get_string_value() const;
		const virtual int get_int_value() const;
		const virtual double get_double_value() const;
		const virtual bool get_bool_value() const;
		const virtual json::array& get_array_value() const;
		const virtual json::object& get_object_vlaue() const;
		const virtual size_t get_array_nums() const;
		const virtual size_t get_object_nums() const;
		const virtual std::string get_object_key_wrapper(const std::string key) const;
		const virtual json get_array_items(size_t index) const;
		const virtual json get_object_value_wrapper(const std::string key) const;

		static json_value* generate_null_instance();
		static json_value* generate_true_instance();
		static json_value* generate_false_instance();
		static json_value* generate_int_instance(int value);
		static json_value* generate_double_instance(double value);
		static json_value* generate_string_instance(const std::string& str);
		static json_value* generate_string_instance(std::string&& str);
		static json_value* generate_object_instance(const json::object& obj);
		static json_value* generate_object_instance(json::object&& obj);
		static json_value* generate_array_instance(const json::array& arr);
		static json_value* generate_array_instance(json::array&& arr);
		
		virtual const json& operator[](size_t i) const;
		virtual const json& operator[](const std::string& key) const;
		
		virtual ~json_value() {}
	};

	class json_number : public json_value {
		friend class json;
		friend class json_value;
	public:
		json::json_type type() const override{
			return json::json_type::ZT_NUMBER;
		}
		/*bool less(const json_value* other) const {
			return value < other->get_value();
		}*/

		virtual int get_int() const = 0;
		virtual double get_double() const = 0;
		virtual bool equal_to(int i) const = 0;
		virtual bool equal_to(double i) const = 0;
	};

	class json_int final : public json_number {
		friend json;
		friend json_value;
	public:
		json_int(int v) :value(v) {}
		/*json_value* clone() const override {
			return new json_int(value);
		}*/
		/*bool less(const json_value* other) const override {
			return	value < other->get_int_value();
		}*/
		int get_int() const override {
			return value;
		}
		double get_double() const override{
			return static_cast<double>(value);
		}
		bool equal(const json_value* other) const override {
			auto num = reinterpret_cast<const json_number*>(other);
			return num->equal_to(value);
		}
	private:
		bool equal_to(int i) const override {
			return value == i;
		}

		bool equal_to(double i) const override {
			return static_cast<double>(value) == i;
		}

	private:
		const int value;
	};

	class json_double final :public json_number {
		friend class json;
		friend class json_value;
	public:
		json_double(double i):value(i){}
		/*json_value* clone() const override {
			return new json_double(value);
		}*/
		int get_int() const override{
			return static_cast<int>(value);
		}
		/*bool less(const json_value* other) const override {
			return value < other->get_double_value();
		}*/
		double get_double()const override {
			return value;
		}
		bool equal(const json_value* other) const override {
			auto num = (json_number*)other;
			return num->equal_to(value);
		}
	private:
		bool equal_to(int i) const override {
			return value == static_cast<double>(i);
		}
		bool equal_to(double i) const override {
			return value == i;
		}
	private:
		const double value;
	};

	class json_boolean : public json_value {
		friend class json_value;
		friend class json;
	public:
		json_boolean(bool b):value(b){}
		/*json_value* clone() const override {
			return new json_boolean(value);
		}*/
		json::json_type type() const override {
			return json::json_type::ZT_BOOL;
		}
		bool equal(const json_value* other) const override {
			return value == static_cast<decltype(this)>(other)->value;
		}
	private:
		bool value;
	};

	class json_string : public json_value {
		friend class json_value;
		friend class json;
	public:
		json_string(const std::string& val):value(val){}
		json_string(std::string&& s) :value(std::move(s)) {}
		/*json_value* clone() const override {
			return new json_string(value);
		}*/
		json::json_type type() const override {
			return json::json_type::ZT_STRING;
		}
		bool equal(const json_value* other) const override {
			return value == static_cast<decltype(this)>(other)->value;
		}
	private:
		std::string value;
	};

	class json_object : public json_value {
		friend class json_value;
		friend class json;
		const json& operator[](const std::string& key) const override;
	public:
		json_object(const json::object& val):obj(val){}
		json_object(const json::object&& val):obj(std::move(val)){}
		/*json_value* clone() const override {
			return new json_object(value);
		}*/

		json::json_type type() const override {
			return json::json_type::ZT_OBJECT;
		}
		const std::string get_object_key_wrapper(const std::string key) const {
			return obj.find(key)->first;
		}
		const json get_object_value_wrapper(const std::string key) const {
			return obj.find(key)->second;
		}
		const size_t get_object_nums() const {
			return obj.size();
		}
		bool equal(const json_value* other) const override {
			return obj == static_cast<decltype(this)>(other)->obj;
		}
	private:
		json::object obj;
	};

	class json_array : public json_value {
		friend class json;
		friend class json_value;
		const json& operator[](size_t i) const override;
	public:
		json_array(const json::array& val):arr(val){}
		json_array(const json::array&& val):arr(std::move(val)){}
		/*json_value* clone() const override {
			return new json_array(value);
		}*/

		const size_t get_array_nums() const {
			return arr.size();
		}
		const json get_array_items(size_t index) const {
			return arr[index];
		}
		json::json_type type() const override {
			return json::json_type::ZT_ARRAY;
		}
		bool equal(const json_value* other) const override {
			return arr == static_cast<decltype(this)>(other)->arr;
		}
	private:
		json::array arr;
	};

	class json_null : public json_value {
	public:
		/*const json_value* clone() const override {
			return this;
		}*/

		json::json_type type() const override {
			return json::json_type::ZT_NULL;
		}
		bool equal(const json_value* other) const override {
			return this == other;
		}
	};
}

#endif
