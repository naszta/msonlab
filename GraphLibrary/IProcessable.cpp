#pragma once
#include "IProcessable.h"

namespace msonlab
{
	// protected

	bool IProcessable::setProcessed(msonlab::Types::DataType _resultValue)
	{
		resultValue = _resultValue;
		processed = true;

		return true;
	}



	// public 

	IProcessable::IProcessable(unsigned int _id, wchar_t _label, Types::DataType _value)
		: id(_id), label(_label), value(_value)
	{ 
	}

	IProcessable::IProcessable() { };

	bool IProcessable::isProcessed() const
	{
		return processed;
	}

	unsigned int IProcessable::getId() const
	{
		return id;
	}

	wchar_t IProcessable::getLabel() const
	{
		return label;
	}

	Types::DataType IProcessable::getValue() const
	{
		return value;
	}

	Types::DataType IProcessable::getResultValue() const
	{
		if (isProcessed())
			return resultValue;
		else
			throw msonlab::Exceptions::ResultStillNotReadyException("Result for this processable element still not available!");
	}
	

	bool IProcessable::operator==(const IProcessable& other) const
	{
		if (id == other.id && label == other.label && value == other.value)
			return true;
		else
			return false;
	}
}