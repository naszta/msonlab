#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <xercesc\util\PlatformUtils.hpp>
XERCES_CPP_NAMESPACE_USE

#include<iostream>
#include<vector>

using namespace std;

int main() {
		{
			cout << "hello" << endl;
			XMLPlatformUtils::Terminate();
		}
	_CrtDumpMemoryLeaks();
}