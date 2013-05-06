#pragma once
#include "IProcessable.h"

namespace msonlab
{
	bool IProcessable::registerParameter()
	{
		return false;
	}

	bool IProcessable::isReady() const
	{
		return false;
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

	bool IProcessable::operator==(const IProcessable& other) const
	{
		return false;
	}
}