#ifndef RWE_BUILDER_GRAPH_PROGRAM
#define RWE_BUILDER_GRAPH_PROGRAM
#include "Frame/main/main.h"
#include "page.h"

class Program {
private:
	vector<Page> pages;
public:
	int id;
	string name;

	Program();
	~Program();
	
	void addPage();
};

#endif