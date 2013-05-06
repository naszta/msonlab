#pragma once
#include "IProcessable.h"

namespace msonlab
{
	IProcessable::IProcessable(unsigned int _id, wchar_t _label, Types::DataType _value)
		: id(_id), label(_label), value(_value)
	{ 
	}

	IProcessable::IProcessable() { };

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
		if (id == other.id && label == other.label && value == other.value)
			return true;
		else
			return false;
	}
}