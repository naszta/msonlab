#ifndef NODE_DIVIDE 
#define NODE_DIVIDE
#include "Node.h"

namespace msonlab
{
	class NodeDivide : public Node
	{
	public:
		NodeDivide(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeDivide(const NodeDivide& other);

		NodeDivide& operator=(const NodeDivide& other);
		NodePtr clone() const override;
		virtual IProcessableVect process() override;

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule) override;
	};
}
#endif