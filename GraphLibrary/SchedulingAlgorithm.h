#ifndef GRAPHLIB_SCHEDULERALGORITHM_H
#define GRAPHLIB_SCHEDULERALGORITHM_H

#include <string>
#include <memory>
#include "Solution.h"
#include "SchedulingResult.h"
#include "Graph.h"

namespace msonlab {
	namespace scheduling {
		
		/*namespace {
			class examplar {};
		}*/

		using msonlab::Graph;
		using std::unique_ptr;
		using std::vector;

		class SchedulingAlgorithm;
		typedef unique_ptr<SchedulingAlgorithm> SchedulingAlgorithmPtr;

		///
		/// Abstract class for schedule algorithms.
		///
		class SchedulingAlgorithm
		{
			friend class SchedulingAlgorithmBuilder;
			//static vector<SchedulingAlgorithm*> exemplars;
			virtual unique_ptr<SchedulingAlgorithm> build(OptionsPtr) const = 0;
		public:
			
			virtual SchedulingResultPtr<const NodePtr> schedule(const Graph &graph, const Options &options) const = 0;

			/*static void add_scheduling_algorithm(SchedulingAlgorithm* sa) { 
				SchedulingAlgorithm::exemplars.push_back(sa);
			}

			static SchedulingAlgorithmPtr find_sceduling_algorithm(OptionsPtr options) {
				for (auto sa : SchedulingAlgorithm::exemplars) {
					auto res = sa->build(options);
					if (res != nullptr) return std::move(res);
				}

				throw std::exception("Requested algorithm was not found.");
				return nullptr;
			}*/
		};
	}
}

#endif