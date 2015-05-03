#pragma once
#include "Global.h"
#include "StackRunner.h"
#include <string>

#include <xercesc\util\PlatformUtils.hpp>
#include <xercesc\dom\DOM.hpp>
#include <xercesc\framework\LocalFileFormatTarget.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\sax\HandlerBase.hpp>
XERCES_CPP_NAMESPACE_USE

namespace msonlab
{
	class Node;
	class Edge;
	class Graph;
	class IProcessable;
	namespace lite {
		class litenode;
	}
	
	typedef const lite::litenode* LWNodePtr;
	typedef vector<const lite::litenode*> LWNodeVect;

	typedef std::shared_ptr<IProcessable> IProcessablePtr;
	typedef vector<std::shared_ptr<IProcessable>> IProcessableVect;

	typedef std::shared_ptr<Node> NodePtr;
	typedef vector<const NodePtr> NodeVect;
	typedef std::set<NodePtr> NodeSet;

	typedef std::shared_ptr<Edge> EdgePtr;
	typedef vector<std::shared_ptr<Edge>> EdgeVect;

	namespace graph {
		namespace algorithms {
		Graph computeChangedGraph(const Graph &graph, NodeSet changed, NodeSet needed);
	}}

	class IProcessable
	{
		friend class Graph;
		friend Graph graph::algorithms::computeChangedGraph(const Graph &graph, NodeSet changed, NodeSet needed);
	protected:
		unsigned int _id;
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
		enum PlaceEnum {Input, Inside, Output};

		IProcessable(unsigned int _id, types::LabelType _label, types::DataPtr _value);
		IProcessable(const IProcessable& other);
		IProcessable& operator=(const IProcessable& other);

		virtual bool registerParameter() = 0;

		virtual IProcessableVect process() = 0;
		virtual bool isReadyForProcess() const = 0;
		bool isProcessed() const;	
		virtual bool resetProcessingState();

		unsigned int id() const;
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