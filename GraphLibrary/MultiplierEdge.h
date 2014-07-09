#pragma once
#include "Edge.h"

namespace msonlab
{
	class MultiplierEdge : public Edge
	{
	private:
		Types::DataType multiplier;
	public:
		MultiplierEdge(unsigned int _id, Types::LabelType _label, Types::DataPtr _value, IProcessable::nPtr _from, IProcessable::nPtr _to, Types::DataType _multiplier);

		virtual IProcessable::pVect process();

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};
}