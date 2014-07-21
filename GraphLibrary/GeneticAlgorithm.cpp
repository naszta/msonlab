#include "GeneticAlgorithm.h"
#include "Algorithms.h"
#include "CriticalPathSchedulingAlgorithm.h"
#include <cstdlib>
#include <ctime>
#include <set>
#include <queue>
#include <map>
#include <memory>

#include "tbb\task_scheduler_init.h"
#include "tbb\task.h"

//temp
#include "SchedulingHelper.h"

namespace msonlab {
	namespace scheduling {

		GeneticAlgorithm::GeneticAlgorithm(Options::oPtr options, FitnessStrategy::fsPtr strategy)
			: options(options), fsstrategy(strategy)
		{
		}

		///
		/// Constructs a solution using greedy scheduling.
		///
		/// @param The input graph.
		/// @return The constructed solution.
		Solution::sPtr GeneticAlgorithm::greedySolution(Graph::gPtr& graph) const
		{
			unsigned timeCounter = 0;
			unsigned taskCounter = 0;
			std::map< IProcessable::nPtr, unsigned> count;
			IProcessable::nVect inputNodes = graph->getInputNodes();
			std::queue < IProcessable::nPtr> free;
			for (size_t i = 0; i < inputNodes.size(); ++i)
			{
				free.push(inputNodes[i]);
			}

			auto c = std::make_shared<Solution>(graph->numberOfNodes(), options->getNumberOfPus(), graph->numberOfEdges());
			while (taskCounter < graph->numberOfNodes())
			{
				vector< IProcessable::nPtr > out;
				int limit = c->pus;
				while (!free.empty() && limit > 0)
				{
					IProcessable::nPtr node = free.front();
					c->mapping[taskCounter] = limit - 1;
					c->scheduling[taskCounter] = node;

					out.push_back(node);
					free.pop();
					--limit;
					taskCounter++;
				}
				++timeCounter;

				if (taskCounter == graph->numberOfNodes())
				{
					break;
				}

				for (size_t i = 0; i < out.size(); ++i)
				{
					IProcessable::eVect successors = out[i]->getSuccessors();
					for (size_t i = 0; i < successors.size(); ++i)
					{
						count[successors[i]->getTo()]++;
						if (successors[i]->getTo()->getPredecessorsSize() == count[successors[i]->getTo()])
						{
							free.push(successors[i]->getTo());
						}
					}
				}
			}

			fitness(c);
			return c;
		}

		///
		/// Schedules the given graph.
		///
		/// For the schedule the Options of this GeneticAlgorithm is used.
		/// @param graph The graph to schedule.
		/// @return the best solution the GA finds.
		Solution::sPtr GeneticAlgorithm::schedule(Graph::gPtr& graph, Options::oPtr options) const {
			auto set = this->generateInitialSolution(graph, options);
			set->limit();

			bool doOrderCrossover = options->getFitnessStrategy().compare("reschedule") != 0;
			for (size_t i = 0; i < options->getNumberOfYears(); ++i)
			{
				if (options->isParallel()) {
					parallelSimulateMating(set, options->getPopMaxSize(), doOrderCrossover);
				}
				else {
					simulateMating(set, options->getPopMaxSize(), doOrderCrossover);
				}

				set->limit();
			}

			return set->best();
		}

		SolutionSet::setPtr GeneticAlgorithm::generateInitialSolution(Graph::gPtr& graph, Options::oPtr options) const
		{
			SolutionSet::setPtr p;
			if (options->getInitialSolution().compare("cp") == 0) {
				vector<IProcessable::nVect> levels = graph::algorithms::partialTopologicalSort(graph);
				size_t numLevels = levels.size();
				vector<unsigned> levelingLimits;
				unsigned limits = 0;
				for (size_t i = numLevels; i > 0; --i)
				{
					DEBUG("Level: " << i);
					for (auto it = levels[i - 1].begin(); it != levels[i - 1].end(); ++it){
						DEBUG(" " << (*it)->getId());
					}

					DEBUGLN("");
					limits += levels[i - 1].size();
					levelingLimits.push_back(limits);
				}
				p = this->generateCPSolution(graph, options);
				p->setLevelSize(levelingLimits);
			}
			else {
				p = this->generateRndSolution(graph, options);
			}


			return p;
		}

