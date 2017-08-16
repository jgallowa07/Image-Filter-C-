
#include <iostream>
#include <logging.h>
#include <image.h>
#ifndef SOURCE_H
#define SOURCE_H

using namespace std;
class Filter;
class source 
{
	protected:
		Image mySourceImg1;
		//virtual void Update();
		virtual void Execute()=0;

	public: 
		Image* GetOutput();	
		virtual void Update() = 0;
		virtual const char* SourceName() = 0;
		virtual void SourceInputIsValid() = 0;
		virtual void setFilterVector(vector<Filter*>&);
		//virtual const char* SourceName() = 0;
		
};

#endif 
