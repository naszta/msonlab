#ifndef LITE_GRAPH_H
#define LITE_GRAPH_H

#include <vector>
#include "Graph.h"
#include "litenode.h"

namespace msonlab { namespace lite {

	using std::vector;

	class litegraph {
	public:
		litegraph() = delete;
		litegraph(const Graph&);
		size_t order() const { return _nodes.size(); }
		size_t size() const { return _edges; }
		size_t input_size() const;

		const vector<unsigned>& inodes() const { return _inodes; }
		const vector<const litenode*>& onodes() const { return _onodes; }
		const vector<litenode>& nodes() const { return _nodes;  }

	private:
		vector<litenode> _nodes;
		vector<unsigned> _inodes;
		vector<const litenode*> _onodes;
		size_t _edges;
	};
}}

#endif