#include "SchedulingHelper.h"

namespace msonlab {
	namespace scheduling {
		using std::pair;
		using std::make_pair;

		typedef pair<unsigned, unsigned> puu;

		unsigned int SchedulingHelper::doComputeLengthSTAndRT(std::shared_ptr<const Solution> solution, const Options::oPtr options,
			vector<unsigned>& ST, vector<unsigned>& RT)
		{
			typedef unsigned int uint;
			// const vector references

			unsigned commOverhead = options->getCommOverhead();
			//unsigned puGroupSize = options->getPuGroupSize();
			// const vector references
			auto mapping = solution->getMapping();
			auto scheduling = solution->getScheduling();
			auto tasks = scheduling.size();
			vector<uint> FT(tasks); // finish time of the tasks
			vector<uint> DAT(tasks); // Data Arrival Time
			vector<uint> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]

			uint actId = scheduling[0]->getId();
			ST[actId] = 0;
			FT[actId] = ST[actId] + scheduling[0]->getComputationTime(); // task length, TODO: create a distribution
			DAT[actId] = 0;
			idPuMapping[actId] = mapping[0];
			RT[mapping[0]] = FT[actId];

			// skipping first task
			for (uint i = 1; i < tasks; ++i)
			{
				IProcessable::nPtr actNode = scheduling[i];
				uint actId = actNode->getId();
				uint actPU = mapping[i];
				idPuMapping[actId] = actPU;

				// calculating data arrival time
				size_t predecessorSize = actNode->getPredecessorsSize();
				for (uint j = 0; j < predecessorSize; ++j)
				{
					// skipping edge
					uint id = actNode->getPredecessorNodeId(j);
					uint comm = 0;
					if (actPU != idPuMapping[id]) {
						comm = commOverhead;
					}

					DAT[actId] = std::max(DAT[actId], FT[id] + comm);

					// check whether a flaw is present in this solution
					//if (FT[id] == 0)
					//{
					//	// TODO: remove, when ensured, cannot happen
					//	std::cout << "Flawed solution" << std::endl;
					//	solution->printSolution(std::cout);
					//	std::cin.get();
					//	return UINT32_MAX;
					//}
				}

				ST[actId] = std::max(RT[actPU], DAT[actId]);
				FT[actId] = ST[actId] + actNode->getComputationTime();
				RT[actPU] = FT[actId];
			}

			uint length = *std::max_element(FT.begin(), FT.end());
			return length;
		}

		unsigned int SchedulingHelper::computeLength(Solution::csPtr solution, const Options::oPtr options)
		{
			vector<unsigned> ST(solution->getScheduling().size());
			vector<unsigned> RT(options->getNumberOfPus());

			return doComputeLengthSTAndRT(solution, options, ST, RT);
		}

		unsigned int SchedulingHelper::computeLengthAndST(Solution::csPtr solution, const Options::oPtr options,
			vector<unsigned>& ST)
		{
			ST.resize(solution->getScheduling().size());
			vector<unsigned> RT(options->getNumberOfPus());

			return doComputeLengthSTAndRT(solution, options, ST, RT);
		}

		unsigned int SchedulingHelper::computeLengthAndRT(Solution::csPtr solution, const Options::oPtr options,
			vector<unsigned>& RT)
		{
			vector<unsigned> ST(solution->getScheduling().size());
			RT.resize(options->getNumberOfPus());

			return doComputeLengthSTAndRT(solution, options, ST, RT);
		}

		unsigned int SchedulingHelper::computeLengthSTAndRT(Solution::csPtr solution, Options::oPtr options,
			vector<unsigned>& ST, vector<unsigned>& RT)
		{
			ST.resize(solution->getScheduling().size());
			RT.resize(options->getNumberOfPus());

			return doComputeLengthSTAndRT(solution, options, ST, RT);
		}

		unsigned int SchedulingHelper::computeLengthAndReuseIdleTime(Solution::sPtr& solution, const Options::oPtr& options)
		{
			typedef unsigned int uint;

			unsigned commOverhead = options->getCommOverhead();
			// const vector references
			auto mapping = solution->getMapping();
			auto scheduling = solution->getScheduling();
			auto tasks = scheduling.size();
			vector<uint> ST(tasks);
			vector<uint> FT(tasks); // finish time of the tasks
			vector<uint> RT(options->getNumberOfPus());
			vector<vector<uint>> DAT(options->getNumberOfPus()); // Data Arrival Time
			vector<uint> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]

			uint actId = scheduling[0]->getId();
			ST[actId] = 0;
			FT[actId] = ST[actId] + scheduling[0]->getComputationTime(); // task length, TODO: create a distribution
			idPuMapping[actId] = mapping[0];
			RT[mapping[0]] = FT[actId];

