#include "ParallelGeneticAlgorithm.h"

#include "tbb\task_scheduler_init.h"
#include "tbb\task.h"

#include "SolutionSet.h"
#include "../Options.h"

namespace msonlab { namespace scheduling {

	ParallelGeneticAlgorithm::ParallelGeneticAlgorithm(FSPtr strategy)
		: GeneticAlgorithm(std::move(strategy))
		{}

	//virtual constructor
	SchedulingAlgorithmPtr ParallelGeneticAlgorithm::build(const Options& opt) const
	{
		if (opt.algorithm().compare("genetic") == 0 && opt.isParallel())
		{
			DEBUG("Initializing ParallelGeneticAlgorithm with ");
			auto fs = FitnessStrategy::find_fitness_strategy(opt.fitnessStrategy());
			return std::move(std::make_unique<GeneticAlgorithm>(std::move(fs)));
		}

		return nullptr;
	}

	// parallel task to calculate the fitness of one solution
	// TODO Change SolutionPtr to SchedulingResultPtr
	class fitness_calculator : public tbb::task {
		const SchedulingResultPtr<const lite::litenode*> solution;
		const FitnessStrategy &strategy;
		const Options& options;
	public:
		fitness_calculator(const SchedulingResultPtr<const lite::litenode*> sol, const FitnessStrategy &strat, const Options& opt)
			: solution(sol), strategy(strat), options(opt) {}

		tbb::task* execute() {
			unsigned f = strategy.fitness(*solution, options);
			solution->fitness(f);
			return nullptr;
		}
	};

	//  parallel task to simulate a round
	class round_simulator : public tbb::task {
		round_simulator(const round_simulator& rs) = delete;
		const ParallelGeneticAlgorithm& alg;
		const Options& options;
		SolutionSet& set;
		// number of offsprings to generate
		int offsprings;
		// if true, the scheduling orders might be crossoverd; otherwise they don't
		bool doOrderCrossover;
	public:
		round_simulator(const ParallelGeneticAlgorithm& genalg, const Options& options, SolutionSet& set, int offsprings, bool doOrderCrossover) :
			alg(genalg), options(options), set(set), offsprings(offsprings), doOrderCrossover(doOrderCrossover) {}

		tbb::task* execute() {
			// the total number of tasks created
			set_ref_count(offsprings + 1);
			vector<SchedulingResultPtr<const lite::litenode*>> new_solutions;
			for (; offsprings > 0; --offsprings)
			{
				auto father = set.getParent();
				auto mother = set.getParent();
				int crossoverType = rand() % 2;
				SchedulingResultPtr<const lite::litenode*> offspring = nullptr;
				// aliteays mapping crossover, if order is not allowed
				if (crossoverType == 0 || !doOrderCrossover) {
					offspring = alg.crossoverMap(father, mother);
				}
				else {
					offspring = alg.crossoverOrder(father, mother, set.getLevels());
				}

				if (static_cast<unsigned>(rand() % 100) < options.mapMutationRate()) {
					alg.mutateMapping(offspring);
				}

				if (static_cast<unsigned>(rand() % 100) < options.scheduleMutationRate()) {
					alg.mutateSheduling(offspring, set.getLevels());
				}

				new_solutions.push_back(offspring);
				fitness_calculator& fc = *new(allocate_child()) fitness_calculator(offspring, *alg.fsstrategy.get(), options);
				spawn(fc);
			}

			wait_for_all();
			for (const auto& offspring : new_solutions) {
				if (offspring->fitness() < INT32_MAX)
				{
					set.addOffspring(offspring);
				}
			}
			return nullptr;
		}
	};

	void ParallelGeneticAlgorithm::simulateMating(SolutionSet& set, int offsprings, bool doOrderCrossover, const Options& options) const {
		/* This is the TBB runtime... */
		tbb::task_scheduler_init init;

		round_simulator& rs = *new(tbb::task::allocate_root()) round_simulator(*this, options, set, offsprings, doOrderCrossover);
		tbb::task::spawn_root_and_wait(rs);
	}
}}