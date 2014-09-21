#ifndef GRAPHLIB_FITNESSSTRATEGY_H
#define GRAPHLIB_FITNESSSTRATEGY_H

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

		///
		/// This class is a base for the Strategy pattern
		/// to calculate the fitness of a chromsome.
		class FitnessStrategy
		{
			static vector<FitnessStrategy*> examplars;
		public:
			typedef unique_ptr<FitnessStrategy> fsPtr;
			
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options) const = 0;
			virtual fsPtr build(string name) const = 0;

			static void add_fitness_strategy(FitnessStrategy* fs) { examplars.push_back(fs); }
			static fsPtr find_fitness_strategy(std::string name) {
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
			LengthFitnessStartegy(Examplar e) { 
				FitnessStrategy::add_fitness_strategy(this); }
			unsigned int fitness(Solution::sPtr solution, const Options::oPtr options) const;
			fsPtr build(string name) const; 

		};

		class RescheduleIdleTimeFitnessStartegy : public FitnessStrategy
		{
			static RescheduleIdleTimeFitnessStartegy example;
		public:
			RescheduleIdleTimeFitnessStartegy(Examplar e) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options) const;
			virtual fsPtr build(string name) const;
		};

		class PUUsageFitnessStrategy : public FitnessStrategy
		{
			static PUUsageFitnessStrategy example;
		public:
			PUUsageFitnessStrategy(Examplar e) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options) const;
			virtual fsPtr build(string name) const;
		};

		class LoadBalanceFitnessStrategy : public FitnessStrategy
		{
			static LoadBalanceFitnessStrategy example;
		public:
			LoadBalanceFitnessStrategy(Examplar e) { FitnessStrategy::add_fitness_strategy(this); }
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options) const;
			virtual fsPtr build(string name) const;
		};

		class OpenEdgesFitnessStrategy : public FitnessStrategy
		{
		public:
			virtual unsigned int fitness(Solution::sPtr solution, const Options::oPtr options) const;
			virtual fsPtr build(string name) const;
		};
	}
}

#endif