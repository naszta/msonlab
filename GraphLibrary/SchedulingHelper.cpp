//#include "SchedulingHelper.h"
#include <vector>

#include "Options.h"
#include "Solution.h"

namespace msonlab {
	namespace scheduling {
			using std::pair;
			using std::make_pair;

			typedef pair<unsigned, unsigned> puu;

			template<typename SolutionType>
			unsigned doComputeLengthSTAndRT(const SolutionType &solution, const Options &options,
				vector<unsigned>& ST, vector<unsigned>& RT)
			{
				typedef unsigned uint;
				// const vector references

				unsigned commOverhead = options.getCommOverhead();
				//unsigned puGroupSize = options.getPuGroupSize();
				// const vector references
				const auto& mapping = solution.mapping();
				const auto& scheduling = solution.scheduling();
				auto tasks = scheduling.size();
				vector<uint> FT(tasks); // finish time of the tasks
				vector<uint> DAT(tasks); // Data Arrival Time
				vector<uint> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]

				uint actId = scheduling[0]->id();
				ST[actId] = 0;
				FT[actId] = ST[actId] + scheduling[0]->cptime(); // task length, TODO: create a distribution
				DAT[actId] = 0;
				idPuMapping[actId] = mapping[0];
				RT[mapping[0]] = FT[actId];

				// skipping first task
				for (uint i = 1; i < tasks; ++i)
				{
					auto actNode = scheduling[i];
					uint actId = actNode->id();
					uint actPU = mapping[i];
					idPuMapping[actId] = actPU;

					// calculating data arrival time
					const auto& preds = actNode->predecessors();
					for (auto n : preds)
					{
						// skipping edge
						uint id = n->id();

						// check whether a flaw is present in this solution
						// if a predecessor is not scheduled yet, that is a flaw
						if (FT[id] == 0) {
							//std::cout << "Solution is checked before. This CANNOT happen.\n";
							return UINT32_MAX;
						}

						uint comm = 0;
						if (actPU != idPuMapping[id]) {
							comm = commOverhead;
						}

						DAT[actId] = std::max(DAT[actId], FT[id] + comm);
					}

					ST[actId] = std::max(RT[actPU], DAT[actId]);
					FT[actId] = ST[actId] + actNode->cptime();
					RT[actPU] = FT[actId];
				}

				uint length = *std::max_element(FT.begin(), FT.end());
				return length;
			}

			template<typename SolutionType>
			unsigned int computeLength(const SolutionType &solution, const Options &options)
			{
				typename SolutionType;
				vector<unsigned> ST(solution.size());
				vector<unsigned> RT(options.getNumberOfPus());

				return doComputeLengthSTAndRT<SolutionType>(solution, options, ST, RT);
			}

			// compute length with returning start time
			template<typename SolutionType>
			unsigned int computeLengthAndST(const SolutionType &solution, const Options &options,
				vector<unsigned>& ST)
			{
				typename SolutionType;
				ST.resize(solution.size());
				vector<unsigned> RT(options.getNumberOfPus());

				return doComputeLengthSTAndRT<SolutionType>(solution, options, ST, RT);
			}

			template<typename SolutionType>
			unsigned int computeLengthAndRT(const SolutionType &solution, const Options &options,
				vector<unsigned>& RT)
			{
				typename SolutionType;
				vector<unsigned> ST(solution.size());
				RT.resize(options.getNumberOfPus());

				return doComputeLengthSTAndRT<SolutionType>(solution, options, ST, RT);
			}

			// compute length with returning start time and ready time
			template<typename SolutionType>
			unsigned int computeLengthSTAndRT(const SolutionType &solution, const Options &options,
				vector<unsigned>& ST, vector<unsigned>& RT)
			{
				ST.resize(solution.size());
				RT.resize(options.getNumberOfPus());

				return doComputeLengthSTAndRT(solution, options, ST, RT);
			}

