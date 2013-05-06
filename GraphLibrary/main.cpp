#include "Graph.h"

void main(void)
{
	msonlab::Graph graph1;

	msonlab::Node::nPtr node1 (new msonlab::Node(1,L'elso node',1));
	msonlab::Node::nPtr node2 (new msonlab::Node(2,L'masodik node',4));

	msonlab::Edge::ePtr edge1 (new msonlab::Edge(1,L'elso el',L'valami',node2,node1));

	graph1.addEdge(edge1);

	graph1.addNode(node1);
	graph1.addNode(node2);


	return;
}
