#pragma once
#include "Edge.h"
#include "Node.h"

namespace msonlab
{
	Edge::Edge(unsigned int _id, wchar_t _label, Types::DataType _value, IProcessable::nPtr _from, IProcessable::nPtr _to)
		: IProcessable(_id, _label, _value), from(_from), to(_to), paramReady(false)
	{
	}

	Edge::Edge(const Edge& other)
	{
		throw Exceptions::NotImplementedException("Edge copy constructor");
	}

	bool Edge::registerParameter()
	{
		paramReady = true;

		return true;
	}

	IProcessable::pVect Edge::process()
	{
		if (isReadyForProcess())
		{
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

	bool Edge::isReadyForProcess() const
	{
		return paramReady;
	}

	bool Edge::resetProcessingState()
	{
		IProcessable::resetProcessingState();
		paramReady = false;
		return true;
	}

	IProcessable::nPtr Edge::opposite(IProcessable::nPtr x)
	{
		if (to == x)
			return from;
		else if (from  == x)
			return to;
		else
			throw Exceptions::NotPartOfEdgeException("The node is not part of the current edge!");
	}

	IProcessable::nPtr Edge::getFrom() const
	{
		return from;
	}

	IProcessable::nPtr Edge::getTo() const
	{
		return to;
	}

	Edge::EdgeTypeEnum Edge::getEdgeType() const
	{
		return default_edge;
	}

}