#ifndef GRAPHLIB_SCHEDULERALGORITHM_H
#define GRAPHLIB_SCHEDULERALGORITHM_H

#include <string>
#include <memory>
#include "Solution.h"
#include "Graph.h"

namespace msonlab {
	namespace scheduling {
		
		namespace {
			class examplar {};
		}

		using msonlab::Graph;
		using std::unique_ptr;
		using std::vector;

		///
		/// Abstract class for schedule algorithms.
		///
		class SchedulingAlgorithm
		{
			static vector<SchedulingAlgorithm*> examplars;
			virtual unique_ptr<SchedulingAlgorithm> build(OptionsPtr) const = 0;
		public:
			typedef unique_ptr<SchedulingAlgorithm> ptr;
			virtual SolutionPtr schedule(const Graph &graph, OptionsPtr options) const = 0;

			static void add_scheduling_algorithm(SchedulingAlgorithm* sa) { examplars.push_back(sa); }
			static ptr find_sceduling_algorithm(OptionsPtr options) {
				for (auto sa : examplars) {
					auto res = sa->build(options);
					if (res != nullptr) return std::move(res);
				}
				return nullptr;
			}
		};
	}
}

#endif