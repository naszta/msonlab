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
		std::string label;

		Types::DataType value;

		bool processed;
		Types::DataType resultValue;

		bool setProcessed(msonlab::Types::DataType _resultValue);
		bool clearProcessed();

		// compile
		bool extra_sync_marker = false;

	public:
		bool synced = false;

		typedef std::shared_ptr<IProcessable> pPtr;
		typedef vector<std::shared_ptr<IProcessable>> pVect;

		typedef std::shared_ptr<Node> nPtr;
		typedef vector<std::shared_ptr<Node>> nVect;
		typedef std::set<std::shared_ptr<Node>> nSet;

		typedef std::shared_ptr<Edge> ePtr;
		typedef vector<ePtr> eVect;

		enum PlaceEnum {Input, Inside, Output};

		IProcessable(unsigned int _id, std::string _label, Types::DataType _value);
		IProcessable();

		virtual bool registerParameter() = 0;

		virtual IProcessable::pVect process() = 0;
		virtual bool isReadyForProcess() const = 0;
		bool isProcessed() const;
		virtual bool resetProcessingState();

		unsigned int getId() const;
		virtual std::string getIdString() const;
		virtual std::string getLabel() const;
		Types::DataType getValue() const;
		Types::DataType getResultValue() const;

		virtual PlaceEnum getPlace() const;

		bool operator==(const IProcessable& other) const;


		// compile
		virtual void compile(int caller_thread, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule);

		void set_sync_marker();
		void clear_sync_marker();
		void set_synced();
		void clear_synced();

		int compile_iteration;


		// exchange
		virtual DOMElement* serialize(DOMDocument* xmlDocument, std::string yedDataKeyName, std::string typeKeyName, std::string customDataKey);
		virtual std::string getTypeString() const;
		virtual std::string get_custom_data() const;
	};

}