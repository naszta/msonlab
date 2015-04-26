#ifndef SCHEDULING_RESULT_H
#define SCHEDULING_RESULT_H

#include "Options.h"
#include "IProcessable.h"
#include <ostream>
#include <vector>
#include <memory>
#include <utility>

/// This class represents a solution
/// a solution is described with
/// a mapping and a scheduling part

using namespace std;

namespace msonlab {
	namespace scheduling {

		template <typename NodeType>
		class SchedulingResult
		{
		public:
			// constructors
			SchedulingResult(unsigned pus_, unsigned tasks_);
			SchedulingResult(const vector<NodeType> &scheduling_);
			SchedulingResult(const vector<unsigned> &mapping_, const vector<NodeType> &scheduling_, unsigned fitness_ = 0);
			SchedulingResult(vector<unsigned>&& mapping_, vector<NodeType> &&scheduling_, unsigned fitness_ = 0);
			SchedulingResult(const SchedulingResult<NodeType> &result_) = default;
			SchedulingResult(const SchedulingResult<NodeType> &&result_);

			// assignment operators
			//SchedulingResult<NodeType>& operator=(const SchedulingResult<NodeType> &result) = default;
			//SchedulingResult<NodeType>& operator=(const SchedulingResult<NodeType> &&result) = default;

			// gets the fitness
			unsigned int fitness() const { return _fitness; }
			// gets the mapping
			const vector<unsigned>& mapping() const { return _mapping; }
			// gets the scheduling
			const vector<NodeType>& scheduling() const { return _scheduling; }
			// gets the number of processing units used
			unsigned pus() const { return _pus; }
			// gets the size of the result - the number of tasks
			size_t size() const { return _mapping.size(); }

			// sets the fitness of the result
			unsigned fitness(unsigned);

			// methods to display
			void printSolution(std::ostream& o) const;

		private:
			// the scheduling part, escribes the order of the tasks
			vector<NodeType> _scheduling;

			// the mapping part, maps the task to PUs.
			vector<unsigned> _mapping;

			// the score of this solution, the less is better
			unsigned int _fitness;

			// number of processing units
			unsigned _pus;

			// scheduling algorithms for setting the result
			friend class GeneticAlgorithm;
			friend class GreedySchedulingAlgorithm;
			friend class ListSchedulingAlgorithm;
			friend class CriticalPathSchedulingAlgorithm;
			friend class CoffmanGrahamSchedulingAlgorithm;
		};

		template < typename NodeType >
		SchedulingResult<NodeType>::SchedulingResult(unsigned pus_, unsigned tasks_) : _fitness(0), _pus(pus_) {
			_mapping.resize(tasks_);
			_scheduling.resize(tasks_);
		}

		//template

		template < typename NodeType >
		SchedulingResult<NodeType>::SchedulingResult(const vector<unsigned>& mapping_, const vector<NodeType> &scheduling_, unsigned fitness_)
			: _mapping(mapping_), _scheduling(scheduling_), _fitness(fitness_), _pus(mapping_.size())
		{
		}

		template < typename NodeType >
		SchedulingResult<NodeType>::SchedulingResult(vector<unsigned> &&mapping_, vector<NodeType> &&scheduling_, unsigned fitness_)
			: _mapping(mapping_), _scheduling(scheduling_), _fitness(fitness_), _pus(mapping_.size())
		{
		}

		template < typename NodeType >
		SchedulingResult<NodeType>::SchedulingResult(const SchedulingResult<NodeType> &&result_)
			: _mapping(result_.mapping), _scheduling(result_.scheduling), _fitness(result_.fitness), _pus(result_.mapping.size())
		{
		}

		// Sets the fitness if it wasn't set yet and returns with the fitness of the result.
		template < typename NodeType >
		unsigned SchedulingResult<NodeType>::fitness(unsigned fitness_) {
			if (_fitness == 0) {
				_fitness = fitness_;
			}

			return _fitness;
		}

		/// prints this Solution's instance to the
		/// given output stream
		template < typename NodeType >
		void SchedulingResult<NodeType>::printSolution(std::ostream& o) const
		{
			for (size_t i = 0; i < _mapping.size(); ++i)
			{
				o << _mapping[i] << " ";
			}

			o << " | ";

			for (size_t i = 0; i < _scheduling.size(); ++i)
			{
				o << _scheduling[i]->id() << " ";
			}

			o << "| length = " << _fitness << std::endl;
		}

		//typedef std::shared_ptr<SchedulingResult<const NodePtr>> SchedulingResultPtr2;

		template<typename NodeType>
		using SchedulingResultPtr = std::shared_ptr < SchedulingResult < NodeType > >;
		//typedef std::shared_ptr<SchedulingResult<const LWNodePtr>> SchedulingResultLWPtr;
	}
}

#endif