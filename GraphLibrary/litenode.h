#ifndef LITE_NODE_H
#define LITE_NODE_H

#include <vector>
#include <cstddef> // size_t
#include "Graph.h"

namespace msonlab { namespace lite {

	using std::vector;

	class litenode {
	public:
		friend class litegraph;
		litenode() = default;
		litenode(const litenode&) = delete;
		litenode(litenode&&);
		litenode& operator=(const litenode&) = delete;
		litenode& operator=(litenode&&);

		std::size_t p_size() const;
		std::size_t s_size() const;
		const litenode* get_predecessor(std::size_t) const;
		const litenode* get_successor(std::size_t) const;
		const vector<const litenode*>& predecessors() const  { return _predecessors; }
		const vector<const litenode*>& successors() const { return _successors; }
		unsigned id() const { return _id; }
		unsigned cptime() const { return _cptime; }
	private:
		litenode(unsigned id, unsigned cptime, std::size_t p_size, std::size_t s_size);
		void add_predecessor(const litenode *);
		void add_successor(const litenode *);

		vector<const litenode*> _predecessors;
		vector<const litenode*> _successors;
		unsigned _id;
		unsigned _cptime;
	};

	typedef vector<const litenode*> LWNodeVect;
}}

#endif