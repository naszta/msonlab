#pragma once
#include "NodeConstant.h"
#include "Edge.h"

namespace msonlab
{
	NodeConstant::NodeConstant(unsigned int _id, wchar_t _label, Types::DataType _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodeConstant::process()
	{
		IProcessable::pVect ret;

		if (isReadyForProcess())
		{
			if (setProcessed(value))
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
	}

	// compile

	void NodeConstant::compile(msonlab::StackRunner::srPtr stackProgram)
	{
		stackProgram->addToken(msonlab::StackRunner::PUSH, getValue());
	}

}