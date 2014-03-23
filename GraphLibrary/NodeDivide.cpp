#pragma once
#include "NodeDivide.h"
#include "Edge.h"

namespace msonlab
{
	NodeDivide::NodeDivide(unsigned int _id, Types::LabelType _label, Types::DataType _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeDivide::process()
	{
		IProcessable::pVect ret;

		if (isReadyForProcess())
		{
			Types::DataType newVal =  msonlab::Types::DataType(new double(1.0));

			int i = 1;

			for (IProcessable::eVect::iterator it = predecessors.begin(); it != predecessors.end(); ++it)
			{
				if (i == 1)
					*newVal = *(*(*it)).getResultValue();
				else
					*newVal /= *(*(*it)).getResultValue();

				i++;
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

	void NodeDivide::compile(msonlab::StackRunner::srPtr stackProgram)
	{
		int i = 1;
		for (msonlab::Edge::eVect::iterator it = predecessors.begin(); it != predecessors.end(); ++it)
		{
			if (i < 3)
				(*(*it)).compile(stackProgram);
			++i;
		}
		stackProgram->addToken(msonlab::StackRunner::DIV, nullptr);
	}

}