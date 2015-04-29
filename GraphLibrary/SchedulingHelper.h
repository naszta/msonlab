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

		///
		/// Computes the length of the solution with the given option.
		///
		/// @param solution the result to use
		/// @param options the options to use
		/// @return the length
		template<typename SolutionType>
		unsigned computeLength(const SolutionType &solution, const Options &options);
		
		template<typename SolutionType>
		unsigned computeLengthAndST(const SolutionType &solution, const Options &options,
			vector<unsigned>& ST);

		template<typename SolutionType>
		unsigned computeLengthAndRT(const SolutionType &solution, const Options &options,
			vector<unsigned>& RT);

		template<typename SolutionType>
		unsigned computeLengthSTAndRT(const SolutionType &solution, const Options &options,
			vector<unsigned>& ST, vector<unsigned>& RT);

		template<typename SolutionType>
		unsigned computeLengthAndReuseIdleTime(SolutionType& solution, const Options& options);

		template<typename SolutionType>
		bool is_correct(const SolutionType &sol);

		template<typename SolutionType>
		void write_solution(const SolutionType &solution, const Options& options, std::ostream& os);
	}
}

#include "SchedulingHelper.cpp"

#endif