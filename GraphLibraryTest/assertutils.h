#ifndef ASSERTUTILS_H
#define ASSERTUTILS_H

#include <vector>
#include "CppUnitTest.h"

namespace GraphLibraryTest
{
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;
	using std::vector;

	void AreEqual(const vector<unsigned>& a, const vector<unsigned>& b)
	{
		Assert::AreEqual(a.size(), b.size(), L"Size not equal");
	}
}

#endif