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
			static unsigned int doComputeLengthSTAndRT(Solution::csPtr solution, Options::oPtr options,
			vector<unsigned>& ST, vector<unsigned>& RT);
		public:
			///
			/// Computes the length of the solution with the given option.
			///
			/// @param solution the result to use
			/// @param options the options to use
			/// @return the length
			static unsigned int computeLength(Solution::csPtr solution, const Options::oPtr options);

			static unsigned int computeLengthAndST(Solution::csPtr solution, const Options::oPtr options,
				vector<unsigned>& ST);

			static unsigned int computeLengthAndRT(Solution::csPtr solution, const Options::oPtr options,
				vector<unsigned>& RT);

			static unsigned int computeLengthSTAndRT(Solution::csPtr solution, const Options::oPtr options,
				vector<unsigned>& ST, vector<unsigned>& RT);

			static unsigned int computeLengthAndReuseIdleTime(Solution::sPtr& solution, const Options::oPtr& options);

			static bool ensureCorrectness(const Solution::csPtr& sol);
		};
	}
}

#endif