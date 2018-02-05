#ifndef _ZT_JSON_H
#define _ZT_JSON_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
namespace ztJSON {
	class json_value;
	//����һ���Լ���ɾ������deleter
	class deleter {
		std::vector<const json_value*> temp;
	public:
		void operator()(const json_value* other) noexcept;
		void self_delete() const;
	private:
		static bool flag;
		static std::vector<const json_value*> pool;
	};

	class json final {
	public:
		//JSON����������
		enum json_type {
			ZT_NULL,
			ZT_BOOL,
			ZT_ARRAY,
			ZT_NUMBER,
			ZT_STRING,
			ZT_OBJECT
		};
		//ARRAY����
		using array=std::vector<json>;
		//OBJECT����
		using object=std::map<std::string, json>;

		//���캯��
		json() noexcept;	//NUL����
		json(int value);	//NUMBER
		json(double vlaue); //NUMBER
		json(bool value);	//BOOL
		json(const std::string& value);	//SRING
		json(std::string&& value);
		json(const char* value);
		json(const array& value);	//ARRAY
		json(array&& value);
		json(const object& value);	//OBJECT
		json(object&& value);
		//get_json()�������ڷ���json����
		json_value* get_json() const { return ptr.get(); }
		//����json������
		json_type type() const;
		bool is_null() const { return type() == json_type::ZT_NULL; }
		bool is_bool() const { return type() == json_type::ZT_BOOL; }
		bool is_number() const { return type() == json_type::ZT_NUMBER; }
		bool is_string() const { return type() == json_type::ZT_STRING; }
		bool is_array() const { return type() == json_type::ZT_ARRAY; }
		const json& operator[](size_t i) const;//�������һ��array������һ��arr[i]������
		json& operator[](size_t i);
		const json& operator[](const std::string& key) const; 
		//�������һ��object������һ��obj[key]������

		//value wrapper,��ֵ�⸲��
		int int_value() const;
		double double_value() const;
		bool bool_value() const;
		const std::string& string_value() const;
		const json::array& array_value() const;
		const json::object& object_value() const;
		std::string serialize() const {
			std::string ret;
			serialize(ret);
			return ret;
		}
		//serialize()�������ڽ�string���л�Ϊһ��json����
		void serialize(const std::string& str) const {}
	private:
		//ptrΪָ���ڲ�ʵ������json_value������ָ��
		std::shared_ptr<json_value> ptr;
	};

	/*
	json_value��һ���ڲ�ʹ�õ����ͣ���ʾ�Ը���json���͵�ֵ�ĳ���
	�ṩһЩ��ȡʵ�����͡���ȡʵ��ֵ�ȳ���ӿ�
	*/
	class json_value {
		friend class json;
	public:
		//Ĭ�Ϲ��캯��
		json_value() = default;
		//��ֹ�����Ĺ��캯��
		json_value(const json_value&) = delete;
		//��ֹ�����Ŀ�����ֵ�����
		json_value& operator =(json_value&) = delete;
		//ͨ����ʾ�ĵ��ã���ֹʹ��void*���ͣ�������ʽ��ת����һ��bool����
		json_value(void *) = delete;	
		json_value(json_value&&) = delete;
		

		const virtual json_value* get_copy() const = 0;
		virtual bool equal(const json_value* value) const = 0;
		virtual json::json_type type() const = 0;
		const virtual json& get_value(size_t i) const;
		const virtual json& get_value(const std::string& kry) const;
		const virtual std::string& get_string_value() const;
		const virtual int get_int_value() const;
		const virtual double get_double_value() const;
		const virtual bool get_bool_value() const;
		const virtual json::array& get_array_value() const;
		const virtual json::object& get_object_vlaue() const;
		virtual const json& operator[](size_t i) const;
		virtual const json& operator[](std::string& key) const;
		virtual ~json_value() = default;

	};

	class json_number :public json_value {
		friend class json;
		friend class json_value;
	public:
		json::json_type type() const override{
			return json::ZT_NUMBER;
		}
		virtual bool less(const json_value* value) const = 0;
		virtual int get_int() const = 0;
		virtual double get_double() const = 0;
		virtual bool equal_to(int i) const = 0;
		virtual bool equal_to(double i) const = 0;
	};

	class json_int final:public json_number {
		friend json;
		friend json_value;
	public:
		json_int(int v) :value(v) {}
		json_value* get_copy() const override {
			return new json_int(value);
		}
		bool less(const json_value* other) const override {
			return	value < other->get_int_value();
		}
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
		json_value* get_copy() const override {
			return new json_double(value);
		}
		int get_int() const override{
			return static_cast<int>(value);
		}
		bool less(const json_value* other) const override {
			return value < other->get_double_value();
		}
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

	class json_boolean :public json_value {
		friend class json_value;
		friend class json;
	public:
		json_boolean(bool b):value(b){}
		json_value* get_copy() const override {
			return new json_boolean(value);
		}
		json::json_type type() const override {
			return json::ZT_BOOL;
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
		json_value* get_copy() const override {
			return new json_string(value);
		}
		json::json_type type() const override {
			return json::ZT_STRING;
		}
		bool equal(const json_value* other) const override {
			return value == static_cast<decltype(this)>(other)->value;
		}
	private:
		std::string value;
	};

	class json_object :public json_value {
		friend class json_value;
		friend class json;
	public:
		json_object(const json::object& val):value(val){}
		json_object(const json::object&& val):value(std::move(val)){}
		json_value* get_copy() const override {
			return new json_object(value);
		}
		json::json_type type() const override {
			return json::ZT_OBJECT;
		}
		bool equal(const json_value* other) const override {
			return value == static_cast<decltype(this)>(other)->value;\
		}
	private:
		json::object value;
	};

	class json_array :public json_value {
		friend class json;
		friend class json_value;
	public:
		json_array(const json::array& val):value(val){}
		json_array(const json::array&& val):value(std::move(val)){}
		json_value* get_copy() const override {
			return new json_array(value);
		}
		json::json_type type() const override {
			return json::ZT_ARRAY;
		}
		bool equal(const json_value* other) const override {
			return value == static_cast<decltype(this)>(other)->value;
		}
	private:
		json::array value;
	};

	class json_null :public json_value {
	public:
		const json_value* get_copy() const override {
			return this;
		}
		json::json_type type() const override {
			return json::ZT_NULL;
		}
		bool equal(const json_value* other) const override {
			return this == other;
		}
	};
}

#endif
