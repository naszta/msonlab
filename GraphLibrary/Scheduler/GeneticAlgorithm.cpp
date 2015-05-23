#include "GeneticAlgorithm.h"

#include <cstdlib>
#include <ctime>
#include <set>
#include <queue>
#include <map>
#include <memory>

#include "tbb\task_scheduler_init.h"
#include "tbb\task.h"

#include "../Algorithms.h"
#include "CriticalPathSchedulingAlgorithm.h"
#include "CoffmanGrahamSchedulingAlgorithm.h"
#include "../litegraph.h"
#include "../litenode.h"
#include "SchedulingUtils.h"

namespace msonlab { namespace scheduling {

	GeneticAlgorithm::GeneticAlgorithm(FSPtr strategy)
		: fsstrategy(std::move(strategy))
	{}
		
	//virtual constructor
	SchedulingAlgorithmPtr GeneticAlgorithm::build(const Options& opt) const
	{
		if (opt.algorithm().compare("genetic") == 0 && !opt.isParallel())
		{
			DEBUG("Initializing GeneticAlgorithm with ");
			auto fs = FitnessStrategy::find_fitness_strategy(opt.fitnessStrategy());
			return std::move(std::make_unique<GeneticAlgorithm>(std::move(fs)));
		}

		return nullptr;
	}

