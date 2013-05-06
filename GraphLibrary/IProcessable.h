#pragma once
#include "Global.h"


namespace msonlab
{

	class IProcessable
	{
	protected:
		unsigned int id;
		wchar_t label;

		Types::DataType value;

	public:
		typedef boost::shared_ptr<IProcessable> pPtr;
		typedef vector<boost::shared_ptr<IProcessable>> pVect;

		IProcessable(unsigned int _id, wchar_t _label, Types::DataType _value);
		IProcessable();

		virtual bool registerParameter() = 0;

		virtual bool process() = 0;
		virtual bool isReady() const = 0;

		unsigned int getId() const;
		wchar_t getLabel() const;
		Types::DataType getValue() const;

		bool operator==(const IProcessable& other) const;
	};

}