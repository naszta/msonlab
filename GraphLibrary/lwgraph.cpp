#include "lwgraph.h"

namespace msonlab {
	namespace lw {
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
			}

			for (auto hwnode : hwnodes)
			{
				auto id = hwnode->getId();
				for (auto suc : hwnode->getSuccessors())
				{
					_nodes[id].add_successor(&_nodes[suc->getToId()]);
				}

				if (hwnode->getPredecessorsSize() == 0)
				{
					_inodes.push_back(id);
				}
				else
				{
					for (auto pred : hwnode->getPredecessors())
					{
						_nodes[id].add_predecessor(&_nodes[pred->getFromId()]);
					}
				}
			}
		}
	}
}