#pragma once
#include "Global.h"


namespace msonlab
{

	class IProcessable
	{
	private:
		unsigned int id;
		wchar_t label;

		Types::DataType value;

		int paramCount;

	public:
		typedef boost::shared_ptr<IProcessable> pPtr;
		typedef vector<boost::shared_ptr<IProcessable>> pVect;


		bool registerParameter();
		bool isReady() const;

		virtual bool process() = 0;

		unsigned int getId() const;
		wchar_t getLabel() const;
		Types::DataType getValue() const;

		bool operator==(const IProcessable& other) const;
	};

}