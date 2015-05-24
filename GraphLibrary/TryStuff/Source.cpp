#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#include<iostream>
#include<vector>

using namespace std;

int main() {
	{
		vector<bool> v(10, false);
		auto b = v[2];
		if (!b) {
			cout << "true";
		}
		else {
			cout << "false";
		}
	}
	_CrtDumpMemoryLeaks();
}