#include "IProcessable.h"

namespace msonlab
{
	// protected

	bool IProcessable::setProcessed(msonlab::types::DataPtr _resultValue)
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

	IProcessable::IProcessable(unsigned int id_, types::LabelType label_, types::DataPtr value_)
		: _id(id_), label(label_), value(value_)
	{ 
	}

	IProcessable::IProcessable(const IProcessable& other) 
	{
		if (this != &other) {
			*this = other;
		}
	};

	IProcessable& IProcessable::operator=(const IProcessable& other)
	{
		this->_id = other._id;
		this->label = other.label;
		this->value = other.value;

		return *this;
	}

	bool IProcessable::isProcessed() const
	{
		return processed;
	}

	bool IProcessable::resetProcessingState()
	{
		clearProcessed();
		return true;
	}

	unsigned int IProcessable::id() const
	{
		return _id;
	}

	std::string IProcessable::getIdString() const
	{
		return "i" + _id;
	}

	types::LabelType IProcessable::getLabel() const
	{
		return label;
	}

	types::DataPtr IProcessable::getValue() const
	{
		return value;
	}

	types::DataPtr IProcessable::getResultValue() const
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
		if (_id == other._id && label == other.label && value == other.value)
			return true;
		else
			return false;
	}

	// compile
	/*void IProcessable::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{

	}*/

	void IProcessable::set_sync_marker()
	{
		extra_sync_marker = true;
	}

	void IProcessable::clear_sync_marker()
	{
		extra_sync_marker = false;
	}

	void IProcessable::set_synced()
	{
		synced = true;
	}

	void IProcessable::clear_synced()
	{
		synced = false;
	}

	// exchange
	std::string IProcessable::get_custom_data() const
	{
		return "";
	}
}