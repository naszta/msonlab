#pragma once
#include "Edge.h"

namespace msonlab
{
	class MultiplierEdge : public Edge
	{
	private:
		double multiplier;
	public:
		MultiplierEdge(unsigned int _id, wchar_t _label, Types::DataType _value, IProcessable::nPtr _from, IProcessable::nPtr _to, double _multiplier);

		virtual IProcessable::pVect process();

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};
}