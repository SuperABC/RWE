#pragma once
#include <iostream>
#include <vector>
#include <string>

enum JE {
	JE_NULL,
	JE_STRING,
	JE_ARRAY,
	JE_BOOL,
	JE_FLOAT,
};
struct Element {
	JE type;
	union {
		char *str;
		float *array;
		bool b;
		float f;
	};
};

class JSON {
private:
	char *json;
	int proc = 0;
	std::vector<std::pair<std::string, Element>> content;

	std::string parseString();
	Element parseElement();
public:
	JSON();
	JSON(char *json);

	Element operator[](std::string name);
};