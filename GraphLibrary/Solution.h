#pragma once	

#include "Options.h"
#include "Graph.h"
#include <ostream>
#include <vector>
#include <memory>

/// This class represents a solution
/// a solution is described with
/// a mapping and a scheduling part

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class Solution : public std::enable_shared_from_this<Solution>
		{
		private:
			// the scheduling part, escribes the order of the tasks
			IProcessable::nVect scheduling;

			// the mapping part, maps the task to PUs.
			vector<unsigned int> mapping;

			// the number of edges
			unsigned int edges;

			// the number of PUs
			unsigned int pus;

			// the score of this solution, the less is better
			unsigned int fitness;

			// the length of the result
			unsigned int length;

			friend class SchedulingHelper;

			// scheduling algorithms
			friend class GeneticAlgorithm;
			friend class GreedySchedulingAlgorithm;
			friend class ListSchedulingAlgorithm;
			friend class CriticalPathSchedulingAlgorithm;
			friend class CoffmanGrahamSchedulingAlgorithm;

			void calcStartTime(Options::oPtr options);
		public:
			friend std::ostream& operator<<(std::ostream& os, const Solution& solution);
			typedef std::shared_ptr<Solution> sPtr;
			typedef std::shared_ptr<const Solution> csPtr;
			typedef vector< sPtr > cVect;

			//Solution(unsigned pus, unsigned edges);
			Solution(size_t size, unsigned pus, unsigned edges);
			Solution(const Solution& solution);

			Solution& operator=(const Solution &solution);

			unsigned int getFitness() const { return fitness; }
			void setFitness(unsigned f) {
				if (fitness == 0) {
					fitness = f;
				}
				else {
					// throw exception, this cannot happen
				}
			}
			const vector<unsigned int>& getMapping() const { return mapping; }
			const IProcessable::nVect& getScheduling() const { return scheduling; }

			void printSolution(std::ostream& o) const;
			void printTable(std::ostream& o, Options::oPtr options) const;
		};
	}
}