	///
	/// Constructs a solution using greedy scheduling.
	///
	/// @param The input graph.
	/// @return The constructed solution.
	SchedulingResultPtr<const lite::litenode*> GeneticAlgorithm::greedySolution(const lite::litegraph &graph, const Options& options) const
	{
		auto &nodes = graph.nodes();
		unsigned timeCounter = 0;
		unsigned taskCounter = 0;
		size_t graphSize = graph.order();
		std::map<unsigned, unsigned> count;
		auto inputNodes = graph.inodes();
		std::queue < unsigned > free(std::deque< unsigned >(inputNodes.begin(), inputNodes.end()));

		vector<unsigned> mapping(graphSize);
		vector<const lite::litenode*> scheduling(graphSize);

		while (taskCounter < graphSize)
		{
			vector< unsigned > scheduled_node_ids;
			int limit = options.numberOfPus();
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
			
		auto result = std::make_shared<SchedulingResult<const lite::litenode*>>(mapping, scheduling, 0, options.numberOfPus());
		fitness(result, options);
		return result;
	}

	///
	/// Schedules the given graph.
	///
	/// For the schedule the Options of this GeneticAlgorithm is used.
	/// @param graph The graph to schedule.
	/// @return the best solution the GA finds.
	SchedulingResultPtr<const NodePtr> GeneticAlgorithm::schedule(const Graph &graph, const Options &options) const
	{
		if (graph.order() == 0) {
			return std::make_shared<SchedulingResult<const NodePtr>>(vector < unsigned > {}, vector < const NodePtr > {}, 0, options.numberOfPus());
		}

		// the light weight graph
		lite::litegraph liteg(graph);
		auto set = this->generateInitialSolution(liteg, options);
		set.limit();

		const unsigned MAX_LOAD = options.popMaxSize();
		bool doOrderCrossover = options.fitnessStrategy().compare("reschedule") != 0;
		for (size_t i = 0; i < options.numberOfYears(); ++i) {
			//DEBUGLN("Round " << i << " Best: " << set->best()->fitness() << " Ultimate: " << set->ultimate()->fitness() << " Avg: " << set->averageFittness());
			simulateMating(set, options.popMaxSize(), doOrderCrossover, options);
			set.limit();
			if (set.last_improvement() >= options.maxRoundsWithoutImprovement()) {
				set.randomized();
				vector<vector<const lite::litenode*>> levels;
				graph::algorithms::constructLayeredOrder<lite::litegraph, const lite::litenode*>(liteg, levels);
					
				for (unsigned counter = 0; counter < MAX_LOAD; ++counter) {
					set.addOffspring(createRandomSolution(liteg, options, levels));
				}
			}
		}

		auto best = set.ultimate();
		vector<const NodePtr> scheduling{ best->size() };
		const auto& hwnodes = graph.getNodes();
		unsigned position = 0;
		for (const auto& litenode : best->scheduling()){
			// linear search
			for (const auto& hwnode : hwnodes) {
				if (hwnode->id() == litenode->id()) {
					scheduling[position] = hwnode;
					++position;
					break;
				}
			}
		}

		return std::make_shared<SchedulingResult<const NodePtr>>(best->mapping(), scheduling, best->fitness(), options.numberOfPus());
	}

	SolutionSet GeneticAlgorithm::generateInitialSolution(const lite::litegraph &graph, const Options &options) const
	{
		if (options.initialSolution().compare("cp") == 0) {
			vector<vector<const lite::litenode*>> levels;
			graph::algorithms::constructLayeredOrder<lite::litegraph, const lite::litenode*>(graph, levels);
			size_t numLevels = levels.size();
			vector<unsigned> levelingLimits;
			unsigned limits = 0;
			for (size_t i = numLevels; i > 0; --i)
			{
				limits += levels[i - 1].size();
				levelingLimits.push_back(limits);
			}
			SolutionSet set (this->generateCPSolution(graph, options));
			set.setLevelSize(levelingLimits);
			return std::move(set);
		}
		else {
			return std::move(this->generateRndSolution(graph, options));
		}

		//return p;
	}


	SchedulingResultPtr<const lite::litenode*> GeneticAlgorithm::createRandomSolution(const lite::litegraph &graph, const Options &options,
		 vector<vector<const lite::litenode*>>& layers) const 
	{
		auto result = std::make_shared<SchedulingResult<const lite::litenode*>>(options.numberOfPus(), graph.order());
		size_t currentLevelSize = 0;
		// counter of the task in the mapping
		unsigned counter = 0;
		for (size_t i = layers.size(); i > 0; --i)
		{
			// set mapping for the level
			for (size_t j = 0; j < layers[i - 1].size(); ++j)
			{
				// accessing private member
				result->_mapping[counter] = j % result->pus();
				++counter;
			}

			// random shuffle the level
			std::random_shuffle(layers[i - 1].begin(), layers[i - 1].end());
			std::copy(layers[i - 1].begin(), layers[i - 1].end(), result->_scheduling.begin() + currentLevelSize);
			currentLevelSize += layers[i - 1].size();
		}

		fitness(result, options);
		return result;
	}

	///
	/// Generating a random initial solution.
	///
	/// The mapping part is generated randomly, each task is randomly mapped to a PU.
	/// The scheduling part is generated using the leveling created by the graph algorithms.
	/// 
	/// @param graph The input graph.
	/// @return The generated population.
	SolutionSet GeneticAlgorithm::generateRndSolution(const lite::litegraph &graph, const Options &options) const
	{
		vector<vector<const lite::litenode*>> levels;
		graph::algorithms::constructLayeredOrder<lite::litegraph, const lite::litenode*>(graph, levels);
		size_t numLevels = levels.size();
		vector<unsigned> levelingLimits;

		// create the set with the given parameters
		auto set = SolutionSet(options.keepSize(), options.popMaxSize(), options.keepBest());

		// add a greedy solution to the set
		auto greedy_solution = this->greedySolution(graph, options);
		//set->addOffspring(greedy_solution);
		DEBUGLN("Greedy fitness " << greedy_solution->fitness());
		// create first solution
		auto result = std::make_shared<SchedulingResult<const lite::litenode*>>(options.numberOfPus(), graph.order());
		size_t currentLevelSize = 0;
		// counter of the task in the mapping
		unsigned counter = 0;
		for (size_t i = numLevels; i > 0; --i)
		{
			// set mapping for the level
			for (size_t j = 0; j < levels[i - 1].size(); ++j)
			{
				// accessing private member
				result->_mapping[counter] = j % result->pus();
				++counter;
			}

			// random shuffle the level
			std::random_shuffle(levels[i - 1].begin(), levels[i - 1].end());
			std::copy(levels[i - 1].begin(), levels[i - 1].end(), result->_scheduling.begin() + currentLevelSize);
			levelingLimits.push_back(currentLevelSize);
			currentLevelSize += levels[i - 1].size();
		}
			
		auto initialFitness = fitness(greedy_solution, options);
		set.addOffspring(greedy_solution);
		DEBUGLN("SolutionSet created. Initial fitness : " << initialFitness);
		fitness(result, options);
		set.addOffspring(result);
		set.setLevelSize(levelingLimits);

		// the number of solutions to generate
		counter = options.popMaxSize() - set.size();
		for (; counter > 0; --counter) {
			set.addOffspring(createRandomSolution(graph, options, levels));
		}

		return std::move(set);
	}

	// Generate the solution using the scheduling created by the CP scheduler
	SolutionSet GeneticAlgorithm::generateCPSolution(const lite::litegraph &graph, const Options &options) const {
		auto set = SolutionSet(options.keepSize(), options.popMaxSize(), options.keepBest());

		CriticalPathSchedulingAlgorithm cpAlg;
		auto cpC = cpAlg.scheduleLite(graph, options);
		auto initialFitness = fitness(cpC, options);
		set.addOffspring(cpC);

		CoffmanGrahamSchedulingAlgorithm cgAlg;
		auto cgC = cgAlg.scheduleLite(graph, options);
		initialFitness = fitness(cgC, options);
		set.addOffspring(cgC);
		while (set.size() < options.popMaxSize()) {
			vector<unsigned> mapping(graph.order());
			vector<const lite::litenode*> scheduling(graph.order());
			for (size_t i = 0; i < mapping.size(); ++i) {
				mapping[i] = rand() % options.numberOfPus();
			}

			std::copy(cpC->_scheduling.begin(), cpC->_scheduling.end(), scheduling.begin());
			auto c = std::make_shared<SchedulingResult<const lite::litenode*>>(std::move(mapping), std::move(scheduling), 0, options.numberOfPus());
			fitness(c, options);
			set.addOffspring(c);
		}

		return std::move(set);
	}

	//
	// Calculates fitness for a given chromosem
	// This is not thread safe.
	/// @param solution which's fitness is calculated.
	/// @return The fitness of the solution.
	unsigned int GeneticAlgorithm::fitness(SchedulingResultPtr<const lite::litenode*> solution, const Options& options) const
	{
		// fitness is zero if it has never been calculated before.
		if (solution->fitness() > 0) {
			return solution->fitness();
		}

		// calculate the fitness only once.
		return solution->fitness(this->fsstrategy->fitness(*solution, options));
	}

	///
	/// Crossover operation between the mapping part of solution.
	///
	/// @param father One parent of the offspring.
	/// @param mother Another parent of the offspring.
	/// @return the offspring.
	SchedulingResultPtr<const lite::litenode*> GeneticAlgorithm::crossoverMap(SchedulingResultPtr<const lite::litenode*> father, SchedulingResultPtr<const lite::litenode*> mother) const
	{
		auto offspring = std::make_shared<SchedulingResult<const lite::litenode*>>(*father);
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
	SchedulingResultPtr<const lite::litenode*> GeneticAlgorithm::crossoverOrder(SchedulingResultPtr<const lite::litenode*> father, SchedulingResultPtr<const lite::litenode*> mother, const vector<unsigned>& levelingLimits) const
	{
		auto offspring = std::make_shared<SchedulingResult<const lite::litenode*>>(*father);
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
	void GeneticAlgorithm::mutateMapping(SchedulingResultPtr<const lite::litenode*> offspring) const
	{
		int position = rand() % offspring->_mapping.size();
		int mutation = rand() % (offspring->pus() - 1) + 1; // to ensure the mapping changes
		offspring->_mapping[position] += mutation;
		// corrigate
		if (offspring->_mapping[position] >= offspring->pus()) {
			offspring->_mapping[position] -= offspring->pus();
		}
	}

	/// Mutates the scheduling part.
	///
	/// The mutation happens with [MutationPercentage] probability.
	/// In case of a mutation a level is choosen and shuffled.
	///
	/// @param offspring The solution to mutate.
	void GeneticAlgorithm::mutateSheduling(SchedulingResultPtr<const lite::litenode*> offspring, const vector<unsigned>& levelingLimits) const
	{
		size_t position = rand() % levelingLimits.size();
		auto begin = offspring->_scheduling.begin() + levelingLimits[position];
		auto end = position == levelingLimits.size() - 1 ? offspring->_scheduling.end() : offspring->_scheduling.begin() + levelingLimits[position + 1];
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
	void GeneticAlgorithm::simulateMating(SolutionSet& set, int offsprings, bool doOrderCrossover, const Options& options) const
	{
		while (set.size() < options.popMaxSize())
		{
			auto father = set.getParent();
			auto mother = set.getParent();
			int crossoverType = rand() % 2;
			SchedulingResultPtr<const lite::litenode*> offspring = nullptr;
			if (crossoverType == 0 || !doOrderCrossover) {
				offspring = crossoverMap(father, mother);
			}
			else {
				offspring = crossoverOrder(father, mother, set.getLevels());
			}

			for (unsigned i = 0; i < offspring->size() / 20; ++i) {
				if (static_cast<unsigned>(rand() % 100) < options.mapMutationRate()) {
					mutateMapping(offspring);
				}

				if (static_cast<unsigned>(rand() % 100) < options.scheduleMutationRate()) {
					mutateSheduling(offspring, set.getLevels());
				}

			}
				
			unsigned cost = fitness(offspring, options);
			// if cost is UINT32_MAX it has a defect
			if (cost < UINT32_MAX) {
				set.addOffspring(offspring);
				--offsprings;
			}
		}
	}

	// EXPERIMENTAL

	void GeneticAlgorithm::swapMutateScheduling(SchedulingResultPtr<const lite::litenode*> offspring, const Options& options) const
	{
		unsigned rate = rand() % 100;
		if (rate < options.scheduleMutationRate())
		{
			unsigned toswap = rand() % offspring->_scheduling.size() - 2;
			std::iter_swap(offspring->_scheduling.begin() + toswap,
				offspring->_scheduling.begin() + toswap + 1);

		}
	}
}}