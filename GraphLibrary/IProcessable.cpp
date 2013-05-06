#pragma once
#include "IProcessable.h"

namespace msonlab
{
	bool IProcessable::registerParameter()
	{
		throw Exceptions::NotImplementedException("IProcessable::registerParameter function");
	}

	bool IProcessable::isReady() const
	{
		throw Exceptions::NotImplementedException("IProcessable::isReady function");
	}

	unsigned int IProcessable::getId() const
	{
		throw Exceptions::NotImplementedException("IProcessable::getId function");
	}

	wchar_t IProcessable::getLabel() const
	{
		throw Exceptions::NotImplementedException("IProcessable::getLabel function");
	}

	Types::DataType IProcessable::getValue() const
	{
		throw Exceptions::NotImplementedException("IProcessable::getValue function");
	}

	bool IProcessable::operator==(const IProcessable& other) const
	{
		throw Exceptions::NotImplementedException("IProcessable::operator== function");
	}
}