#ifndef ID_H
#define ID_H

#include <climits>
#include <stdexcept>

namespace graphlib { namespace graph {

	class Id {
	public:
		Id() : _id(UINT_MAX) {}
		
		Id& operator=(const Id& id_)
		{
			validateNotInitialized();
			_id = id_._id;
		}

		unsigned id() const { return _id; }
		operator unsigned() const { return _id;  }

	private:
		unsigned int _id;
		friend class Graph;
		
		void id(unsigned id_)
		{
			validateNotInitialized();
			_id = id_;
		}
		
		void validateNotInitialized()
		{
			if (_id != UINT_MAX)
				throw std::logic_error("Id already initialized");
		}
	};

} }


#endif