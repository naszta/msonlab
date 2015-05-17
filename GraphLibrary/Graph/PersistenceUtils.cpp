#include "PersistenceUtils.h"

#include <array>
#include <stdexcept>

namespace msonlab {	namespace persistence {
	std::array<std::string, 3> _edge_type_names{ {  "SIMPLE",    "BLUE", "MULTIPLIER" } };
	std::array<std::string, 3> _edge_type_color{ { "#000000", "#0000FF",     "#00000" } };
	std::array<std::string, 5> _node_type_names{ {     "ADD", "CONSTANT",  "DIVIDE", "MULTIPLY", "SQUAREROOT" } };
	std::array<std::string, 5> _node_type_color{ { "#00FF00",  "#CC99FF", "#FF9900",  "#FFFF00",    "#00CCFF" } };
	std::string edgeTypeToString(SupportedEdgeType type_){
		return _edge_type_names[type_];
	}
	std::string edgeTypeToColor(SupportedEdgeType type_){
		return _edge_type_color[type_];
	}
	std::string nodeTypeToString(SupportedNodeType type_) {
		return _node_type_names[type_];
	}
	std::string nodeTypeToColor(SupportedNodeType type_) {
		return _node_type_color[type_];
	}
}}