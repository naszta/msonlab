#pragma once
#include "Node.h"

namespace msonlab
{
	class NodeSquareRoot : public Node
	{
	public:
		NodeSquareRoot(unsigned int _id, std::string _label, Types::DataType _value);

		virtual IProcessable::pVect process();

		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		// exchange
		std::string getTypeString() const;
		std::string get_color() const;
	};



}