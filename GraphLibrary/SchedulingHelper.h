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
			//namespace helper {
			using std::vector;

			//vector<unsigned>& ST, vector<unsigned>& RT);
			///
			/// Computes the length of the solution with the given option.
			///
			/// @param solution the result to use
			/// @param options the options to use
			/// @return the length
			unsigned computeLength(const Solution &solution, const OptionsPtr options);

			unsigned computeLengthAndST(const Solution &solution, const OptionsPtr options,
				vector<unsigned>& ST);

			unsigned computeLengthAndRT(const Solution &solution, const OptionsPtr options,
				vector<unsigned>& RT);

			unsigned computeLengthSTAndRT(const Solution &solution, const OptionsPtr options,
				vector<unsigned>& ST, vector<unsigned>& RT);

			unsigned computeLengthAndReuseIdleTime(Solution& solution, const Options& options);

			bool is_correct(const Solution &sol);
		//}
	}
}

#endif