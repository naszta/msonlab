#ifndef NODE_CONSTANT 
#define NODE_CONSTANT
#include "Node.h"

namespace msonlab
{
	class NodeConstant : public Node
	{
	public:
		NodeConstant(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeConstant(const NodeConstant& other);

		NodeConstant& operator=(const NodeConstant& other);
		NodePtr clone() const override;

		virtual IProcessableVect process() override;
		
		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule) override;

		// exchange
		std::string get_custom_data() const;
	};
}
#endif