		///
		/// Generating a random initial solution.
		///
		/// The mapping part is generated randomly, each task is randomly mapped to a PU.
		/// The scheduling part is generated using the leveling created by the graph algorithms.
		/// 
		/// @param graph The input graph.
		/// @return The generated population.
		SolutionSet::setPtr GeneticAlgorithm::generateRndSolution(Graph::gPtr& graph, Options::oPtr options) const
		{
			cVect solution;
			vector<IProcessable::nVect> levels = graph::algorithms::partialTopologicalSort(graph);
			size_t numLevels = levels.size();
			vector<unsigned> levelingLimits;
			unsigned limits = 0;
			for (size_t i = numLevels; i > 0; --i)
			{
				DEBUG("Level: " << i);
				for (auto it = levels[i - 1].begin(); it != levels[i - 1].end(); ++it){
					DEBUG(" " << (*it)->getId());
				}

				DEBUGLN("");
				limits += levels[i - 1].size();
			}

			auto set = std::make_unique<SolutionSet>(solution, options->getKeepSize(), options->getPopMaxSize(), options->getKeepBest());

			Solution::sPtr chr = this->greedySolution(graph);

			Solution::sPtr cc = std::make_shared<Solution>(graph->numberOfNodes(), options->getNumberOfPus(), graph->numberOfEdges());
			size_t currentPos = 0;
			unsigned counter = 0;
			for (size_t i = numLevels; i > 0; --i)
			{
				// set mapping for the level
				for (size_t j = 0; j < levels[i - 1].size(); ++j)
				{
					cc->mapping[counter] = j % cc->pus;
					++counter;
				}

				// random shuffle the level
				std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
				std::copy(levels[i - 1].begin(), levels[i - 1].end(), cc->scheduling.begin() + currentPos);
				levelingLimits.push_back(currentPos);
				currentPos += levels[i - 1].size();
			}

			auto initialFitness = fitness(cc);
			DEBUGLN("Population created. Initial fitness : " << initialFitness);
			set->addOffspring(cc);
			set->setLevelSize(levelingLimits);

			// the number of solutions to generate
			counter = options->getPopMaxSize() - 1;
			size_t nodes = graph->numberOfNodes();
			for (; counter > 0; --counter)
			{
				auto sol = std::make_shared<Solution>(graph->numberOfNodes(), options->getNumberOfPus(), graph->numberOfEdges());
				for (unsigned int i = 0; i < nodes; ++i)
				{
					sol->mapping[i] = rand() % sol->pus;
				}

				size_t currentPos = 0;
				for (size_t i = numLevels; i > 0; --i)
				{
					std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
					std::copy(levels[i - 1].begin(), levels[i - 1].end(), sol->scheduling.begin() + currentPos);
					currentPos += levels[i - 1].size();
				}

				fitness(sol);
				set->addOffspring(sol);
			}

			return set;
		}

		// Generate the solution using the scheduling created by the CP scheduler
		SolutionSet::setPtr GeneticAlgorithm::generateCPSolution(Graph::gPtr& graph, Options::oPtr options) const {
			cVect solution;
			auto set = std::make_unique<SolutionSet>(solution, options->getKeepSize(), options->getPopMaxSize(), options->getKeepBest());
			CriticalPathSchedulingAlgorithm cpAlg;
			auto greedy = greedySolution(graph);
			set->addOffspring(greedy);
			auto cpC = cpAlg.schedule(graph, options);
			auto initialFitness = fitness(cpC);
			set->addOffspring(cpC);
			unsigned counter = options->getPopMaxSize() - 2; // CP and greedy
			for (; counter > 0; --counter)
			{
				auto c = std::make_shared<Solution>(graph->numberOfNodes(), options->getNumberOfPus(), graph->numberOfEdges());
				for (unsigned int i = 0; i < c->mapping.size(); ++i)
				{
					c->mapping[i] = rand() % c->pus;
				}

				std::copy(cpC->scheduling.begin(), cpC->scheduling.end(), c->scheduling.begin());
				fitness(c);
				set->addOffspring(c);
			}

			DEBUGLN("Solution set created. Initial fitness : " << initialFitness);
			return set;
		}

		///
		/// calculate fitness for a given chromosem
		///
		/// @param solution Which's fitness is calculated.
		/// @return The fitness of the solution.
		unsigned int GeneticAlgorithm::fitness(Solution::sPtr solution) const
		{
			if (solution->fitness > 0)
			{
				return solution->fitness;
			}

			solution->fitness = this->fsstrategy->fitness(solution, options);
			return solution->fitness;
		}

