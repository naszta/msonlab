#include "Solution.h"
#include "SchedulingHelper.h"
#include <utility>

namespace msonlab {
	namespace scheduling {

		/// Initialises a new instance of the Solution class.
		/// The size is the number of tasks.
		/*Solution::Solution(unsigned pus, unsigned edges) : fitness(0), pus(pus), edges(edges)
		{
		}*/

		/// Initialises a new instance of the Solution class.
		/// The size is the number of tasks.
		Solution::Solution(size_t size, unsigned pus, unsigned edges) : edges(edges), pus(pus), fitness(0), length(0)
		{
			_scheduling.resize(size);
			_mapping.resize(size);
		}

		/// Copy constructor.
		/// Sets the fitness to zero.
		Solution::Solution(const Solution& solution)
		{
			*this = solution;
			this->fitness = 0;
		}

		/// Move constructor.
		/// Sets the fitness to zero.
		Solution::Solution(const Solution&& solution)
			:_scheduling(std::move(solution._scheduling)), _mapping(std::move(solution._mapping)),
			edges(solution.edges), pus(solution.pus), fitness(solution.fitness), length(solution.length)
		{
		}

		/// operator =
		/// It DOES copy the fitness.
		Solution& Solution::operator=(const Solution &solution)
		{
			if (this != &solution)
			{
				this->_mapping = solution._mapping;
				this->_scheduling = solution._scheduling;
				this->fitness = solution.fitness;
				this->pus = solution.pus;
				this->edges = solution.edges;
			}

			return *this;
		}

		/// prints this Solution's instance to the
		/// given output stream
		void Solution::printSolution(std::ostream& o) const
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

			o << "| length = " << fitness << std::endl;
		}

		std::ostream& operator<<(std::ostream& os, const Solution& solution)
		{
			solution.printSolution(os);
			return os;
		}

		void Solution::printTable(std::ostream& os, OptionsPtr options) const
		{
			auto tasks = _scheduling.size();

			vector<unsigned> ST(tasks);
			unsigned length = computeLengthAndST(*this, options, ST);

			// initialize table
			vector<vector<int>> table(length);
			for (unsigned i = 0; i < table.size(); ++i)
			{
				table[i].resize(this->pus, -1);

			}
			for (unsigned i = 0; i < _scheduling.size(); ++i)
			{
				unsigned pu = this->_mapping[i];
				unsigned task = this->_scheduling[i]->id();
				unsigned start = ST[task];
				for (unsigned j = 0; j < _scheduling[i]->cptime(); ++j)
				{
					table[start + j][pu] = task;
				}
			}

			//os.width(3);
			os << "Print table length: " << length << std::endl;
			for (unsigned i = 0; i < table.size(); ++i)
			{
				os.width(2);
				os << i << ": ";
				for (unsigned j = 0; j < table[i].size(); ++j)
				{
					if (table[i][j] >= 0) {
						os.width(3);
						os << table[i][j];
					}
					else
						os << "   ";
					os << " | ";
				}

				os << std::endl;
			}
		}
	}
}