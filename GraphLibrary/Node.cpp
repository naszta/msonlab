#pragma once
#include "Node.h"
#include "GraphExchanger.h"
#include "Edge.h"


namespace msonlab
{
	Node::Node(unsigned int _id, types::LabelType _label, types::DataPtr _value)
		: IProcessable(_id, _label, _value), paramCount(0), compTime(1)
	{
	}

	Node::Node(unsigned int _id, types::LabelType _label, types::DataPtr _value, unsigned compTime)
		: IProcessable(_id, _label, _value), paramCount(0), compTime(compTime)
	{
	}

	Node::Node(const Node& other)
	{
		throw Exceptions::NotImplementedException("Node copy constructor");
	}

	bool Node::registerParameter()
	{
		++paramCount;

		return isReadyForProcess();
	}

	IProcessable::pVect Node::process()
	{
		throw Exceptions::NotImplementedException("Node::process function");
	}

	bool Node::isReadyForProcess() const
	{
		if (predecessors.size() == paramCount)
			return true;
		else
			return false;
	}

	bool Node::resetProcessingState()
	{
		IProcessable::resetProcessingState();
		paramCount = 0;
		return true;
	}

	const IProcessable::eVect& Node::getPredecessors() const
	{
		return predecessors;
	}

	size_t Node::getPredecessorsSize() const
	{
		return predecessors.size();
	}

	unsigned Node::getPredecessorNodeId(size_t index) const
	{
		return predecessors[index]->getFromId();
	}

	const IProcessable::ePtr& Node::getPredecessor(size_t index) const
	{
		return predecessors[index];
	}

	IProcessable::eVect::iterator Node:: getPredecessorBegin()
	{
		return predecessors.begin();
	}

	IProcessable::eVect::iterator Node:: getPredecessorEnd()
	{
		return predecessors.end();
	}

	const IProcessable::eVect& Node::getSuccessors() const
	{
		return successors;
	}

	std::string Node::getIdString() const
	{
		std::string ret("n");
		ret = ret + std::to_string(id);
		return ret;
	}

	size_t Node::getSuccessorsSize() const
	{
		return successors.size();
	}

	bool Node::registerPredecessor(IProcessable::ePtr _newPredecessor)
	{
		msonlab::IProcessable::eVect::iterator it;
		it = std::find(predecessors.begin(),predecessors.end(),_newPredecessor);

		if (it != predecessors.end())
			return false;

		it = predecessors.insert(predecessors.end(),_newPredecessor);

		if (*it == _newPredecessor)
			return true;
		else
		{
			throw Exceptions::FailedToAddEdgeException("Failed to register new predecessor to the node!");
			return false;
		}

	}

	bool Node::unregisterPredecessor(IProcessable::ePtr _newPredecessor)
	{
		throw Exceptions::NotImplementedException("Node::unregisterPredecessor function");
	}

	bool Node::registerSuccessor(IProcessable::ePtr _newSuccessor)
	{
		msonlab::IProcessable::eVect::iterator it;
		it = std::find(successors.begin(),successors.end(),_newSuccessor);

		if (it != successors.end())
			return false;

		it = successors.insert(successors.end(),_newSuccessor);

		if (*it == _newSuccessor)
			return true;
		else
		{
			throw Exceptions::FailedToAddEdgeException("Failed to register new successor to the node!");
			return false;
		}
	}

	bool Node::unregisterSuccessor(IProcessable::ePtr _newSuccessor)
	{
		throw Exceptions::NotImplementedException("Node::unregisterSuccessor function");
	}


	IProcessable::PlaceEnum Node::getPlace() const
	{
		if (predecessors.size() > 0 && successors.size() > 0)
			return IProcessable::Inside;
		else if (predecessors.size() > 0)
			return IProcessable::Output;
		else
			return IProcessable::Input;
	}

