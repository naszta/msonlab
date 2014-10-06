#include "lwnode.h"

namespace msonlab {
	namespace lw {
		using std::size_t;

		lwnode::lwnode(unsigned id, unsigned cptime, std::size_t p_size, std::size_t s_size)
			: _id(id), _cptime(cptime)
		{
			successors.reserve(s_size);
			predecessors.reserve(p_size);
		}

		void lwnode::add_predecessor(const lwnode *node)
		{
			predecessors.push_back(node);
		}

		void lwnode::add_successor(const lwnode *node)
		{
			successors.push_back(node);
		}

		std::size_t lwnode::p_size() const
		{
			return predecessors.size();
		}

		std::size_t lwnode::s_size() const
		{
			return successors.size();
		}

		const lwnode* lwnode::get_predecessor(size_t index) const
		{
			return predecessors[index];
		}

		const lwnode* lwnode::get_successor(size_t index) const
		{
			return successors[index];
		}
	}
}