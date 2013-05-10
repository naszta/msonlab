#pragma once
#include "MultiplierEdge.h"

namespace msonlab
{
	MultiplierEdge::MultiplierEdge(unsigned int _id, wchar_t _label, Types::DataType _value, Node::nPtr _from, Node::nPtr _to, double _multiplier)
		: Edge(_id, _label, _value, _from, _to)
	{
		multiplier = _multiplier;
	}

	IProcessable::pVect MultiplierEdge::process()
	{
		if (isReadyForProcess())
		{
			// TODO: static class for operands
			if (setProcessed((*from).getResultValue()))
			{
				IProcessable::pVect retVal;
				
				if((*to).registerParameter())
					retVal.insert(retVal.begin(),to);

				return retVal;
			}
			else
				throw msonlab::Exceptions::GeneralErrorException("Error while setting the result of processing on this processable element!");
		}
		else
		{
			throw msonlab::Exceptions::StillNotReadyForProcessException("This processable element is not yet ready for processing!");
		}
	}

}