		///
		/// Crossover operation between the mapping part of solution.
		///
		/// @param father One parent of the offspring.
		/// @param mother Another parent of the offspring.
		/// @return the offspring.
		Solution::sPtr GeneticAlgorithm::crossoverMap(Solution::sPtr father, Solution::sPtr mother) const
		{
			Solution::sPtr offspring = std::make_shared<Solution>(*father);
			uint crossoverPoint = rand() % father->mapping.size();
			std::copy(mother->mapping.begin() + crossoverPoint, mother->mapping.end(), offspring->mapping.begin() + crossoverPoint);

			return offspring;
		}

		///
		/// Crossover operation between the scheduling part of solution.
		///
		/// @param father One parent of the offspring.
		/// @param mother Another parent of the offspring.
		/// @return the offspring.
		Solution::sPtr GeneticAlgorithm::crossoverOrder(Solution::sPtr father, Solution::sPtr mother, const vector<unsigned>& levelingLimits) const
		{
			Solution::sPtr offspring = std::make_shared<Solution>(*father);
			uint crossoverPoint = rand() % levelingLimits.size();

			std::copy(mother->scheduling.begin() + levelingLimits[crossoverPoint], mother->scheduling.end(), offspring->scheduling.begin() + levelingLimits[crossoverPoint]);

			return offspring;
		}

		///
		/// Mutates the mapping part.
		///
		/// Changes the mapping of a task to another PU.
		/// The mutation happens with [MutationPercentage] probablity
		/// and the number of mutation points are [MutationRate].
		///
		/// @param offspring solution to mutate
		void GeneticAlgorithm::mutateMapping(Solution::sPtr& offspring) const
		{
			unsigned rate = rand() % 100;
			if (rate < this->options->getMapMutationRate())
			{
				int position = rand() % offspring->scheduling.size();
				int mutation = rand() % (offspring->pus - 1) + 1;
				offspring->mapping[position] += mutation;
				if (offspring->mapping[position] >= offspring->pus)
				{
					offspring->mapping[position] -= offspring->pus;
				}
			}
		}

		/// Mutates the scheduling part.
		///
		/// The mutation happens with [MutationPercentage] probability.
		/// In case of a mutation a level is choosen and shuffled.
		///
		/// @param offspring The solution to mutate.
		void GeneticAlgorithm::mutateSheduling(Solution::sPtr offspring, const vector<unsigned>& levelingLimits) const
		{
			//unsigned rate = rand() % 100;
			//if (rate < options->getScheduleMutationRate())
			//{
			size_t position = rand() % levelingLimits.size();
			IProcessable::nVect::iterator begin = offspring->scheduling.begin() + levelingLimits[position];
			IProcessable::nVect::iterator end = position == levelingLimits.size() - 1 ? offspring->scheduling.end() : offspring->scheduling.begin() + levelingLimits[position + 1];
			std::random_shuffle(begin, end);
			//}
		}

		///
		/// Generates offsprings.
		/// 
		/// Randomly chooses parents from the population, uses a crossover operation on them and
		/// mutates the offspring. The offspring is added to the population. Totally [offsprings]
		/// number of offsprings are added to the populationn.
		///
		/// @param population A set of solutions to choose the parents from.
		/// @param offsprings The number of offsprings to generate.
		void GeneticAlgorithm::simulateMating(SolutionSet::setPtr& set, int offsprings, bool doOrderCrossover) const
		{
			int faultyGens = 0;
			int mutations = 0;
			for (; offsprings > 0;)
			{
				Solution::sPtr father = set->getParent();
				Solution::sPtr mother = set->getParent();
				int crossoverType = rand() % 2;
				Solution::sPtr offspring;
				// always mapping crossover
				if (crossoverType == 0 || !doOrderCrossover)
				{
					offspring = crossoverMap(father, mother);
				}
				else
				{
					offspring = crossoverOrder(father, mother, set->getLevels());
				}

				mutateMapping(offspring);
				unsigned rate = rand() % 100;
				if (rate < options->getScheduleMutationRate()) {
					mutateSheduling(offspring, set->getLevels());
					++mutations;
				}

				/*bool correct = SchedulingHelper::ensureCorrectness(offspring);
				if (!correct) {
					std::cout << "Sol not correct" << std::endl;
				}
				if (!correct && cost < UINT32_MAX)
				{
					std::cout << "gebasz" << std::endl;

				}*/
				unsigned cost = fitness(offspring);
				if (cost < UINT32_MAX)
				{
					set->addOffspring(offspring);
					--offsprings;
				}
				else ++faultyGens;
			}

			/*DEBUGLN("Mutations happened   " << mutations);
			DEBUGLN("Faulty gens created  " << faultyGens);
			DEBUGLN("Successful mutations " << mutations - faultyGens);*/
		}

