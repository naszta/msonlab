#pragma once
#include "Global.h"
#include "IProcessable.h"
#include "Node.h"

namespace msonlab
{
	class Edge : public IProcessable
	{
	private:
		Node::nPtr from;
		Node::nPtr to;

		bool paramReady;
	public:
		typedef boost::shared_ptr<Edge> ePtr;
		typedef vector<boost::shared_ptr<Edge>> eVect;

		Edge(unsigned int _id, wchar_t _label, Types::DataType _value, Node::nPtr _from, Node::nPtr _to);
		Edge(const Edge& other);

		bool registerParameter();

		virtual bool process();
		virtual bool isReadyForProcess() const;

		Node::nPtr opposite(Node::nPtr x);

		Node::nPtr getFrom() const;
		Node::nPtr getTo() const;
	};

}