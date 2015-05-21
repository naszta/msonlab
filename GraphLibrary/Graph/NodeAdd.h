#ifndef NODE_ADD 
#define NODE_ADD
#include "Node.h"

namespace msonlab
{
	class NodeAdd : public Node
	{
	public:
		NodeAdd(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeAdd(const NodeAdd& other);

		NodeAdd& operator=(const NodeAdd& other);
		NodePtr clone() const override;

		virtual IProcessableVect process() override;
		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule) override;
	};
}
#endif