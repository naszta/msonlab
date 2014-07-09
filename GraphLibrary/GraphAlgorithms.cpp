#include "GraphAlgorithms.h"
#include "BFSIterator.h"
#include <map>
#include <queue>
#include <set>
#include <iostream>
#include <utility>
#include <algorithm>

namespace msonlab
{
	using std::pair;
	using std::make_pair;

	typedef pair<unsigned, unsigned> puu;

	unsigned int GraphAlgorithms::doComputeLengthSTAndRT(std::shared_ptr<const Chromosome> chromosome, const Options::oPtr options,
		vector<unsigned>& ST, vector<unsigned>& RT)
	{
		typedef unsigned int uint;
		unsigned communication = 0;
		// const vector references

		unsigned commOverhead = options->getCommOverhead();
		unsigned puGroupSize = options->getPuGroupSize();
		// const vector references
		auto mapping = chromosome->getMapping();
		auto scheduling = chromosome->getScheduling();
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
				uint id = actNode->getPredecessor(j)->getFromId();
				uint comm = 0;
				if (actPU != idPuMapping[id]) {
					comm = commOverhead;
					++communication;
				}

				if (actPU / puGroupSize != idPuMapping[id] / puGroupSize)
				{
					comm *= 2;
				}
				if (DAT[actId] < FT[id] + comm)
				{
					DAT[actId] = FT[id] + comm;
				}

				// check whether a flaw is present in this solution
				if (FT[id] == 0)
				{
					// TODO: remove, when ensured, cannot happen
					std::cout << "Flawed chromosome" << std::endl;
					chromosome->printChromosome(std::cout);
					std::cin.get();
					return UINT32_MAX;
				}
			}

