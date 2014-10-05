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

		class Solution;
		typedef std::shared_ptr<Solution> SolutionPtr;
		typedef vector< SolutionPtr > SolutionVect;

		class Solution : public std::enable_shared_from_this<Solution>
		{
		private:
			// the scheduling part, escribes the order of the tasks
			vector<NodePtr> scheduling;

			// the mapping part, maps the task to PUs.
			vector<unsigned> mapping;

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

			void calcStartTime(OptionsPtr options);
		public:
			friend std::ostream& operator<<(std::ostream& os, const Solution& solution);

			Solution(size_t size, unsigned pus, unsigned edges);
			Solution(const Solution& solution);

			Solution& operator=(const Solution &solution);

			unsigned int getFitness() const { return fitness; }
			void setFitness(unsigned f) {
				if (fitness == 0) {
					fitness = f;
				}
			}
			const vector<unsigned int>& getMapping() const { return mapping; }
			const NodeVect& getScheduling() const { return scheduling; }

			void printSolution(std::ostream& o) const;
			void printTable(std::ostream& o, OptionsPtr options) const;
		};
	}
}