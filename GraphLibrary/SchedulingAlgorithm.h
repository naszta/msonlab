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
			virtual unique_ptr<SchedulingAlgorithm> build(Options::oPtr) const = 0;
		public:
			typedef unique_ptr<SchedulingAlgorithm> ptr;
			virtual Solution::sPtr schedule(const Graph &graph, Options::oPtr options) const = 0;

			static void add_scheduling_algorithm(SchedulingAlgorithm* sa) { examplars.push_back(sa); }
			static ptr find_sceduling_algorithm(Options::oPtr options) {
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