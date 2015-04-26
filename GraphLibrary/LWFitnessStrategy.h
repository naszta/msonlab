#ifndef GRAPHLIB_LWFitnessStrategy_H
#define GRAPHLIB_LWFitnessStrategy_H

#include "Solution.h"
#include "Options.h"
#include <string>

namespace msonlab {
	namespace scheduling {

		using std::string;
		using std::unique_ptr;

		namespace {
			class Examplar {};
		}

		class LWLitnessStrategy;
		typedef unique_ptr<LWFitnessStrategy> FSPtr;

		///
		/// This class is a base for the Strategy pattern
		/// to calculate the fitness of a chromsome.
		class LWFitnessStrategy
		{
			static vector<LWFitnessStrategy*> examplars;
		public:
			virtual unsigned int fitness(const Solution &solution, const OptionsPtr options) const = 0;
			virtual FSPtr build(string name) const = 0;

			static void add_fitness_strategy(LWFitnessStrategy* fs) { examplars.push_back(fs); }
			static FSPtr find_fitness_strategy(std::string name) {
				for (auto fs : examplars) {
					auto res = fs->build(name);
					if (res != nullptr) return std::move(res);
				}
				return nullptr;
			}
		};

		class LengthFitnessStartegy : public LWFitnessStrategy
		{
			static LengthFitnessStartegy example;
		public:
			LengthFitnessStartegy(Examplar) { LWFitnessStrategy::add_fitness_strategy(this); }
			unsigned int fitness(const Solution &solution, const OptionsPtr options) const;
			FSPtr build(string name) const;

		};

		class RescheduleIdleTimeFitnessStartegy : public LWFitnessStrategy
		{
			static RescheduleIdleTimeFitnessStartegy example;
		public:
			RescheduleIdleTimeFitnessStartegy(Examplar) { LWFitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(const Solution &solution, const OptionsPtr options) const;
			virtual FSPtr build(string name) const;
		};

		class PUUsageLWFitnessStrategy : public LWFitnessStrategy
		{
			static PUUsageLWFitnessStrategy example;
		public:
			PUUsageLWFitnessStrategy(Examplar) { LWFitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(const Solution &solution, const OptionsPtr options) const;
			virtual FSPtr build(string name) const;
		};

		class LoadBalanceLWFitnessStrategy : public LWFitnessStrategy
		{
			static LoadBalanceLWFitnessStrategy example;
		public:
			LoadBalanceLWFitnessStrategy(Examplar) { LWFitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(const Solution &solution, const OptionsPtr options) const;
			virtual FSPtr build(string name) const;
		};

		class OpenEdgesLWFitnessStrategy : public LWFitnessStrategy
		{
		public:
			virtual unsigned int fitness(const Solution &solution, const OptionsPtr options) const;
			virtual FSPtr build(string name) const;
		};
	}
}

#endif