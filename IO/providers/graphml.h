#ifndef GRAPHML_H_
#define GRAPHML_H_

#include "IO/io.h"

class GraphmlGraphInput : public InputProvider
{
public:
	virtual Error readGraph(Graph *graph, const char *fileName,
	  Encoding *encoding);
};

#endif // GRAPHML_H_
