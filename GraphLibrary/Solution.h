#pragma once	

#include "Options.h"
#include "litenode.h"
#include <ostream>
#include <vector>
#include <memory>

/// This class represents a solution
/// a solution is described with
/// a mapping and a scheduling part

namespace msonlab {
	namespace scheduling {

		using namespace msonlab;

		class Solution //: public std::enable_shared_from_this<Solution>
		{
		private:
			// the scheduling part, escribes the order of the tasks
			vector<const lite::litenode*> _scheduling;

			// the mapping part, maps the task to PUs.
			vector<unsigned> _mapping;

			// the number of edges
			unsigned int edges;

			// the number of PUs
			unsigned int pus;

			// the score of this solution, the less is better
			unsigned int fitness;

			// the length of the result
			unsigned int length;

			//friend unsigned doComputeLengthSTAndRT(const Solution &solution, const OptionsPtr options,
			//	vector<unsigned>& ST, vector<unsigned>& RT);
			//friend unsigned computeLengthAndReuseIdleTime(Solution& solution, const Options& options);
			//friend bool is_correct(const Solution &sol);

			// scheduling algorithms
			friend class GeneticAlgorithm;
			friend class GreedySchedulingAlgorithm;
			friend class ListSchedulingAlgorithm;
			friend class CriticalPathSchedulingAlgorithm;
			friend class CoffmanGrahamSchedulingAlgorithm;
			friend unsigned computeLengthAndReuseIdleTime(Solution& solution, const Options& options);

			void calcStartTime(OptionsPtr options);
		public:
			friend std::ostream& operator<<(std::ostream& os, const Solution& solution);

			Solution(size_t size, unsigned pus, unsigned edges);
			Solution(const Solution& solution);
			Solution(const Solution&& solution);

			Solution& operator=(const Solution &solution);

			unsigned int getFitness() const { return fitness; }
			void setFitness(unsigned f) {
				if (fitness == 0) {
					fitness = f;
				}
			}
			
			const vector<unsigned>& mapping() const { return _mapping; }
			const vector<const lite::litenode*>& scheduling() const { return _scheduling; }
			// number of scheduled nodes
			size_t size() const { return _scheduling.size(); }

			void printSolution(std::ostream& o) const;
			void printTable(std::ostream& o, OptionsPtr options) const;
		};

		typedef std::shared_ptr<Solution> SolutionPtr;
		typedef vector< SolutionPtr > SolutionVect;
	}
}