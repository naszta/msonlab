/**
  *
  * Date: 2014.07.17 13:21
  * Author: Zoltan
  */
#ifndef GRAPHLIB_SCHEDULINGUTILS_H
#define GRAPHLIB_SCHEDULINGUTILS_H

#include <vector>

#include "../Options.h"

namespace msonlab { namespace scheduling {
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
	SolutionType computeLengthAndReuseIdleTime(SolutionType& solution, const Options& options);

	template<typename SolutionType>
	bool is_correct(const SolutionType &sol);

	template<typename SolutionType>
	void write_solution(const SolutionType &solution, const Options& options, std::ostream& os);

	template<typename SolutionType>
	void write_mapping_scheduling(const SolutionType&, std::ostream&);
}}

#include "SchedulingUtils.cpp"

#endif