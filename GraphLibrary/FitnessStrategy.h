#ifndef GRAPHLIB_FITNESSSTRATEGY_H
#define GRAPHLIB_FITNESSSTRATEGY_H

#include "Solution.h"
#include "Options.h"
#include "SchedulingResult.h"
#include "litenode.h"
#include <string>

namespace msonlab {
	namespace scheduling {

		using std::string;
		using std::unique_ptr;

		namespace {
			class Examplar {};
		}

		class FitnessStrategy;
		typedef unique_ptr<FitnessStrategy> FSPtr;

		///
		/// This class is a base for the Strategy pattern
		/// to calculate the fitness of a chromsome.
		class FitnessStrategy
		{
			static vector<FitnessStrategy*> examplars;
		public:
			virtual unsigned int fitness(const SchedulingResult<const lite::litenode*> &SolutionType, const Options& options) const = 0;
			virtual FSPtr build(string name) const = 0;

			static void add_fitness_strategy(FitnessStrategy* fs) { examplars.push_back(fs); }
			static FSPtr find_fitness_strategy(std::string name) {
				for (auto fs : examplars) {
					auto res = fs->build(name);
					if (res != nullptr) return std::move(res);
				}
				return nullptr;
			}
		};

		class LengthFitnessStartegy : public FitnessStrategy
		{
			static LengthFitnessStartegy example;
		public:
			LengthFitnessStartegy() = default;
			LengthFitnessStartegy(Examplar) { FitnessStrategy::add_fitness_strategy(this); }
			unsigned int fitness(const SchedulingResult<const lite::litenode*> &solution, const Options& options) const;
			FSPtr build(string name) const; 
		};


		class RescheduleIdleTimeFitnessStartegy : public FitnessStrategy
		{
			static RescheduleIdleTimeFitnessStartegy example;
		public:
			RescheduleIdleTimeFitnessStartegy(Examplar) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(const SchedulingResult<const lite::litenode*> &solution, const Options& options) const;
			virtual FSPtr build(string name) const;
		};

		class PUUsageFitnessStrategy : public FitnessStrategy
		{
			static PUUsageFitnessStrategy example;
		public:
			PUUsageFitnessStrategy(Examplar) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(const SchedulingResult<const lite::litenode*> &solution, const Options& options) const;
			virtual FSPtr build(string name) const;
		};

		class LoadBalanceFitnessStrategy : public FitnessStrategy
		{
			static LoadBalanceFitnessStrategy example;
		public:
			LoadBalanceFitnessStrategy(Examplar) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(const SchedulingResult<const lite::litenode*> &solution, const Options& options) const;
			virtual FSPtr build(string name) const;
		};

		/*class OpenEdgesFitnessStrategy : public FitnessStrategy
		{
		public:
			virtual unsigned int fitness(const SchedulingResult<const lite::litenode*> &solution, const Options& options) const;
			virtual FSPtr build(string name) const;
		};*/
	}
}

#endif