#pragma once
#include "MultiplierEdge.h"
#include "Node.h"

namespace msonlab
{
	MultiplierEdge::MultiplierEdge(unsigned int _id, Types::LabelType _label, Types::DataPtr _value, IProcessable::nPtr _from, IProcessable::nPtr _to, Types::DataType _multiplier)
		: Edge(_id, _label, _value, _from, _to)
	{
		multiplier = _multiplier;
	}

	IProcessable::pVect MultiplierEdge::process()
	{
		IProcessable::pVect ret;

		if (isReadyForProcess())
		{
			// TODO: static class for operands
			if (setProcessed((*from).getResultValue()))
			{
				if ((*to).registerParameter())
				{
					ret.insert(ret.begin(),(to));
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

	void MultiplierEdge::compile(msonlab::StackRunner::srPtr stackProgram)
	{
		stackProgram->addToken(msonlab::StackRunner::PUSH, std::make_shared<Types::DataType>(multiplier));
		stackProgram->addToken(msonlab::StackRunner::MUL, nullptr);
		(*from).compile(stackProgram);
	}

}