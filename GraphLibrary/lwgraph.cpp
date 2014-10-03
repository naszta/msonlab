#include "lwgraph.h"

namespace msonlab {
	namespace lwgraph {
		using std::size_t;

		lwgraph::lwgraph(const Graph& graph)
			: _nodes(vector<lwnode>(graph.numberOfNodes()))
		{
			auto hwnodes = graph.getNodes();
			for (auto hwnode : hwnodes)
			{
				auto id = hwnode->getId();
				_nodes[id] = lwnode(id, hwnode->getComputationTime(),
					hwnode->getPredecessorsSize(), hwnode->getSuccessorsSize());
				for (auto pred : hwnode->getPredecessors())
				{
					_nodes[id].add_predecessor(pred->getFromId());
				}

				for (auto pred : hwnode->getPredecessors())
				{
					_nodes[id].add_predecessor(pred->getFromId());
				}
			}
		}
	}
}