			ST[actId] = std::max(RT[actPU], DAT[actId]);
			FT[actId] = ST[actId] + actNode->getComputationTime();
			RT[actPU] = FT[actId];
		}

		uint length = *std::max_element(FT.begin(), FT.end());

		return length;
	}

	unsigned int GraphAlgorithms::computeLength(Chromosome::ccPtr chromosome, const Options::oPtr options)
	{
		vector<unsigned> ST(chromosome->getScheduling().size());
		vector<unsigned> RT(options->getNumberOfPus());

		return doComputeLengthSTAndRT(chromosome, options, ST, RT);
	}

	unsigned int GraphAlgorithms::computeLengthAndST(Chromosome::ccPtr chromosome, const Options::oPtr options,
		vector<unsigned>& ST)
	{
		ST.resize(chromosome->getScheduling().size());
		vector<unsigned> RT(options->getNumberOfPus());

		return doComputeLengthSTAndRT(chromosome, options, ST, RT);
	}

	unsigned int GraphAlgorithms::computeLengthAndRT(Chromosome::ccPtr chromosome, const Options::oPtr options,
		vector<unsigned>& RT)
	{
		vector<unsigned> ST(chromosome->getScheduling().size());
		RT.resize(options->getNumberOfPus());

		return doComputeLengthSTAndRT(chromosome, options, ST, RT);
	}

	unsigned int GraphAlgorithms::computeLengthSTAndRT(Chromosome::ccPtr chromosome, Options::oPtr options,
		vector<unsigned>& ST, vector<unsigned>& RT)
	{
		ST.resize(chromosome->getScheduling().size());
		RT.resize(options->getNumberOfPus());

		return doComputeLengthSTAndRT(chromosome, options, ST, RT);
	}

	// creates partial topological order starting with the outputs
	vector<IProcessable::nVect> GraphAlgorithms::partialTopologicalSort(const Graph::gPtr graph) const
	{
		IProcessable::nVect outputNodes = graph->getOutputNodes();
		vector<IProcessable::nVect> result;
		result.push_back(outputNodes);
		std::map< IProcessable::nPtr, int> count;

		size_t added = outputNodes.size();
		IProcessable::nVect::iterator it;
		for (int level = 0;; ++level) {
			result.push_back(IProcessable::nVect());
			for (it = result[level].begin(); it != result[level].end(); ++it)
			{
				IProcessable::nPtr act = *it;
				IProcessable::eVect predecessors = act->getPredecessors();
				for (size_t i = 0; i < predecessors.size(); ++i)
				{
					count[predecessors[i]->getFrom()]++;
					if (predecessors[i]->getFrom()->getSuccessors().size() == count[predecessors[i]->getFrom()])
					{
						result[level + 1].push_back(predecessors[i]->getFrom());
						++added;
					}
				}
			}

			if (added == graph->nodes.size())
			{
				break;
			}
		}

		return result;
	}

	// creates partial topological order starting with the inputs
	vector<IProcessable::nVect> GraphAlgorithms::partialTopologicalSortFromBottom(const Graph::gPtr graph) const
	{
		IProcessable::nVect inputNodes = graph->getInputNodes();
		vector<IProcessable::nVect> result;
		// first level, the input nodes
		result.push_back(inputNodes);
		std::map< IProcessable::nPtr, int> count;

		// this number of nodes added to the leveling
		size_t added = inputNodes.size();
		IProcessable::nVect::iterator it;
		for (int level = 0;; ++level) {
			// adding a new level to the result
			result.push_back(IProcessable::nVect());
			for (it = result[level].begin(); it != result[level].end(); ++it)
			{
				IProcessable::nPtr act = *it;
				IProcessable::eVect successors = act->getSuccessors();
				for (size_t i = 0; i < successors.size(); ++i)
				{
					count[successors[i]->getTo()]++;
					if (successors[i]->getTo()->getPredecessorsSize() == count[successors[i]->getTo()])
					{
						result[level + 1].push_back(successors[i]->getTo());
						++added;
					}
				}
			}

			if (added == graph->nodes.size())
			{
				break;
			}
		}

		return result;
	}

	// creates a topological sort from the graph
	IProcessable::nVect GraphAlgorithms::topologicalSort(const Graph::gPtr graph) const
	{
		IProcessable::nVect order;
		IProcessable::nVect allNodes = graph->nodes;
		IProcessable::nVect inputNodes = graph->getInputNodes();
		IProcessable::nVect::iterator it;
		std::map< IProcessable::nPtr, int> count;
		std::queue< IProcessable::nPtr> q;

		// pushing input nodes into a queue
		// nodes in this queue have no unprocessed predecessors
		for (it = inputNodes.begin(); it != inputNodes.end(); ++it)
		{
			q.push(*it);
		}

		while (!q.empty())
		{
			IProcessable::nPtr act = q.front();
			q.pop();
			order.push_back(act);
			IProcessable::eVect successors = act->getSuccessors();
			for (unsigned i = 0; i < successors.size(); ++i)
			{
				count[successors[i]->getTo()]++;
				if (successors[i]->getTo()->getPredecessors().size() == count[successors[i]->getTo()])
				{
					q.push(successors[i]->getTo());
				}
			}
		}

		return order;
	}

	Graph::gPtr GraphAlgorithms::computeChangedGraph(const Graph::gPtr graph, IProcessable::nSet changed, IProcessable::nSet needed) const
	{
		IProcessable::nVect topsort = topologicalSort(graph);
		IProcessable::nSet::iterator it;
		std::map<IProcessable::nPtr, int> topologicalOrderMap;

		// finding the the last needed element in the topological order
		int maxNodeTopValue = -1;
		for (int i = topsort.size() - 1; i >= 0; --i)
		{
			topologicalOrderMap[topsort[i]] = i;
			if (needed.count(topsort[i]) > 0 && i > maxNodeTopValue)
			{
				maxNodeTopValue = i;
			}
		}

		// starting bfs from every changed node
		// stops if topological number is bigger than
		// the maximum of needed
		IProcessable::nSet modified;
		BFSIterator bfsItr(graph);
		for (it = changed.cbegin(); it != changed.cend(); ++it)
		{
			bfsItr.setStartNode(*it);
			for (; bfsItr.hasMoreNode(); ++bfsItr)
			{
				if (modified.count(*bfsItr) > 0 || topologicalOrderMap[*bfsItr] > maxNodeTopValue)
				{
					bfsItr.skipActNode();
				}
				else
				{
					modified.insert(*bfsItr);
				}
			}
		}

		// collecting modified nodes
		Graph::gPtr changedGraph(new Graph());

		std::queue<IProcessable::nPtr> toVisit;
		IProcessable::nSet visited;
		IProcessable::nPtr node;
		IProcessable::eVect::iterator eIt;
		for (it = needed.begin(); it != needed.end(); ++it)
		{
			toVisit.push(*it);
		}

		while (!toVisit.empty())
		{
			do
			{
				if (toVisit.empty())
				{
					break;
				}
				else
				{
					node = toVisit.front();
					toVisit.pop();
				}
			} while (visited.count(node) > 0);

			IProcessable::eVect neighbours = node->getPredecessors();
			for (eIt = neighbours.begin(); eIt != neighbours.end(); ++eIt)
			{
				if (changed.count((*eIt)->getTo()) == 0)
				{
					changedGraph->addEdge(*eIt);
				}

				if (modified.count((*eIt)->getFrom()) > 0)
				{
					toVisit.push((*eIt)->getFrom());
				}

			}
		}

		return changedGraph;
	}

	void GraphAlgorithms::computeNextFreeNodes(vector<int>& dependencies, Node::nPtr node) const
	{
		for (auto it = node->getSuccessors().begin(); it != node->getSuccessors().end(); ++it)
		{
			unsigned id = (*it)->getToId();
			--dependencies[id];
		}

		dependencies[node->getId()] = -1;
	}

	unsigned GraphAlgorithms::findMaxDistanceWithoutDependency(const vector<int>& dependencies, const vector<unsigned>& distances) const
	{
		unsigned id = 0;
		unsigned max = 0;
		for (unsigned i = 0; i < distances.size(); ++i) {
			if (dependencies[i] == 0 && (max == 0 || distances[i] > max)) {
				id = i;
				max = distances[i];
			}
		}

		return id;
	}

	void GraphAlgorithms::createDependencyVector(const Graph::gPtr graph, vector<int>& dependencies) const
	{
		if (dependencies.size() != graph->numberOfNodes()) {
			dependencies.resize(graph->numberOfNodes());
		}

		auto end = graph->nodes.end();
		for (auto it = graph->nodes.begin(); it != end; ++it) {
			dependencies[(*it)->getId()] = (*it)->getPredecessorsSize();
		}
	}

	void GraphAlgorithms::listNodes(const Graph::gPtr graph, vector<Node::nPtr>& nodes) {
		if (nodes.size() != graph->numberOfNodes()) {
			nodes.resize(graph->numberOfNodes());
		}

		auto end = graph->nodes.end();
		for (auto it = graph->nodes.begin(); it != end; ++it) {
			nodes[(*it)->getId()] = (*it);
		}
	}

	int GraphAlgorithms::scheduleGreedy(const Graph::gPtr graph, int pus) const
	{
		int timeCounter = 0;
		unsigned taskCounter = 0;
		std::map< IProcessable::nPtr, int> count;
		IProcessable::nVect inputNodes = graph->getInputNodes();
		std::queue < IProcessable::nPtr> free;
		for (size_t i = 0; i < inputNodes.size(); ++i)
		{
			free.push(inputNodes[i]);
		}

		while (taskCounter < graph->numberOfNodes())
		{
			vector< IProcessable::nPtr > out;
			int limit = pus;
			while (!free.empty() && limit > 0)
			{
				out.push_back(free.front());
				free.pop();
				--limit;
				taskCounter++;
			}
			++timeCounter;
			if (taskCounter == graph->numberOfNodes())
			{
				break;
			}

			for (size_t i = 0; i < out.size(); ++i)
			{
				IProcessable::eVect successors = out[i]->getSuccessors();
				for (size_t j = 0; j < successors.size(); ++j)
				{
					count[successors[j]->getTo()]++;
					if (successors[j]->getTo()->getPredecessorsSize() == count[successors[j]->getTo()])
					{
						free.push(successors[j]->getTo());
					}
				}
			}
		}

		return timeCounter;
	}

	unsigned int GraphAlgorithms::computeLengthAndReuseIdleTime(Chromosome::cPtr chromosome, const Options::oPtr options)
	{
		typedef unsigned int uint;
		unsigned communication = 0;
		// const vector references

		unsigned commOverhead = options->getCommOverhead();
		unsigned puGroupSize = options->getPuGroupSize();
		// const vector references
		auto mapping = chromosome->getMapping();
		auto scheduling = chromosome->getScheduling();
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
			IProcessable::nPtr actNode = scheduling[i];
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
				//std::cout << "Reschedule " << actId << " ID to " << min_st << ", on " << pu << std::endl;
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

		// how to create the new chromosome?

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
			chromosome->scheduling[i] = nodes[STS[i].second];
			chromosome->mapping[i] = idPuMapping[STS[i].second];
		}

		//Chromosome::cVect nSched;

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