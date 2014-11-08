#pragma once
#include "Edge.h"
#include "Node.h"
#include "GraphExchanger.h"

namespace msonlab
{
	Edge::Edge(unsigned int _id, types::LabelType _label, types::DataPtr _value, NodePtr _from, NodePtr _to)
		: IProcessable(_id, _label, _value), from(_from), to(_to), paramReady(false)
	{
	}

	Edge::Edge(const Edge& other) : IProcessable(other)
	{
		if (this != &other)
		{
			*this = other;
		}
	}

	Edge& Edge::operator=(const Edge& other) 
	{
		IProcessable::operator=(other);

		this->paramReady = other.paramReady;
		this->from = other.from;
		this->to = other.to;

		return *this;
	}

	bool Edge::registerParameter()
	{
		paramReady = true;

		return true;
	}

	IProcessableVect Edge::process()
	{
		if (isReadyForProcess())
		{
			if (setProcessed((*from).getResultValue()))
			{
				IProcessableVect retVal;
				
				if((*to).registerParameter())
					retVal.insert(retVal.begin(), to);

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

	NodePtr Edge::opposite(NodePtr x)
	{
		if (to == x)
			return from;
		else if (from  == x)
			return to;
		else
			throw Exceptions::NotPartOfEdgeException("The node is not part of the current edge!");
	}

	NodePtr Edge::getFrom() const
	{
		return from;
	}

	NodePtr Edge::getTo() const
	{
		return to;
	}

	unsigned Edge::getFromId() const
	{
		return from->id();
	}

	unsigned Edge::getToId() const
	{
		return to->id();
	}

	std::string Edge::getIdString() const
	{
		std::string ret("e");
		ret = ret + std::to_string(id());
		return ret;
	}

	Edge::EdgeTypeEnum Edge::getEdgeType() const
	{
		return default_edge;
	}
	// compile

	void Edge::compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{
		unsigned int thread_id = schedule.at(id());
		StackRunner::program* prog = &(programs->at(thread_id));

		// if already synced, just push the associated future object to the stack
		if (synced)
		{
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, id())));
			return;
		}

		if (from->compile_iteration < compile_iteration)
		{
			// already has value
			StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, from->id())));
		}
		else
		{
			// going deeper and calculate predecessor
			from->compile(thread_id, programs, schedule);

			// if predecessor is placed on another thread, add a wait operation
			if (schedule.at(from->id()) != thread_id)
			{
				// predecessor is on another thread, need to create a FutureStackValue
				StackRunner::addToken(prog, StackRunner::WAIT, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, from->id())));
			}
		}

		// manage sync-s
		if (!synced && extra_sync_marker)
		{
			// if need to sync but called from the same thread, DUP is needed
			if (caller_thread != -1)
			{
				if (caller_thread == thread_id)
				{
					StackRunner::addToken(prog, StackRunner::DUP, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, -1)));
				}
			}

			StackRunner::addToken(prog, StackRunner::SYNC, StackRunner::dataToken(new std::pair<StackValue::stackvaluePtr, int>(nullptr, id())));
			set_synced();
		}

	}


	// exchange

	DOMElement* Edge::serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey)
	{
		DOMElement* newEdge = xmlDocument->createElement(L"edge");
		newEdge->setAttribute(L"id", XMLString::transcode(getIdString().c_str()));
		newEdge->setAttribute(L"source", XMLString::transcode(from->getIdString().c_str()));
		newEdge->setAttribute(L"target", XMLString::transcode(to->getIdString().c_str()));

		// create data for YED
		DOMElement* edgeData = xmlDocument->createElement(L"data");
		edgeData->setAttribute(L"key", XMLString::transcode(yedDataKeyName.c_str()));
		newEdge->appendChild(edgeData);

		DOMElement* ple = xmlDocument->createElement(L"y:PolyLineEdge");
		edgeData->appendChild(ple);

		DOMElement* line = xmlDocument->createElement(L"y:LineStyle");
		line->setAttribute(L"color", XMLString::transcode(get_color().c_str()));
		line->setAttribute(L"type", XMLString::transcode(get_line_style().c_str()));
		line->setAttribute(L"width", L"1.0");
		ple->appendChild(line);

		DOMElement* arrow = xmlDocument->createElement(L"y:Arrows");
		arrow->setAttribute(L"source", L"none");
		arrow->setAttribute(L"target", XMLString::transcode(get_target_arrow_style().c_str()));
		ple->appendChild(arrow);


		// create custom data for deserialization
		DOMElement* customData = xmlDocument->createElement(L"data");
		customData->setAttribute(L"key", XMLString::transcode(typeKeyName.c_str()));
		DOMText* lbl_edgeType = xmlDocument->createTextNode(XMLString::transcode(getTypeString().c_str()));
		customData->appendChild(lbl_edgeType);
		newEdge->appendChild(customData);

		DOMElement* customData2 = xmlDocument->createElement(L"data");
		customData2->setAttribute(L"key", XMLString::transcode(customDataKey.c_str()));
		DOMText* lbl_edgeCustomData = xmlDocument->createTextNode(XMLString::transcode(get_custom_data().c_str()));
		customData2->appendChild(lbl_edgeCustomData);
		newEdge->appendChild(customData2);

		return newEdge;
	}

	std::string Edge::getTypeString() const
	{
		return GraphExchanger::getSupportedEdgeTypeName(GraphExchanger::supportedEdgeType::SIMPLE);
	}

	std::string Edge::get_target_arrow_style() const
	{
		// plain, standard
		return "standard";
	}

	std::string Edge::get_line_style() const
	{
		// line, dotted, dashed_dotted
		return "line";
	}

	std::string Edge::get_color() const
	{
		return "#000000";
	}

}