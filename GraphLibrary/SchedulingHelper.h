/**
  *
  * Date: 2014.07.17 13:21
  * Author: Zoltan
  */
#ifndef GRAPHLIB_SCHEDULINGHELPER_H
#define GRAPHLIB_SCHEDULINGHELPER_H

#include <vector>

#include "Options.h"
#include "Solution.h"

namespace msonlab {
	namespace scheduling {

		using std::vector;

		class SchedulingHelper {
			static unsigned int doComputeLengthSTAndRT(const Solution &solution, OptionsPtr options,
			vector<unsigned>& ST, vector<unsigned>& RT);
		public:
			///
			/// Computes the length of the solution with the given option.
			///
			/// @param solution the result to use
			/// @param options the options to use
			/// @return the length
			static unsigned int computeLength(const Solution &solution, const OptionsPtr options);

			static unsigned int computeLengthAndST(const Solution &solution, const OptionsPtr options,
				vector<unsigned>& ST);

			static unsigned int computeLengthAndRT(const Solution &solution, const OptionsPtr options,
				vector<unsigned>& RT);

			static unsigned int computeLengthSTAndRT(const Solution &solution, const OptionsPtr options,
				vector<unsigned>& ST, vector<unsigned>& RT);

			static unsigned int computeLengthAndReuseIdleTime(Solution& solution, const OptionsPtr& options);

			static bool is_correct(const Solution &sol);
		};
	}
}

#endif