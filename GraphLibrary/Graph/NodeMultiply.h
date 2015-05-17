#ifndef NODE_MULTIPLY 
#define NODE_MULTIPLY
#include "Node.h"

namespace msonlab
{
	class NodeMultiply : public Node
	{
	public:
		NodeMultiply(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeMultiply(const NodeMultiply& other);

		NodeMultiply& operator=(const NodeMultiply& other);
		NodePtr clone();
		virtual IProcessableVect process();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string get_color() const;
	};
}
#endif