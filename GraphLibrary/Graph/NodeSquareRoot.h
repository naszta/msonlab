#ifndef NODE_SQUARE_ROOT 
#define NODE_SQUARE_ROOT
#include "Node.h"

namespace msonlab
{
	class NodeSquareRoot : public Node
	{
	public:
		NodeSquareRoot(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeSquareRoot(const NodeSquareRoot& other);

		NodeSquareRoot& operator=(const NodeSquareRoot& other);
		NodePtr clone() const override;
		virtual IProcessableVect process() override;

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule) override;
	};
}
#endif