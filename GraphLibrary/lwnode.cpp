#include "lwnode.h"
#include <utility>

namespace msonlab {
	namespace lw {
		using std::size_t;

		lwnode::lwnode(unsigned id, unsigned cptime, std::size_t p_size, std::size_t s_size)
			: _id(id), _cptime(cptime)
		{
			_successors.reserve(s_size);
			_predecessors.reserve(p_size);
		}

		lwnode::lwnode(const lwnode&& moved)
			: _predecessors(std::move(moved._predecessors)), _successors(std::move(moved._successors)),
			_id(moved._id), _cptime(moved._cptime)
		{
		}

		void lwnode::add_predecessor(const lwnode *node)
		{
			_predecessors.push_back(node);
		}

		void lwnode::add_successor(const lwnode *node)
		{
			_successors.push_back(node);
		}

		std::size_t lwnode::p_size() const
		{
			return _predecessors.size();
		}

		std::size_t lwnode::s_size() const
		{
			return _successors.size();
		}

		const lwnode* lwnode::get_predecessor(size_t index) const
		{
			return _predecessors[index];
		}

		const lwnode* lwnode::get_successor(size_t index) const
		{
			return _successors[index];
		}
	}
}