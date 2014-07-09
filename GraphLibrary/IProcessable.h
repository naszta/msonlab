#pragma once
#include "Global.h"
#include "StackRunner.h"
#include <string>

namespace msonlab
{
	class Node;
	class Edge;
	class IProcessable
	{
	protected:
		unsigned int id;
		//wchar_t label;
		Types::LabelType label;

		Types::DataPtr value;

		bool processed;
		Types::DataPtr resultValue;

		bool setProcessed(msonlab::Types::DataPtr _resultValue);
		bool clearProcessed();

	public:
		typedef std::shared_ptr<IProcessable> pPtr;
		typedef vector<std::shared_ptr<IProcessable>> pVect;

		typedef std::shared_ptr<Node> nPtr;
		typedef vector<std::shared_ptr<Node>> nVect;
		typedef std::set<std::shared_ptr<Node>> nSet;

		typedef std::shared_ptr<Edge> ePtr;
		typedef vector<std::shared_ptr<Edge>> eVect;

		enum PlaceEnum {Input, Inside, Output};

		IProcessable(unsigned int _id, Types::LabelType _label, Types::DataPtr _value);
		IProcessable();

		virtual bool registerParameter() = 0;

		virtual IProcessable::pVect process() = 0;
		virtual bool isReadyForProcess() const = 0;
		bool isProcessed() const;
		virtual bool resetProcessingState();

		unsigned int getId() const;
		Types::LabelType getLabel() const;
		Types::DataPtr getValue() const;
		Types::DataPtr getResultValue() const;

		virtual PlaceEnum getPlace() const;

		bool operator==(const IProcessable& other) const;

		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};

}