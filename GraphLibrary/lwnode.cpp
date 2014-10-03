#include "lwnode.h"

namespace msonlab {
	namespace lwgraph {
		using std::size_t;

		lwnode::lwnode(unsigned id, unsigned cptime, std::size_t p_size, std::size_t s_size)
			: id(id), cptime(cptime)
		{
			successors.reserve(s_size);
			predecessors.reserve(p_size);
		}

		void lwnode::add_predecessor(unsigned nodeid)
		{
			predecessors.push_back(nodeid);
		}

		void lwnode::add_successor(unsigned nodeid)
		{
			successors.push_back(nodeid);
		}

		std::size_t lwnode::p_size() const
		{
			return predecessors.size();
		}

		std::size_t lwnode::s_size() const
		{
			return successors.size();
		}

		unsigned lwnode::get_predecessor(size_t index) const
		{
			return predecessors[index];
		}

		unsigned lwnode::get_successor(size_t index) const
		{
			return successors[index];
		}
	}
}