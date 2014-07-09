#pragma once
#include "IProcessable.h"

namespace msonlab
{
	// protected

	bool IProcessable::setProcessed(msonlab::Types::DataPtr _resultValue)
	{
		resultValue = _resultValue;
		processed = true;

		return true;
	}

	bool IProcessable::clearProcessed()
	{
		// resultValue = default
		processed = false;
		return true;
	}

	// public constructors and methods

	IProcessable::IProcessable(unsigned int _id, Types::LabelType _label, Types::DataPtr _value)
		: id(_id), label(_label), value(_value)
	{ 
	}

	IProcessable::IProcessable() { };

	bool IProcessable::isProcessed() const
	{
		return processed;
	}

	bool IProcessable::resetProcessingState()
	{
		clearProcessed();
		return true;
	}

	unsigned int IProcessable::getId() const
	{
		return id;
	}

	Types::LabelType IProcessable::getLabel() const
	{
		return label;
	}

	Types::DataPtr IProcessable::getValue() const
	{
		return value;
	}

	Types::DataPtr IProcessable::getResultValue() const
	{
		if (isProcessed())
			return resultValue;
		else
			throw msonlab::Exceptions::ResultStillNotReadyException("Result for this processable element still not available!");
	}
	
	IProcessable::PlaceEnum IProcessable::getPlace() const
	{
		return IProcessable::Inside;
	}

	bool IProcessable::operator==(const IProcessable& other) const
	{
		if (id == other.id && label == other.label && value == other.value)
			return true;
		else
			return false;
	}

	// compile
	void IProcessable::compile(msonlab::StackRunner::srPtr stackProgram)
	{

	}

}