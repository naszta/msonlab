#include "Solution.h"
#include "GraphAlgorithms.h"

namespace msonlab {
	namespace scheduling {

		/// Initialises a new instance of the Solution class.
		/// The size is the number of tasks.
		/*Solution::Solution(unsigned pus, unsigned edges) : fitness(0), pus(pus), edges(edges)
		{
		}*/

		/// Initialises a new instance of the Solution class.
		/// The size is the number of tasks.
		Solution::Solution(size_t size, unsigned pus, unsigned edges) : fitness(0), pus(pus), edges(edges)
		{
			scheduling.resize(size);
			mapping.resize(size);
		}

		/// Copy constructor.
		/// Sets the fitness to zero.
		Solution::Solution(const Solution& solution)
		{
			*this = solution;
			this->fitness = 0;
		}

		/// operator =
		/// It DOES copy the fitness.
		Solution& Solution::operator=(const Solution &solution)
		{
			if (this != &solution)
			{
				this->mapping = solution.mapping;
				this->scheduling = solution.scheduling;
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
			for (size_t i = 0; i < mapping.size(); ++i)
			{
				o << mapping[i] << " ";
			}

			o << " | ";

			for (size_t i = 0; i < scheduling.size(); ++i)
			{
				o << scheduling[i]->getId() << " ";
			}

			o << "| length = " << fitness << std::endl;
		}

		std::ostream& operator<<(std::ostream& os, const Solution& solution)
		{
			solution.printSolution(os);
			return os;
		}

		void Solution::printTable(std::ostream& os, Options::oPtr options) const
		{
			auto tasks = scheduling.size();

			vector<unsigned> ST(tasks);
			unsigned length = GraphAlgorithms::computeLengthAndST(shared_from_this(), options, ST);

			// initialize table
			//unsigned length = *std::max_element(FT.begin(), FT.end());
			vector<vector<int>> table(length);
			for (unsigned i = 0; i < table.size(); ++i)
			{
				table[i].resize(this->pus, -1);

			}
			for (unsigned i = 0; i < this->scheduling.size(); ++i)
			{
				unsigned pu = this->mapping[i];
				unsigned task = this->scheduling[i]->getId();
				unsigned start = ST[task];
				for (unsigned j = 0; j < this->scheduling[i]->getComputationTime(); ++j)
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