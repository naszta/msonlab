#ifndef LW_GRAPH_H
#define LW_GRAPH_H

#include <vector>
#include "Graph.h"
#include "lwnode.h"

namespace msonlab {
	namespace lw {

		using std::vector;

		class lwgraph {
		public:
			lwgraph() = delete;
			lwgraph(const Graph&);
			size_t order() const { return _nodes.size(); }
			size_t size() const { return _edges; }
			size_t input_size() const;

			const vector<unsigned>& inodes() const { return _inodes; }
			const vector<const lwnode*>& onodes() const { return _onodes; }
			const vector<lwnode>& nodes() const { return _nodes;  }

		private:
			vector<lwnode> _nodes;
			vector<unsigned> _inodes;
			vector<const lwnode*> _onodes;
			size_t _edges;
		};
	}
}

#endif