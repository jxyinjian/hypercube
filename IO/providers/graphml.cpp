#include <string>
#include <map>
#include <stack>
#include "CORE/misc.h"
#include "IO/providers/xml/xml.h"
#include "graphml.h"

using namespace std;


class GraphmlHandler : public XmlHandler
{
public:
	GraphmlHandler(Graph *graph) : _graph(graph) {}

	virtual bool startDocument();
	virtual bool endDocument();
	virtual bool startElement(const std::wstring &name, const XmlAttributes &atts);
	virtual bool endElement(const wstring &name);
	virtual bool data(const wstring &data);

private:
	struct Relation {
		wstring node;
		wstring parent;
	};

	struct GraphAttributes {
		wstring edgedefault;
	};

	struct NodeAttributes {
		wstring id;
	};

	struct EdgeAttributes {
		wstring id;
		wstring source;
		wstring target;
	};

	Vertex *addVertex(const wstring &id);
	Edge *addEdge(const wstring &source, const wstring &target);
	bool checkRelation(const wstring &node, const wstring &parent);
	void initGraphAttributes();
	void clearNodeAttributes();
	void clearEdgeAttributes();
	void setAttribute(const wstring &element, const wstring &attr,
	  const wstring &value);
	bool handleElement(const wstring &element);

	Graph *_graph;
	map<wstring, Vertex*> _vertexes;
	stack<wstring> _parent;
	GraphAttributes _graphAttributes;
	NodeAttributes _nodeAttributes;
	EdgeAttributes _edgeAttributes;

	static const Relation relations[];
};


const GraphmlHandler::Relation GraphmlHandler::relations[] = {
	{L"graphml", L""},
	{L"graph", L"graphml"},
	{L"node", L"graph"},
	{L"edge", L"graph"}
};


bool GraphmlHandler::startDocument()
{
	initGraphAttributes();

	return true;
}

bool GraphmlHandler::endDocument()
{
	_graph->setDirected((_graphAttributes.edgedefault == L"directed"));

	return true;
}

bool GraphmlHandler::startElement(const std::wstring &name, const XmlAttributes &atts)
{
	if (!checkRelation(name, _parent.empty() ? L"" : _parent.top()))
		return false;

	for (size_t i = 0; i < atts.count(); i++)
		setAttribute(name, atts.name(i), atts.value(i));

	if (!handleElement(name))
		return false;

	_parent.push(name);

	return true;
}

bool GraphmlHandler::endElement(const wstring &)
{
	_parent.pop();

	return true;
}

bool GraphmlHandler::data(const wstring &)
{
	return true;
}


Vertex* GraphmlHandler::addVertex(const wstring &id)
{
	Vertex *v;
	map<wstring, Vertex*>::const_iterator it;

	it = _vertexes.find(id);
	if (it != _vertexes.end())
		return it->second;

	v = _graph->addVertex();

	_vertexes.insert(pair<wstring, Vertex*>(id, v));

	return v;
}

Edge* GraphmlHandler::addEdge(const wstring &source, const wstring &target)
{
	Vertex *src, *dst;

	src = addVertex(source);
	dst = addVertex(target);

	return _graph->addEdge(src, dst);
}

bool GraphmlHandler::checkRelation(const wstring &node, const wstring &parent)
{
	if (parent.empty()) {
		if (node != L"graphml")
			return false;
	}

	for (size_t i = 0; i < ARRAY_SIZE(relations); i++) {
		if (node == relations[i].node) {
			if (parent != relations[i].parent)
				return false;
		}
	}

	return true;
}

void GraphmlHandler::setAttribute(const wstring &element, const wstring &attr,
  const wstring &value)
{
	if (element == L"node") {
		if (attr == L"id")
			_nodeAttributes.id = value;
	} else if (element == L"edge") {
		if (attr == L"id")
			_edgeAttributes.id = value;
		if (attr == L"source")
			_edgeAttributes.source = value;
		if (attr == L"target")
			_edgeAttributes.target = value;
	} else if (element == L"graph") {
		if (attr == L"edgedefault")
			_graphAttributes.edgedefault = value;
	}
}

void GraphmlHandler::initGraphAttributes()
{
	_graphAttributes.edgedefault = L"directed";
}

void GraphmlHandler::clearNodeAttributes()
{
	_nodeAttributes.id.clear();
}

void GraphmlHandler::clearEdgeAttributes()
{
	_edgeAttributes.id.clear();
	_edgeAttributes.source.clear();
	_edgeAttributes.target.clear();
}

bool GraphmlHandler::handleElement(const wstring &element)
{
	Vertex *vertex;
	Edge *edge;

	if (element == L"node") {
		if (_nodeAttributes.id.empty())
			return false;
		vertex = addVertex(_nodeAttributes.id);
		vertex->setText(_nodeAttributes.id);

		clearNodeAttributes();
	}
	if (element == L"edge") {
		if (_edgeAttributes.source.empty() || _edgeAttributes.target.empty())
			return false;
		edge = addEdge(_edgeAttributes.source, _edgeAttributes.target);
		if (!_edgeAttributes.id.empty())
			edge->setText(_edgeAttributes.id);

		clearEdgeAttributes();
	}

	return true;
}


IO::Error GraphmlGraphInput::readGraph(Graph *graph, const char *fileName,
  Encoding *)
{
	XmlParser parser;
	GraphmlHandler handler(graph);

	parser.setHandler(&handler);
	parser.setErrorPrefix("GraphML");
	return parser.parse(fileName);
}
