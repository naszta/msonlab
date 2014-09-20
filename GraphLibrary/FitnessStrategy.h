#ifndef GRAPHLIB_FITNESSSTRATEGY_H
#define GRAPHLIB_FITNESSSTRATEGY_H

#include "Solution.h"
#include "Options.h"
#include <string>

namespace msonlab {
	namespace scheduling {

		using std::string;
		using std::shared_ptr;

		namespace {
			class Examplar {};
		}

		///
		/// This class is a base for the Strategy pattern
		/// to calculate the fitness of a chromsome.
		class FitnessStrategy
		{
			static vector<FitnessStrategy*> examplars;
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options) = 0;
			virtual shared_ptr<FitnessStrategy> build(string name) = 0;

			typedef shared_ptr<FitnessStrategy> fsPtr;
			static void add_fitness_strategy(FitnessStrategy* fs) { examplars.push_back(fs); }
			static shared_ptr<FitnessStrategy> find_fitness_strategy(std::string name) {
				for (auto fs : examplars) {
					auto res = fs->build(name);
					if (res != nullptr) return res;
				}
				return nullptr;
			}
		};

		class LengthFitnessStartegy : public FitnessStrategy
		{
			static LengthFitnessStartegy example;
		public:
			LengthFitnessStartegy(Examplar e) { 
				FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
			virtual shared_ptr<FitnessStrategy> build(string name);

		};

		class RescheduleIdleTimeFitnessStartegy : public FitnessStrategy
		{
			static RescheduleIdleTimeFitnessStartegy example;
		public:
			RescheduleIdleTimeFitnessStartegy(Examplar e) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
			virtual shared_ptr<FitnessStrategy> build(string name);
		};

		class PUUsageFitnessStrategy : public FitnessStrategy
		{
			static PUUsageFitnessStrategy example;
		public:
			PUUsageFitnessStrategy(Examplar e) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
			virtual shared_ptr<FitnessStrategy> build(string name);
		};

		class LoadBalanceFitnessStrategy : public FitnessStrategy
		{
			static LoadBalanceFitnessStrategy example;
		public:
			LoadBalanceFitnessStrategy(Examplar e) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
			virtual shared_ptr<FitnessStrategy> build(string name);
		};

		class OpenEdgesFitnessStrategy : public FitnessStrategy
		{
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options);
			virtual shared_ptr<FitnessStrategy> build(string name);
		};
	}
}

#endif