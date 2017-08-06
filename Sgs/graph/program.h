#ifndef RWE_BUILDER_GRAPH_PROGRAM
#define RWE_BUILDER_GRAPH_PROGRAM

/*
 *Class Program is the final exe structure. It contains several
 *pages.
*/

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