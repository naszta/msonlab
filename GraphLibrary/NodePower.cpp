#pragma once
#include "NodePower.h"
#include "Edge.h"
#include <math.h>

namespace msonlab
{
	NodePower::NodePower(unsigned int _id, Types::LabelType _label, Types::DataPtr _value)
		: Node(_id, _label, _value)
	{
	}

	IProcessable::pVect NodePower::process()
	{
		IProcessable::pVect ret;

		if (isReadyForProcess())
		{
			Types::DataPtr newVal = std::make_shared<Types::DataType>(1.0);
			Types::DataPtr floor = std::make_shared<Types::DataType>(1.0);

			int i = 1;

			for (IProcessable::eVect::iterator it = predecessors.begin(); it != predecessors.end(); ++it)
			{
				if (i == 1)
					*newVal = *(*(*it)).getResultValue();
				else if (i == 2)
					*floor = *(*(*it)).getResultValue();
				else
				{ }
				i++;
			}

			

			if (setProcessed(std::make_shared<Types::DataType>(pow(*newVal, *floor))))
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
	void NodePower::compile(msonlab::StackRunner::srPtr stackProgram)
	{
		int i = 1;
		for (msonlab::Edge::eVect::iterator it = predecessors.begin(); it != predecessors.end(); ++it)
		{
			if (i < 3)
				(*(*it)).compile(stackProgram);
			++i;
		}
		stackProgram->addToken(msonlab::StackRunner::POW, nullptr);
	}
}