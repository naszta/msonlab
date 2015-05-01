//#include <vector>
//#include "Options.h"
//#include "IProcessable.h"
//#include <ostream>
//#include <vector>
//#include <memory>
//#include <utility>
//
//namespace msonlab { namespace scheduling {
//	template < typename NodeType >
//	SchedulingResult<NodeType>::SchedulingResult(unsigned pus_, unsigned tasks_) : _fitness(0), _pus(pus_) {
//		_mapping.resize(tasks_);
//		_scheduling.resize(tasks_);
//	}
//
//	//template
//
//	template < typename NodeType >
//	SchedulingResult<NodeType>::SchedulingResult(const vector<unsigned>& mapping_, const vector<NodeType> &scheduling_, unsigned fitness_)
//		: _mapping(mapping_), _scheduling(scheduling_), _fitness(fitness_), _pus(mapping_.size())
//	{
//	}
//
//	template < typename NodeType >
//	SchedulingResult<NodeType>::SchedulingResult(vector<unsigned> &&mapping_, vector<NodeType> &&scheduling_, unsigned fitness_)
//		: _mapping(mapping_), _scheduling(scheduling_), _fitness(fitness_), _pus(mapping_.size())
//	{
//	}
//
//	template < typename NodeType >
//	SchedulingResult<NodeType>::SchedulingResult(const SchedulingResult<NodeType> &&result_)
//		: _mapping(result_.mapping), _scheduling(result_.scheduling), _fitness(result_.fitness), _pus(result_.mapping.size())
//	{
//	}
//
//	// Sets the fitness if it wasn't set yet and returns with the fitness of the result.
//	template < typename NodeType >
//	unsigned SchedulingResult<NodeType>::fitness(unsigned fitness_) {
//		if (_fitness == 0) {
//			_fitness = fitness_;
//		}
//
//		return _fitness;
//	}
//}}