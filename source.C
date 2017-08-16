
#include <iostream>
#include <source.h>

Image*
source::GetOutput()
{
	return &this->mySourceImg1;
}

void 
source::setFilterVector(vector<Filter*> &FV) {}