	// compile
	void Node::compile(int thread_id, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{

	}

	// exchange

	DOMElement* Node::serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey)
	{
		DOMElement* newNode = xmlDocument->createElement(L"node");
		newNode->setAttribute(L"id", XMLString::transcode(getIdString().c_str()));
		

		// create custom data for deserialization
		DOMElement* customData = xmlDocument->createElement(L"data");
		customData->setAttribute(L"key", XMLString::transcode(typeKeyName.c_str()));
		DOMText* lbl_nodeType = xmlDocument->createTextNode(XMLString::transcode(getTypeString().c_str()));
		customData->appendChild(lbl_nodeType);
		newNode->appendChild(customData);

		DOMElement* customData2 = xmlDocument->createElement(L"data");
		customData2->setAttribute(L"key", XMLString::transcode(customDataKey.c_str()));
		DOMText* lbl_nodeCustomData = xmlDocument->createTextNode(XMLString::transcode(get_custom_data().c_str()));
		customData2->appendChild(lbl_nodeCustomData);
		newNode->appendChild(customData2);


		// create data for YED
		DOMElement* nodeData = xmlDocument->createElement(L"data");
		nodeData->setAttribute(L"key", XMLString::transcode(yedDataKeyName.c_str()));
		newNode->appendChild(nodeData);

		DOMElement* sn = xmlDocument->createElement(L"y:ShapeNode");
		nodeData->appendChild(sn);

		DOMElement* geometry = xmlDocument->createElement(L"y:Geometry");
		geometry->setAttribute(L"height", L"30.0");
		geometry->setAttribute(L"width", L"30.0");
		geometry->setAttribute(L"x", L"0.0");
		geometry->setAttribute(L"y", L"0.0");
		sn->appendChild(geometry);

		DOMElement* fill = xmlDocument->createElement(L"y:Fill");
		fill->setAttribute(L"color", XMLString::transcode(get_color().c_str()));
		fill->setAttribute(L"transparent", L"false");
		sn->appendChild(fill);

		DOMElement* borderStyle = xmlDocument->createElement(L"y:BorderStyle");
		borderStyle->setAttribute(L"color", L"#000000");
		borderStyle->setAttribute(L"type", L"line");
		borderStyle->setAttribute(L"width", L"1.0");
		sn->appendChild(borderStyle);

		DOMElement* nodeLabel = xmlDocument->createElement(L"y:NodeLabel");
		nodeLabel->setAttribute(L"alignment", L"center");
		nodeLabel->setAttribute(L"autoSizePolicy", L"content");
		nodeLabel->setAttribute(L"fontFamily", L"Dialog");
		nodeLabel->setAttribute(L"fontSize", L"12");
		nodeLabel->setAttribute(L"fontStyle", L"plain");
		nodeLabel->setAttribute(L"hasBackgroundColor", L"false");
		nodeLabel->setAttribute(L"hasLineColor", L"false");
		nodeLabel->setAttribute(L"height", L"18.0");
		nodeLabel->setAttribute(L"modelName", L"internal");
		nodeLabel->setAttribute(L"modelPosition", L"c");
		nodeLabel->setAttribute(L"textColor", L"#000000");
		nodeLabel->setAttribute(L"visible", L"true");
		nodeLabel->setAttribute(L"width", L"10.0");
		nodeLabel->setAttribute(L"x", L"0.0");
		nodeLabel->setAttribute(L"y", L"0.0");
		std::string str(label.begin(), label.end());
		DOMText* lbl = xmlDocument->createTextNode(XMLString::transcode(str.c_str()));
		nodeLabel->appendChild(lbl);
		sn->appendChild(nodeLabel);

		DOMElement* shp = xmlDocument->createElement(L"y:Shape");
		shp->setAttribute(L"type", XMLString::transcode(get_shape().c_str()));
		sn->appendChild(shp);
		

		return newNode;
	}

	std::string Node::getTypeString() const
	{
		throw new Exceptions::NodeTypeCanNotBeSerializedException("NodeType can not be serialized. Please use one inherited class for the graph.");
		//GraphExchanger::getSupportedNodeTypeName(GraphExchanger::supportedNodeType::ADD);
	}

	std::string Node::get_shape() const
	{
		return "roundrectangle";
	}

	std::string Node::get_color() const
	{
		return "#FFFFFF";
	}

}