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
		const SchedulingResultPtr<node> solution;
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
		// the place to put the result
		vector<SchedulingResultPtr<node>>& solutions;
		size_t position;
	public:
		round_simulator(const ParallelGeneticAlgorithm& genalg, const Options& options, SolutionSet& set, vector<SchedulingResultPtr<node>>& solutions, size_t position, bool doOrderCrossover) :
			alg(genalg), options(options), set(set), solutions(solutions), position(position), doOrderCrossover(doOrderCrossover) {}

		tbb::task* execute() {
			auto father = set.getParent();
			auto mother = set.getParent();
			int crossoverType = rand() % 2;
			SchedulingResultPtr<node> solution = nullptr;
			// aliteays mapping crossover, if order is not allowed
			if (crossoverType == 0 || !doOrderCrossover) {
				solution = alg.crossoverMap(father, mother);
			}
			else {
				solution = alg.crossoverOrder(father, mother, set.getLevels());
			}

			if (static_cast<unsigned>(rand() % 100) < options.mapMutationRate()) {
				alg.mutateMapping(solution);
			}

			if (static_cast<unsigned>(rand() % 100) < options.scheduleMutationRate()) {
				alg.mutateSheduling(solution, set.getLevels());
			}

			unsigned f = alg.fsstrategy->fitness(*solution, options);
			solution->fitness(f);
			solutions[position] = solution;
			return nullptr;
		}
	};

	class mate_simulator : public tbb::task {
		mate_simulator(const mate_simulator& rs) = delete;
		const ParallelGeneticAlgorithm& alg;
		const Options& options;
		SolutionSet& set;
		// number of offsprings to generate
		int offsprings;
		// if true, the scheduling orders might be crossoverd; otherwise they don't
		bool doOrderCrossover;
	public:
		mate_simulator(const ParallelGeneticAlgorithm& genalg, const Options& options, SolutionSet& set, int offsprings, bool doOrderCrossover) :
			alg(genalg), options(options), set(set), offsprings(offsprings), doOrderCrossover(doOrderCrossover) {}

		tbb::task* execute() {
			set_ref_count(offsprings + 1);
			vector<SchedulingResultPtr<node>> new_solutions(offsprings);

			for (size_t i = 0; i < static_cast<size_t>(offsprings); ++i) {
				// pass a scheduling result ptr to it.
				round_simulator& round = *new(allocate_child()) round_simulator(alg, options, set, new_solutions, i, doOrderCrossover);
				spawn(round);
			}
			wait_for_all();
			for (const auto& r : new_solutions) {
				if (r->fitness() < UINT32_MAX) {
					set.addOffspring(r);
				}
			}
			return nullptr;
		}
	};

	void ParallelGeneticAlgorithm::simulateMating(SolutionSet& set, int offsprings, bool doOrderCrossover, const Options& options) const {
		/* This is the TBB runtime... */
		tbb::task_scheduler_init init;

		mate_simulator& ms = *new(tbb::task::allocate_root()) mate_simulator(*this, options, set, offsprings, doOrderCrossover);
		tbb::task::spawn_root_and_wait(ms);
	}
}}