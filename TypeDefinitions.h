#pragma once
 
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <vector>

#include "IProcessable.hpp"

using std::vector;


// pptr = ProcessablePointer
// nptr = NodePointer
// eptr = EdgePointer







typedef enum e_operations {ADD, SUB, MUL, DIV, SQ, SQRT} operations;
