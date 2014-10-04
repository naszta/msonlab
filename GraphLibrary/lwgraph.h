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
			size_t size() const { return _nodes.size(); }
			size_t input_size() const;

			const vector<unsigned>& inodes() const { return _inodes; }
			const vector<lwnode>& nodes() const { return _nodes;  }

		private:
			vector<lwnode> _nodes;
			vector<unsigned> _inodes;
		};
	}
}

#endif