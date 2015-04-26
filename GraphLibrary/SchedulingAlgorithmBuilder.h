#ifndef GRAPHLIB_SCHEDULINGALGORITHMBUILDER_H
#define GRAPHLIB_SCHEDULINGALGORITHMBUILDER_H

#include <string>
#include "SchedulingAlgorithm.h"

namespace msonlab {
	namespace scheduling {

		namespace {
			class exemplar {};
		}

		class Options;
		using std::vector;

		// Builder for SchedulingAlgorihms.
		// Creates a scheduling algorithm instance based on it's name.
		class SchedulingAlgorithmBuilder
		{
			static vector<SchedulingAlgorithm*> exemplars;
		public:

			// registers a type of scheduling algorithm to the builder.
			static void add_scheduling_algorithm(SchedulingAlgorithm* sa) {
				exemplars.push_back(sa);
			}

			// tries to create a scheduling algorithm instance by invoking
			// each registered scheduling algorihtm's build function.
			static SchedulingAlgorithmPtr find_sceduling_algorithm(OptionsPtr options) {
				for (auto sa : exemplars) {
					auto res = sa->build(options);
					if (res != nullptr) return std::move(res);
				}

				throw std::exception("Requested algorithm was not found.");
			}
		};
	}
}

#endif