#include <iostream>
#include <source.h>

Image*
source::GetOutput()
{
	return &this->mySourceImg1;
}

void 
source::setFilterVector(vector<Filter*> &FV) {}

/*
void 
source::Update()
{
	char msg[128];
	sprintf(msg,"%s: about to execute\n", SourceName().c_str());
	Logger::LogEvent(msg);
	Execute();
	sprintf(msg,"%s: done executing\n", SourceName().c_str());
	Logger::LogEvent(msg);
}
*/

