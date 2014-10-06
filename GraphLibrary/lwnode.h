#ifndef LW_NODE_H
#define LW_NODE_H

#include <vector>
#include <cstddef> // size_t
#include "Graph.h"

namespace msonlab {
	namespace lw {

		using std::vector;

		class lwnode {
		public:
			lwnode() = default;
			lwnode(const lwnode&) = delete;
			lwnode& operator=(const lwnode&) = default;

			lwnode(unsigned id, unsigned cptime, std::size_t p_size, std::size_t s_size);
			void add_predecessor(const lwnode *);
			void add_successor(const lwnode *);
			std::size_t p_size() const;
			std::size_t s_size() const;
			const lwnode * get_predecessor(std::size_t) const;
			const lwnode * get_successor(std::size_t) const;
			unsigned id() const { return _id; }
		private:
			vector<const lwnode*> predecessors;
			vector<const lwnode*> successors;
			unsigned _id;
			unsigned _cptime;
		};
	}
}

#endif