			template<typename SolutionType>
			unsigned int computeLengthAndReuseIdleTime(SolutionType &solution, const Options& options)
			{
				typedef unsigned int uint;

				unsigned commOverhead = options.getCommOverhead();
				// const vector references
				const vector<unsigned>& mapping = solution.mapping();
				const vector<const lite::litenode*>& scheduling = solution.scheduling();
				auto tasks = scheduling.size();
				vector<uint> ST(tasks); // start time of the tasks
				vector<uint> FT(tasks); // finish time of the tasks
				vector<uint> RT(options.getNumberOfPus()); // ready time of the pus
				vector<vector<uint>> DAT(options.getNumberOfPus()); // Data Arrival Time
				vector<uint> idPuMapping(tasks); // idPuMapping[i] = j means that node[i] is processed by pu[j]

				// start the with the first node in the list
				uint actId = scheduling[0]->id();
				ST[actId] = 0; // start time is 0
				FT[actId] = ST[actId] + scheduling[0]->cptime(); // task length, TODO: create a distribution
				idPuMapping[actId] = mapping[0];
				RT[mapping[0]] = FT[actId];

				// first  of pair is the start of the idle time
				// second of pair is the  end  of the idle time
				// the first vector stores the idle slots for each pu
				// the second vector stores the idle slots in one pu
				vector<vector<pair<uint, uint>>> slots;
				for (unsigned i = 0; i < options.getNumberOfPus(); ++i){
					DAT[i].resize(tasks);
					DAT[i][actId] = 0;
					// initializing with empty vectors
					slots.push_back(vector<pair<uint, uint>>());
				}

				// skipping first task, already computed
				for (uint i = 1; i < tasks; ++i)
				{
					auto actNode = scheduling[i];
					uint actId = actNode->id();
					uint actPU = mapping[i];
					idPuMapping[actId] = actPU;

					// calculating data arrival time
					const vector<const lite::litenode*>& preds = actNode->predecessors();
					for (auto n : preds)
					{
						uint id = n->id();
						if (FT[id] == 0) {
							// this solutuion is not good
							return UINT32_MAX;
						}

						// check dat for all pus, for possible reschedule
						for (unsigned p = 0; p < options.getNumberOfPus(); ++p)
						{
							uint comm = p != idPuMapping[id] ? commOverhead : 0;
							DAT[p][actId] = std::max(DAT[p][actId], FT[id] + comm);
						}
					}

					// start time on the scheduled pu.
					uint st = std::max(RT[actPU], DAT[actPU][actId]);

					uint min_st = st;
					uint pu = actPU;
					vector<pair<uint, uint>>::iterator min_it;
					// iterating over every idle time slots
					for (unsigned p = 0; p < options.getNumberOfPus(); ++p)
					{
						// the start time at pu p
						//unsigned stp = std::max(RT[p], DAT[p][actId]);
						for (auto pit = slots[p].begin(); pit != slots[p].end(); ++pit)
						{
							// possible first start in this idle time
							unsigned start = std::max(DAT[p][actId], pit->first);
							// it's better if
							// 1) the start is earlier than the calculated
							// 2) the finish is earlier than the end of the idle time
							if (start < min_st && start + actNode->cptime() <= pit->second)
							{
								min_st = start;
								pu = p;
								min_it = pit;
							}
						}
					}

					// if better found, reschedule
					if (st != min_st)
					{
						st = min_st;
						actPU = pu;

						pair<unsigned, unsigned> it = *min_it;
						slots[pu].erase(min_it);

						if (st - it.first > 1) {
							slots[pu].push_back(make_pair(it.first, st - 1));
						}

						if (it.second - (st + actNode->cptime()) > 1) {
							slots[pu].push_back(make_pair(st + actNode->cptime() + 1, it.second));
						}

						idPuMapping[actId] = actPU;
					}

					ST[actId] = st;
					FT[actId] = ST[actId] + actNode->cptime();

					// add idle time slot if there is one
					if (ST[actId] > 0 && RT[actPU] < ST[actId])
					{
						slots[actPU].push_back(make_pair(RT[actPU], ST[actId]));
					}

					// in case an idle time is used, the finish time is smaller
					// than the ready time of the pu, because there are already other
					// task scheduled after the one scheduled in this round
					RT[actPU] = std::max(RT[actPU], FT[actId]);
				}

				uint length = *std::max_element(FT.begin(), FT.end());

				// collecting the nodes
				vector<const lite::litenode*> nodes(tasks);
				vector<pair<unsigned, unsigned>> STS;
				for (size_t i = 0; i < tasks; ++i)
				{
					// first is the start time of the task
					// second is the id of the task
					STS.push_back(make_pair(ST[i], i));
					nodes[scheduling[i]->id()] = scheduling[i];
				}

				// sort taks by start time
				std::sort(STS.begin(), STS.end());

				// updating the scheduling and mapping
				for (size_t i = 0; i < tasks; ++i)
				{
					solution._scheduling[i] = nodes[STS[i].second];
					solution._mapping[i] = idPuMapping[STS[i].second];
				}

				return length;
			}

			// a helper method for development
			// fast checks, whether is solution is correct or not
			template<typename SolutionType>
			bool is_correct(const SolutionType &solution)
			{
				// ensure there is no duplication in the scheduling
				const auto& scheduling = solution.scheduling();
				vector<bool> scheduled(scheduling.size(), false);
				for (const auto& node : scheduling)
				{
					// ensure there is no duplication
					if (scheduled[node->id()]) {
						std::cout << node->id() << "scheduled twice\n";
						return false;
					}

					const auto& preds = node->predecessors();
					for (auto pred_node : preds)
					{
						if (!scheduled[pred_node->id()])
						{
							std::cout << pred_node->id() << " is not scheduled before " << node->id() << std::endl;
							return false;
						}
					}

					scheduled[node->id()] = true;
				}

				return true;
			}
	}
}