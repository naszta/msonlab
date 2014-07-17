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
		types::LabelType label;

		types::DataPtr value;

		bool processed;
		types::DataPtr resultValue;

		bool setProcessed(msonlab::types::DataPtr _resultValue);
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
		typedef vector<std::shared_ptr<Edge>> eVect;

		enum PlaceEnum {Input, Inside, Output};

		IProcessable(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		IProcessable();

		virtual bool registerParameter() = 0;

		virtual IProcessable::pVect process() = 0;
		virtual bool isReadyForProcess() const = 0;
		bool isProcessed() const;
		virtual bool resetProcessingState();

		unsigned int getId() const;
		virtual std::string getIdString() const;
		virtual types::LabelType getLabel() const;
		types::DataPtr getValue() const;
		types::DataPtr getResultValue() const;

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