#pragma once
#include "NodeMultiply.h"
#include "Edge.h"

namespace msonlab
{
	NodeMultiply::NodeMultiply(unsigned int _id, wchar_t _label, Types::DataType _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeMultiply::process()
	{
		IProcessable::pVect ret;

		if (isReadyForProcess())
		{
			Types::DataType newVal = msonlab::Types::DataType(new double(1.0));

			for (IProcessable::eVect::iterator it = predecessors.begin(); it != predecessors.end(); ++it)
			{
				*newVal *= *(*(*it)).getResultValue();
			}

			if (setProcessed(newVal))
			{
				for (IProcessable::eVect::iterator it = successors.begin(); it != successors.end(); ++it)
				{
					if((*(*it)).registerParameter())
					{
						ret.insert(ret.begin(),(*it));
					}
				}
				return ret;
			}
			else
				throw msonlab::Exceptions::GeneralErrorException("Error while setting the result of processing on this processable element!");
		}
		else
		{
			throw msonlab::Exceptions::StillNotReadyForProcessException("This processable element is not yet ready for processing!");
		}

		return ret;
	}

	// compile
	void NodeMultiply::compile(msonlab::StackRunner::srPtr stackProgram)
	{
		for (msonlab::Edge::eVect::iterator it = predecessors.begin(); it != predecessors.end(); ++it)
		{
			(*(*it)).compile(stackProgram);
		}
		int n = predecessors.size();
		for (int i = 0; i < n-1; ++i)
		{
			stackProgram->addToken(msonlab::StackRunner::MUL, nullptr);
		}
	}
}