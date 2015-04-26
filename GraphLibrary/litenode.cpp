#include "litenode.h"
#include <utility>

namespace msonlab { namespace lite {
	using std::size_t;

	litenode::litenode(unsigned id, unsigned cptime, std::size_t p_size, std::size_t s_size)
		: _id(id), _cptime(cptime)
	{
		_successors.reserve(s_size);
		_predecessors.reserve(p_size);
	}

	litenode::litenode(litenode&& moved)
		: _predecessors(std::move(moved._predecessors)), _successors(std::move(moved._successors)),
		_id(moved._id), _cptime(moved._cptime)
	{
	}

	litenode& litenode::operator=(litenode&& moved)
	{
		_predecessors.swap(moved._predecessors);
		_successors.swap(moved._successors);
		_id = moved._id;
		_cptime = moved._cptime;
		return *this;
	}

	void litenode::add_predecessor(const litenode *node)
	{
		_predecessors.push_back(node);
	}

	void litenode::add_successor(const litenode *node)
	{
		_successors.push_back(node);
	}

	std::size_t litenode::p_size() const
	{
		return _predecessors.size();
	}

	std::size_t litenode::s_size() const
	{
		return _successors.size();
	}

	const litenode* litenode::get_predecessor(size_t index) const
	{
		return _predecessors[index];
	}

	const litenode* litenode::get_successor(size_t index) const
	{
		return _successors[index];
	}
}}