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

	std::string IProcessable::getIdString() const
	{
		return "i" + id;
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
	void IProcessable::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{

	}

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

	DOMElement* IProcessable::serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey)
	{
		DOMElement* newNode = xmlDocument->createElement(L"node");
		newNode->setAttribute(L"id", XMLString::transcode(getIdString().c_str()));

		// create data for YED
		DOMElement* nodeData = xmlDocument->createElement(L"data");
		nodeData->setAttribute(L"key", XMLString::transcode(yedDataKeyName.c_str()));
		newNode->appendChild(nodeData);

		// create custom data for deserialization
		DOMElement* customData = xmlDocument->createElement(L"data");
		customData->setAttribute(L"id", XMLString::transcode(typeKeyName.c_str()));
		newNode->appendChild(customData);

		return newNode;
	}

	std::string IProcessable::getTypeString() const
	{
		throw new Exceptions::NodeTypeCanNotBeSerializedException("IProcessable can not be serialized. Please use one inherited class for the graph.");
		//GraphExchanger::getSupportedNodeTypeName(GraphExchanger::supportedNodeType::ADD);
	}

	std::string IProcessable::get_custom_data() const
	{
		return "";
	}


}