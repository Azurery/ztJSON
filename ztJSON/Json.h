#ifndef _ZT_JSON_H
#define _ZT_JSON_H

#include <string>
#include <map>
#include <vector>

namespace ztJSON {
	class json final {
		//JSON����������
		enum type {
			ZT_NULL,
			ZT_FALSE,
			ZT_TRUE,
			ZT_NUMBER,
			ZT_STRING,
			ZT_OBJECT
		};
		//array:���ڴ洢JSON���ݵ�vector
		typedef std::vector<json> array;
		//object:���ڴ洢JSON����
		typedef std::map<std::string, json> object;

		//���ֲ�ͬ���͵�JSON������
		//NULL
		json() noexcept;	
		//NUMBER
		json(double value);	
		json(int value);
		//STRING
		json(const std::string& value);
		//ARRAY
		json(const array& values);
		//OBJECT
		json(const object& values);
	};
}

#endif
