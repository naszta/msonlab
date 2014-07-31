#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeConstant : public Node
	{
	public:
		NodeConstant(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		NodeConstant(const NodeConstant& other);

		NodeConstant& operator=(const NodeConstant& other);
		Node::nPtr clone();

		virtual IProcessable::pVect process();
		
		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string get_custom_data() const;
		std::string get_shape() const;
		std::string get_color() const;
	};
}