			// first  of pair is the start of the idle time
			// second of pair is the  end  of the idle time
			vector<vector<pair<uint, uint>>> LT;
			for (unsigned i = 0; i < options->getNumberOfPus(); ++i){
				DAT[i].resize(tasks);
				DAT[i][actId] = 0;
				LT.push_back(vector<pair<uint, uint>>());
			}

			// skipping first task
			for (uint i = 1; i < tasks; ++i)
			{
				auto actNode = scheduling[i];
				uint actId = actNode->getId();
				uint actPU = mapping[i];
				idPuMapping[actId] = actPU;

				// calculating data arrival time
				size_t predecessorSize = actNode->getPredecessorsSize();
				for (unsigned p = 0; p < options->getNumberOfPus(); ++p)
				{
					for (uint j = 0; j < predecessorSize; ++j)
					{
						uint id = actNode->getPredecessor(j)->getFromId();
						uint comm = p != idPuMapping[id] ? commOverhead : 0;

						if (FT[id] == 0)
						{
							// this solutuion is not good
							return UINT32_MAX;
						}

						DAT[p][actId] = std::max(DAT[p][actId], FT[id] + comm);
					}
				}

				uint st = std::max(RT[actPU], DAT[actPU][actId]);

				uint min_st = st;
				uint pu = actPU;
				vector<pair<uint, uint>>::iterator min_it;
				for (unsigned p = 0; p < options->getNumberOfPus(); ++p)
				{
					for (auto pit = LT[p].begin(); pit != LT[p].end(); ++pit)
					{
						unsigned start = std::max(DAT[p][actId], pit->first);
						if (start < min_st && start + actNode->getComputationTime() <= pit->second)
						{
							//std::cout << "Reschedule " << actId << " ID to " << pit->first << std::endl;
							//std::cout << "DAT[" << actId << "] is " << DAT[actId] << std::endl;
							min_st = start;
							pu = p;
							min_it = pit;
							//if (start > pit->first) {
							//	LT[actPU].push_back(make_pair(pit->first, start));
							//}

							//if (pit->second > start + actNode->getComputationTime()) {
							//	LT[actPU].push_back(make_pair(start + actNode->getComputationTime(), pit->second));
							//}

							//pit->first = 0;
							//pit->second = 0;
							////pit = LT[actPU].erase(pit);
							//break;
						}
					}
				}

				if (st != min_st)
				{
					st = min_st;
					actPU = pu;
					min_it->first = 0;
					min_it->second = 0;
					idPuMapping[actId] = actPU;
				}

				ST[actId] = st;
				FT[actId] = ST[actId] + actNode->getComputationTime();

				//if (pit != LT[actPU].end())
				//{
				//	// remove used idle time
				//	uint end = pit->second;
				//LT[actPU].erase(pit);
				//	if (FT[actId] < end)
				//	{
				//		// add idle time to the list
				//		LT[actPU].push_back(make_pair(FT[actId], end));
				//	}
				//}


				if (ST[actId] > 0 && RT[actPU] + 1  < ST[actId])
				{
					// add idle time to the list
					//std::cout << "Sceduling " << actId << ", ";
					//std::cout << "Idle time " << RT[actPU] + 1 << " -> " << ST[actId] << std::endl;
					LT[actPU].push_back(make_pair(RT[actPU] + 1, ST[actId]));
				}

				RT[actPU] = std::max(RT[actPU], FT[actId]);
			}

			uint length = *std::max_element(FT.begin(), FT.end());

			// how to create the new solution?

			Node::nVect nodes(tasks);
			vector<pair<unsigned, unsigned>> STS;
			for (size_t i = 0; i < tasks; ++i)
			{
				puu p = make_pair(ST[i], i);
				STS.push_back(p);
				nodes[scheduling[i]->getId()] = scheduling[i];
			}

			std::sort(STS.begin(), STS.end());

			for (size_t i = 0; i < tasks; ++i)
			{
				solution->scheduling[i] = nodes[STS[i].second];
				solution->mapping[i] = idPuMapping[STS[i].second];
			}

			//Solution::cVect nSched;

			//// right know just use the idle time on the same PU
			//for (auto it = scheduling.begin(); it != scheduling.end(); ++it)
			//{
			//	uint id = (*it)->getId();
			//	uint pu = idPuMapping[id];
			//	for (auto pit = LT[pu].begin(); pit != LT[pu].end(); ++pit)
			//	{
			//		if (pit->second - pit->first >= (*it)->getComputationTime() &&
			//			DAT[id] >= pit->first)
			//		{
			//			std::cout << "Reschedlue " << id << " ID to " << pit->first << std::endl;
			//		}
			//		
			//	}
			//}

			return length;
		}
	}
}