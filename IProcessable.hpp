#pragma once

template <class DataType>
class IProcessable
{
private:
	unsigned int id;
	wchar_t label;
	DataType value;

	int paramCount;

public:
	typedef boost::shared_ptr<IProcessable<DataType>> pPtr;

	bool registerParameter();
	bool isReady() const;

	virtual bool process() = 0;

	unsigned int getId() const;
	unsigned wchar_t getLabel() const;
	unsigned DataType getValue() const;

	bool operator==(const IProcessable<DataType>& other) const; 
};

