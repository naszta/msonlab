#include "Node.h"
#include "GraphExchanger.h"
#include "Edge.h"

namespace msonlab
{
	Node::Node(unsigned int id_, types::LabelType label_, types::DataPtr value_, string type_string_, unsigned compTime_)
		: IProcessable(id_, label_, value_), type_string(type_string_), paramCount(0), compTime(compTime_)
	{
	}

	Node::Node(const Node& other) : IProcessable(other)
	{
		if (this != &other)
		{
			*this = other;
		}
	}

	// does NOT copy connections
	Node& Node::operator=(const Node& other)
	{
		IProcessable::operator=(other);

		this->paramCount = other.paramCount;
		this->type_string = other.type_string;
		this->compTime = other.compTime;

		return *this;
	}

	bool Node::registerParameter()
	{
		++paramCount;

		return isReadyForProcess();
	}

	bool Node::isReadyForProcess() const
	{
		if (_predecessors.size() == paramCount)
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

	const EdgeVect& Node::getPredecessors() const
	{
		return _predecessors;
	}

	size_t Node::getPredecessorsSize() const
	{
		return _predecessors.size();
	}

	unsigned Node::getPredecessorNodeId(size_t index) const
	{
		return _predecessors[index]->getFromId();
	}

	const EdgePtr& Node::getPredecessor(size_t index) const
	{
		return _predecessors[index];
	}

	EdgeVect::iterator Node::getPredecessorBegin()
	{
		return _predecessors.begin();
	}

	EdgeVect::iterator Node::getPredecessorEnd()
	{
		return _predecessors.end();
	}

	const EdgeVect& Node::getSuccessors() const
	{
		return _successors;
	}

	std::string Node::getIdString() const
	{
		std::string ret("n");
		ret = ret + std::to_string(id());
		return ret;
	}

	size_t Node::s_size() const
	{
		return _successors.size();
	}

	void Node::addPreNode(NodePtr toAdd) {
		preNodes.push_back(toAdd);
	}

	void Node::addSucNode(NodePtr toAdd) {
		sucNodes.push_back(toAdd);
	}

	bool Node::registerPredecessor(EdgePtr _newPredecessor)
	{
		msonlab::EdgeVect::iterator it;
		it = std::find(_predecessors.begin(), _predecessors.end(), _newPredecessor);

		if (it != _predecessors.end())
			return false;

		it = _predecessors.insert(_predecessors.end(), _newPredecessor);
		addPreNode(_newPredecessor->getFrom());

		if (*it == _newPredecessor)
			return true;
		else
		{
			throw Exceptions::FailedToAddEdgeException("Failed to register new predecessor to the node!");
			return false;
		}

	}

	bool Node::unregisterPredecessor(EdgePtr _newPredecessor)
	{
		throw Exceptions::NotImplementedException("Node::unregisterPredecessor function");
	}

	bool Node::registerSuccessor(EdgePtr _newSuccessor)
	{
		msonlab::EdgeVect::iterator it;
		it = std::find(_successors.begin(), _successors.end(), _newSuccessor);

		if (it != _successors.end())
			return false;

		it = _successors.insert(_successors.end(), _newSuccessor);
		addSucNode(_newSuccessor->getTo());

		if (*it == _newSuccessor)
			return true;
		else
		{
			throw Exceptions::FailedToAddEdgeException("Failed to register new successor to the node!");
			return false;
		}
	}

	bool Node::unregisterSuccessor(EdgePtr _newSuccessor)
	{
		throw Exceptions::NotImplementedException("Node::unregisterSuccessor function");
	}


	IProcessable::PlaceEnum Node::getPlace() const
	{
		if (_predecessors.size() > 0 && _successors.size() > 0)
			return IProcessable::Inside;
		else if (_predecessors.size() > 0)
			return IProcessable::Output;
		else
			return IProcessable::Input;
	}

	NodeVect Node::predecessors() const {
		NodeVect res;
		for (auto& weak_node : preNodes)
			res.push_back(weak_node.lock());
		return res;
	}

	// compile
	void Node::compile(int thread_id, vector<msonlab::StackRunner::program>* programs, StackRunner::scheduleOrder schedule)
	{

	}

	// exchange
	std::string Node::getTypeString() const
	{
		if (type_string.empty()) {
			throw new Exceptions::NodeTypeCanNotBeSerializedException("NodeType can not be serialized. Please use one inherited class for the graph.");
		}

		return type_string;
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