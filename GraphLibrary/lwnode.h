#ifndef LW_NODE_H
#define LW_NODE_H

#include <vector>
#include <cstddef> // size_t
#include "Graph.h"

namespace msonlab { namespace lw {

	using std::vector;

	class lwnode {
	public:
		lwnode() = default;
		lwnode(const lwnode&) = default;
		lwnode(const lwnode&&);
		lwnode& operator=(const lwnode&) = default;

		lwnode(unsigned id, unsigned cptime, std::size_t p_size, std::size_t s_size);
		void add_predecessor(const lwnode *);
		void add_successor(const lwnode *);
		std::size_t p_size() const;
		std::size_t s_size() const;
		const lwnode* get_predecessor(std::size_t) const;
		const lwnode* get_successor(std::size_t) const;
		const vector<const lwnode*>& predecessors() const  { return _predecessors; }
		const vector<const lwnode*>& successors() const { return _successors; }
		unsigned id() const { return _id; }
		unsigned cptime() const { return _cptime; }
	private:
		vector<const lwnode*> _predecessors;
		vector<const lwnode*> _successors;
		unsigned _id;
		unsigned _cptime;
	};

	typedef vector<const lwnode*> LWNodeVect;
}}

#endif