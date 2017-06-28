#include <source.h>
#include <sink.h>
#include <vector>
#ifndef FILTER_H
#define FILTER_H
using namespace::std;
class Filter : public source, public sink
{
	protected:
		bool hasTwo;
		vector<Filter*> filterVector; 
	public:
		void Update();
		virtual const char* FilterName() = 0;
		virtual const char* SourceName() {return FilterName();};
		virtual const char* SinkName() {return FilterName();}; 
		virtual void FilterInputIsValid() = 0;
		virtual void SourceInputIsValid() {FilterInputIsValid();};
		virtual void SinkInputIsValid() {FilterInputIsValid();};
		void setFilterVector(vector<Filter*> &FV);
		vector<Filter*>& getFilterVector();
};

#endif
