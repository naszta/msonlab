#include "lwgraph.h"

namespace msonlab {
	namespace lw {
		using std::size_t;

		lwgraph::lwgraph(const Graph& graph)
			: _nodes(vector<lwnode>(graph.order())), _edges(graph.size())
		{
			auto hwnodes = graph.getNodes();
			for (auto hwnode : hwnodes)
			{
				auto id = hwnode->id();
				_nodes[id] = lwnode(id, hwnode->cptime(),
					hwnode->getPredecessorsSize(), hwnode->s_size());
			}

			for (auto hwnode : hwnodes)
			{
				auto id = hwnode->id();
				if (hwnode->s_size() == 0)
				{
					_onodes.push_back(&_nodes[id]);
				}
				else {
					for (auto suc : hwnode->getSuccessors())
					{
						_nodes[id].add_successor(&_nodes[suc->getToId()]);
					}
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