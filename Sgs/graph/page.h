#ifndef RWE_BUILDER_GRAPH_PAGE
#define RWE_BUILDER_GRAPH_PAGE

/*
 *Class Page is one layout in a graph. It contains several 
 *widgets.
*/

#include "Frame/main/main.h"
#include "widget.h"

class Page {
private:
	vector<Widget> widgets;
public:
	int id;
	string name;

	Page();
	~Page();

	void addWidget();
};

#endif