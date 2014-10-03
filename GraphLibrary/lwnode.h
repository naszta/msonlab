#ifndef LW_NODE_H
#define LW_NODE_H

#include <vector>
#include <cstddef> // size_t
#include "Graph.h"

namespace msonlab {
	namespace lwgraph {

		using std::vector;

		class lwnode {
		public:
			lwnode() = default;
			lwnode(const lwnode&) = delete;
			lwnode& operator=(const lwnode&) = default;

			lwnode(unsigned id, unsigned cptime, std::size_t p_size, std::size_t s_size);
			void add_predecessor(unsigned);
			void add_successor(unsigned);
			std::size_t p_size() const;
			std::size_t s_size() const;
			unsigned get_predecessor(std::size_t) const;
			unsigned get_successor(std::size_t) const;
		private:
			vector<unsigned> predecessors;
			vector<unsigned> successors;
			unsigned id;
			unsigned cptime;
		};
	}
}

#endif