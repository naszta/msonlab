#include "GeneticAlgorithm.h"
#include "Algorithms.h"
#include "CriticalPathSchedulingAlgorithm.h"
#include "lwgraph.h"
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

		GeneticAlgorithm::GeneticAlgorithm(OptionsPtr options, FSPtr strategy)
			: options(options), fsstrategy(std::move(strategy))
		{
		}

		GeneticAlgorithm GeneticAlgorithm::example{ examplar() };
		
		//virtual constructor
		SchedulingAlgorithmPtr GeneticAlgorithm::build(OptionsPtr opt) const
		{
			if (opt->getAlgorithm().compare("genetic") == 0)
			{
				auto fs = FitnessStrategy::find_fitness_strategy(opt->getFitnessStrategy());
				return std::move(std::make_unique<GeneticAlgorithm>(opt, std::move(fs)));
			}

			return nullptr;
		}

		///
		/// Constructs a solution using greedy scheduling.
		///
		/// @param The input graph.
		/// @return The constructed solution.
		SchedulingResultLWPtr GeneticAlgorithm::greedySolution(const lw::lwgraph &graph) const
		{
			const vector<lw::lwnode> &nodes = graph.nodes();
			unsigned timeCounter = 0;
			unsigned taskCounter = 0;
			size_t graphSize = graph.size();
			std::map<unsigned, unsigned> count;
			auto inputNodes = graph.inodes();
			std::queue < unsigned > free(std::deque< unsigned >(inputNodes.begin(), inputNodes.end()));

			//auto c = std::make_shared<Solution>(graphSize, options->getNumberOfPus(), graph.edge_size());
			vector<uint> mapping(graphSize);
			vector<const lw::lwnode*> scheduling(graphSize);

			while (taskCounter < graphSize)
			{
				vector< unsigned > scheduled_node_ids;
				int limit = options->getNumberOfPus();
				while (!free.empty() && limit > 0)
				{
					auto node_id = free.front();
					mapping[taskCounter] = limit - 1;
					scheduling[taskCounter] = &nodes[node_id];

					scheduled_node_ids.push_back(node_id);
					free.pop();
					--limit;
					taskCounter++;
				}
				++timeCounter;

				// if all the nodes are scheduled, stop the algo
				if (taskCounter == graphSize)
				{
					break;
				}

				for (auto scheduled_node_id : scheduled_node_ids)
				{
					auto& node = nodes[scheduled_node_id];
					for (size_t i = 0; i < node.s_size(); ++i)
					{
						auto suc_node = node.get_successor(i);
						auto processed_successors = ++count[suc_node->id()];
						if (suc_node->p_size() == processed_successors)
						{
							free.push(suc_node->id());
						}
					}
				}
			}

			auto res = std::make_shared<SchedulingResultLW>(std::move(mapping), std::move(scheduling), 0);
			fitness(res);
			return res;
		}

		///
		/// Schedules the given graph.
		///
		/// For the schedule the Options of this GeneticAlgorithm is used.
		/// @param graph The graph to schedule.
		/// @return the best solution the GA finds.
		SolutionPtr GeneticAlgorithm::schedule(const Graph &graph, OptionsPtr options) const 
		{
			lw::lwgraph lwg(graph);
			SolutionSet::setPtr set = this->generateInitialSolution(lwg, options);
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

		SolutionSet::setPtr GeneticAlgorithm::generateInitialSolution(const lw::lwgraph &graph, OptionsPtr options) const
		{
			SolutionSet::setPtr p;
			if (options->getInitialSolution().compare("cp") == 0) {
				vector<vector<const lw::lwnode*>> levels;
				graph::algorithms::partialTopologicalSort<lw::lwgraph, const lw::lwnode*>(graph, levels);
				size_t numLevels = levels.size();
				vector<unsigned> levelingLimits;
				unsigned limits = 0;
				for (size_t i = numLevels; i > 0; --i)
				{
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
		SolutionSet::setPtr GeneticAlgorithm::generateRndSolution(const lw::lwgraph &graph, OptionsPtr options) const
		{
			cVect solution; // delete this
			vector<vector<const lw::lwnode*>> levels;
			graph::algorithms::partialTopologicalSort<lw::lwgraph, const lw::lwnode*>(graph, levels);
			size_t numLevels = levels.size();
			vector<unsigned> levelingLimits;

			auto set = std::make_shared<SolutionSet>(solution, options->getKeepSize(), options->getPopMaxSize(), options->getKeepBest());

			SolutionPtr chr = this->greedySolution(graph);

			SolutionPtr cc = std::make_shared<Solution>(graph.size(), options->getNumberOfPus(), graph.edge_size());
			size_t currentPos = 0;
			unsigned counter = 0;
			for (size_t i = numLevels; i > 0; --i)
			{
				// set mapping for the level
				for (size_t j = 0; j < levels[i - 1].size(); ++j)
				{
					cc->_mapping[counter] = j % cc->pus;
					++counter;
				}

				// random shuffle the level
				std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
				std::copy(levels[i - 1].begin(), levels[i - 1].end(), cc->_scheduling.begin() + currentPos);
				levelingLimits.push_back(currentPos);
				currentPos += levels[i - 1].size();
			}

			auto initialFitness = fitness(cc);
			DEBUGLN("SolutionSet created. Initial fitness : " << initialFitness);
			set->addOffspring(cc);
			set->setLevelSize(levelingLimits);

			// the number of solutions to generate
			counter = options->getPopMaxSize() - 1;
			size_t nodes = graph.size();
			for (; counter > 0; --counter)
			{
				auto sol = std::make_shared<Solution>(graph.size(), options->getNumberOfPus(), graph.edge_size());
				for (unsigned int i = 0; i < nodes; ++i)
				{
					sol->_mapping[i] = rand() % sol->pus;
				}

				size_t currentPos = 0;
				for (size_t i = numLevels; i > 0; --i)
				{
					std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
					std::copy(levels[i - 1].begin(), levels[i - 1].end(), sol->_scheduling.begin() + currentPos);
					currentPos += levels[i - 1].size();
				}

				fitness(sol);
				set->addOffspring(sol);
			}

			return set;
		}

		// Generate the solution using the scheduling created by the CP scheduler
		SolutionSet::setPtr GeneticAlgorithm::generateCPSolution(const lw::lwgraph &graph, OptionsPtr options) const {
			cVect solution;
			auto set = std::make_shared<SolutionSet>(solution, options->getKeepSize(), options->getPopMaxSize(), options->getKeepBest());
			CriticalPathSchedulingAlgorithm cpAlg;
			auto greedy = greedySolution(graph);
			set->addOffspring(greedy);
			SolutionPtr cpC = nullptr;
			//auto cpC = cpAlg.schedule(graph, options);
			auto initialFitness = fitness(cpC);
			set->addOffspring(cpC);
			unsigned counter = options->getPopMaxSize() - 2; // CP and greedy
			for (; counter > 0; --counter)
			{
				auto c = std::make_shared<Solution>(graph.size(), options->getNumberOfPus(), graph.edge_size());
				for (size_t i = 0; i < c->_mapping.size(); ++i) {
					c->_mapping[i] = rand() % c->pus;
				}

				std::copy(cpC->_scheduling.begin(), cpC->_scheduling.end(), c->_scheduling.begin());
				fitness(c);
				set->addOffspring(c);
			}

			return set;
		}

		///
		/// calculate fitness for a given chromosem
		///
		/// @param solution Which's fitness is calculated.
		/// @return The fitness of the solution.
		unsigned int GeneticAlgorithm::fitness(SchedulingResultLWPtr solution) const
		{
			// fitness is zero if it has never been calculated before
			if (solution->fitness > 0) {
				return solution->fitness;
			}

			solution->fitness = this->fsstrategy->fitness(*solution, options);
			return solution->fitness;
		}

		///
		/// Crossover operation between the mapping part of solution.
		///
		/// @param father One parent of the offspring.
		/// @param mother Another parent of the offspring.
		/// @return the offspring.
		SolutionPtr GeneticAlgorithm::crossoverMap(SolutionPtr father, SolutionPtr mother) const
		{
			SolutionPtr offspring = std::make_shared<Solution>(*father);
			uint crossoverPoint = rand() % father->_mapping.size();
			std::copy(mother->_mapping.begin() + crossoverPoint, mother->_mapping.end(), offspring->_mapping.begin() + crossoverPoint);

			return offspring;
		}

		///
		/// Crossover operation between the scheduling part of solution.
		///
		/// @param father One parent of the offspring.
		/// @param mother Another parent of the offspring.
		/// @return the offspring.
		SolutionPtr GeneticAlgorithm::crossoverOrder(SolutionPtr father, SolutionPtr mother, const vector<unsigned>& levelingLimits) const
		{
			SolutionPtr offspring = std::make_shared<Solution>(*father);
			uint crossoverPoint = rand() % levelingLimits.size();

			std::copy(mother->_scheduling.begin() + levelingLimits[crossoverPoint], mother->_scheduling.end(), offspring->_scheduling.begin() + levelingLimits[crossoverPoint]);

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
		void GeneticAlgorithm::mutateMapping(SolutionPtr offspring) const
		{
			int position = rand() % offspring->_mapping.size();
			int mutation = rand() % (offspring->pus - 1) + 1; // to ensure the mapping changes
			offspring->_mapping[position] += mutation;
			// corrigate
			if (offspring->_mapping[position] >= offspring->pus) {
				offspring->_mapping[position] -= offspring->pus;
			}
		}

		/// Mutates the scheduling part.
		///
		/// The mutation happens with [MutationPercentage] probability.
		/// In case of a mutation a level is choosen and shuffled.
		///
		/// @param offspring The solution to mutate.
		void GeneticAlgorithm::mutateSheduling(SolutionPtr offspring, const vector<unsigned>& levelingLimits) const
		{
			size_t position = rand() % levelingLimits.size();
			lw::LWNodeVect::iterator begin = offspring->_scheduling.begin() + levelingLimits[position];
			lw::LWNodeVect::iterator end = position == levelingLimits.size() - 1 ? offspring->_scheduling.end() : offspring->_scheduling.begin() + levelingLimits[position + 1];
			std::random_shuffle(begin, end);
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
			for (; offsprings > 0; --offsprings)
			{
				SolutionPtr father = set->getParent();
				SolutionPtr mother = set->getParent();
				int crossoverType = rand() % 2;
				SolutionPtr offspring;
				if (crossoverType == 0 || !doOrderCrossover) {
					offspring = crossoverMap(father, mother);
				}
				else {
					offspring = crossoverOrder(father, mother, set->getLevels());
				}

				if ((rand() % 100) < options->getMapMutationRate()) {
					mutateMapping(offspring);
				}

				if ((rand() % 100) < options->getScheduleMutationRate()) {
					mutateSheduling(offspring, set->getLevels());
				}

				unsigned cost = fitness(offspring);
				// cost is UINT32_MAX it has a defect
				if (cost < UINT32_MAX) {
					set->addOffspring(offspring);
				}
			}
		}

		// parallel task to calculate the fitness of one solution
		class fitness_calculator : public tbb::task {
			const SolutionPtr solution;
			const FitnessStrategy &strategy;
			const OptionsPtr options;
		public:
			fitness_calculator(const SolutionPtr sol, const FitnessStrategy &strat, const OptionsPtr opt)
				: solution(sol), strategy(strat), options(opt) {}

			tbb::task* execute() {
				unsigned f = strategy.fitness(*solution, options);
				solution->setFitness(f);
				return nullptr;
			}
		};

		//  parallel task to simulate a round
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
				vector<SolutionPtr> new_solutions;
				for (; offsprings > 0; --offsprings)
				{
					SolutionPtr father = set->getParent();
					SolutionPtr mother = set->getParent();
					int crossoverType = rand() % 2;
					SolutionPtr offspring;
					// always mapping crossover, if order is not allowed
					if (crossoverType == 0 || !doOrderCrossover) {
						offspring = alg.crossoverMap(father, mother);
					}
					else {
						offspring = alg.crossoverOrder(father, mother, set->getLevels());
					}

					if ((rand() % 100) < alg.options->getMapMutationRate()) {
						alg.mutateMapping(offspring);
					}

					if ((rand() % 100) < alg.options->getScheduleMutationRate()) {
						alg.mutateSheduling(offspring, set->getLevels());
					}

					new_solutions.push_back(offspring);
					fitness_calculator& fc = *new(allocate_child()) fitness_calculator(offspring, *alg.fsstrategy.get(), alg.options);
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
		//void GeneticAlgorithm::transfromResult(SolutionPtr c, vector<unsigned>& result) {
		//	auto mapping = c.mapping();
		//	auto scheduling = c.getScheduling();
		//	size_t tasks = scheduling.size();
		//	size_t edges = c->edges;
		//	if (result.size() != tasks + edges) {
		//		result.resize(tasks + edges);
		//	}

		//	for (unsigned i = 0; i < tasks; ++i) {
		//		result[scheduling[i]->getId()] = mapping[i];
		//		NodePtr node = scheduling[i];
		//		for (auto it = node->getPredecessorBegin(); it != node->getPredecessorEnd(); ++it)
		//		{
		//			result[(*it)->getId()] = mapping[i];
		//		}
		//	}
		//}

		// EXPERIMENTAL

		void GeneticAlgorithm::swapMutateScheduling(SolutionPtr offspring) const
		{
			unsigned rate = rand() % 100;
			if (rate < options->getScheduleMutationRate())
			{
				unsigned toswap = rand() % offspring->_scheduling.size() - 2;
				std::iter_swap(offspring->_scheduling.begin() + toswap,
					offspring->_scheduling.begin() + toswap + 1);

			}
		}
	}
}