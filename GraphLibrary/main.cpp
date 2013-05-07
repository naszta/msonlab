#include "Graph.h"

void main(void)
{
	msonlab::Graph graph1;

	/*msonlab::Types::DataType value1 (new int(2));
	msonlab::Types::DataType value2 (new int(4));
	msonlab::Types::DataType value3 (new wchar_t(L'valami szoveg'));*/

	msonlab::Node::nPtr node1 (new msonlab::Node(1,L'elso node',1));
	msonlab::Node::nPtr node2 (new msonlab::Node(2,L'masodik node',2));

	msonlab::Edge::ePtr edge1 (new msonlab::Edge(1,L'elso el',L'valami szoveg',node2,node1));

	graph1.addEdge(edge1);

	graph1.addNode(node1);
	graph1.addNode(node2);


	return;
}
