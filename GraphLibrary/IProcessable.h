#pragma once
#include "Global.h"
#include "StackRunner.h"

namespace msonlab
{
	class Node;
	class Edge;
	class IProcessable
	{
	protected:
		unsigned int id;
		wchar_t label;

		Types::DataType value;

		bool processed;
		Types::DataType resultValue;

		bool setProcessed(msonlab::Types::DataType _resultValue);
		bool clearProcessed();

	public:
		typedef boost::shared_ptr<IProcessable> pPtr;
		typedef vector<boost::shared_ptr<IProcessable>> pVect;

		typedef boost::shared_ptr<Node> nPtr;
		typedef vector<boost::shared_ptr<Node>> nVect;
		typedef std::set<boost::shared_ptr<Node>> nSet;

		typedef boost::shared_ptr<Edge> ePtr;
		typedef vector<boost::shared_ptr<Edge>> eVect;

		enum PlaceEnum {Input, Inside, Output};

		IProcessable(unsigned int _id, wchar_t _label, Types::DataType _value);
		IProcessable();

		virtual bool registerParameter() = 0;

		virtual IProcessable::pVect process() = 0;
		virtual bool isReadyForProcess() const = 0;
		bool isProcessed() const;
		virtual bool resetProcessingState();

		unsigned int getId() const;
		wchar_t getLabel() const;
		Types::DataType getValue() const;
		Types::DataType getResultValue() const;

		virtual PlaceEnum getPlace() const;

		bool operator==(const IProcessable& other) const;


		// compile
		virtual void compile(msonlab::StackRunner::srPtr stackProgram);
	};

}