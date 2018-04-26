#include "Text/util/json.h"

std::string JSON::parseString() {
	if (json[proc] == '\"') {
		proc++;
		std::string ret;
		while (json[proc] != '\"')ret.push_back(json[proc++]);
		proc++;
		return ret;
	}
	return std::string();
}
Element JSON::parseElement() {
	Element ret;
	while (json[proc] == ' ')proc++;
	if (json[proc] == '\"') {
		ret.type = JE_STRING;

		std::string tmp = parseString();
		ret.str = (char *)malloc(tmp.length() + 1);
		strcpy_s(ret.str, tmp.length() + 1, tmp.data());
	}
	else if (json[proc] == '[') {
		ret.type = JE_ARRAY;

		std::vector<float> tmp;
		proc++;
		while (json[proc] != ']') {
			if (json[proc] == ',') {
				proc++;
				continue;
			}
			tmp.push_back((float)atof(json + proc));
			while (json[proc] != ',' && json[proc] != ']')proc++;
		}
		proc++;
		ret.array = (float *)malloc(tmp.size() * sizeof(float));
		for (unsigned int i = 0; i < tmp.size(); i++) {
			ret.array[i] = tmp[i];
		}
	}
	else if (json[proc] == 'f' || json[proc] == 't') {
		ret.type = JE_BOOL;
		if (json[proc] == 'f') {
			ret.b = false;
			proc += 5;
		}
		else if (json[proc] == 't') {
			ret.b = true;
			proc += 4;
		}
	}
	else if(json[proc] >= '0' && json[proc] <= '9'){
		ret.type = JE_FLOAT;
		ret.f = (float)atof(json + proc);
		while (json[proc] >= '0' && json[proc] <= '9')proc++;
		if (json[proc] == '.') {
			proc++;
			while (json[proc] >= '0' && json[proc] <= '9')proc++;
		}
	}
	return ret;
}

JSON::JSON() {

}
JSON::JSON(char *json) {
	this->json = json;
	if (json[proc] == '{') {
		proc++;
		while (json[proc] != '}') {
			if (json[proc] == ' ' || json[proc] == '\t' || json[proc] == '\n' ||
				json[proc] == ',') {
				proc++;
				continue;
			}
			if (json[proc] == '#') {
				while (json[proc++] != '\t');
				continue;
			}
			std::string name = parseString();
			while (json[proc] == ' ')proc++;
			if (json[proc] == ':') {
				proc++;
				Element element = parseElement();
				content.push_back(std::pair<std::string, Element>(name, element));
			}
			else return;
		}
	}
	else return;
}

Element JSON::operator[](std::string name) {
	for (auto e : content) {
		if (e.first == name)return e.second;
	}
	return Element();
}