		class fitness_calculator : public tbb::task {
			const Solution::sPtr solution;
			const FitnessStrategy::fsPtr strategy;
			const Options::oPtr options;
		public:
			fitness_calculator(const Solution::sPtr sol, const FitnessStrategy::fsPtr strat, const Options::oPtr opt)
				: solution(sol), strategy(strat), options(opt) {}

			tbb::task* execute() {
				unsigned f = strategy->fitness(solution, options);
				solution->setFitness(f);
				return nullptr;
			}
		};

		class round_simulator : public tbb::task {
			const GeneticAlgorithm& alg;
			SolutionSet::setPtr& set;
			int offsprings;
			bool doOrderCrossover;
		public:
			round_simulator(const GeneticAlgorithm& genalg, SolutionSet::setPtr& set, int offsprings, bool doOrderCrossover) :
				alg(genalg), set(set), offsprings(offsprings), doOrderCrossover(doOrderCrossover) {}

			tbb::task* execute() {
				set_ref_count(offsprings + 1);
				vector<Solution::sPtr> new_solutions;
				for (; offsprings > 0; --offsprings)
				{
					Solution::sPtr father = set->getParent();
					Solution::sPtr mother = set->getParent();
					int crossoverType = rand() % 2;
					Solution::sPtr offspring;
					// always mapping crossover, if order is not allowed
					if (crossoverType == 0 || !doOrderCrossover)
					{
						offspring = alg.crossoverMap(father, mother);
					}
					else
					{
						offspring = alg.crossoverOrder(father, mother, set->getLevels());
					}

					alg.mutateMapping(offspring);
					unsigned rate = rand() % 100;
					if (rate < alg.options->getScheduleMutationRate()) {
						alg.mutateSheduling(offspring, set->getLevels());
					}

					new_solutions.push_back(offspring);
					fitness_calculator& fc = *new(allocate_child()) fitness_calculator(offspring, alg.fsstrategy, alg.options);
					spawn(fc);
				}

				wait_for_all();
				for (auto it = new_solutions.begin(); it != new_solutions.end(); ++it) {
					if ((*it)->getFitness() < INT32_MAX)
					{
						set->addOffspring((*it));
					}
				}
				return nullptr;
			}
		};

		void GeneticAlgorithm::parallelSimulateMating(SolutionSet::setPtr& set, int offsprings, bool doOrderCrossover) const
		{
			/* This is the TBB runtime... */
			tbb::task_scheduler_init init;
			
			round_simulator& rs = *new(tbb::task::allocate_root()) round_simulator(*this, set, offsprings, doOrderCrossover);
			tbb::task::spawn_root_and_wait(rs);
		}

		///
		/// Transform the mapping and scheduling into one vector.
		///
		/// Assumes that the id's of the tasks are from 0 to n-1, where n is the number
		/// nodes in the graph.
		///
		/// @param c the solution to transform
		/// @param result the result vector.
		void GeneticAlgorithm::transfromResult(Solution::sPtr c, vector<unsigned>& result) {
			auto mapping = c->getMapping();
			auto scheduling = c->getScheduling();
			size_t tasks = scheduling.size();
			size_t edges = c->edges;
			if (result.size() != tasks + edges) {
				result.resize(tasks + edges);
			}

			for (unsigned i = 0; i < tasks; ++i) {
				result[scheduling[i]->getId()] = mapping[i];
				Node::nPtr node = scheduling[i];
				for (auto it = node->getPredecessorBegin(); it != node->getPredecessorEnd(); ++it)
				{
					result[(*it)->getId()] = mapping[i];
				}
			}
		}

		// EXPERIMENTAL

		void GeneticAlgorithm::swapMutateScheduling(Solution::sPtr offspring) const
		{
			unsigned rate = rand() % 100;
			if (rate < options->getScheduleMutationRate())
			{
				unsigned toswap = rand() % offspring->scheduling.size() - 2;
				std::iter_swap(offspring->scheduling.begin() + toswap,
					offspring->scheduling.begin() + toswap + 1);

			}
		}
	}
}