#ifndef BLUE_EDGE 
#define BLUE_EDGE
#include "../Global.h"
#include "Edge.h"

namespace msonlab
{
	class BlueEdge : public Edge
	{
	public:
		BlueEdge(unsigned int _id, types::LabelType _label, types::DataPtr _value, NodePtr _from, NodePtr _to);

		virtual EdgeTypeEnum getEdgeType() const;

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string getTypeString() const;
		std::string get_color() const;
	};

}
#endif