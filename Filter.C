/*
#include <Filter.h>

void
Filter::Update()
{
	if(hasTwo)
	{
		if(myImg1->getStatus() && myImg2->getStatus())
		{
			this->Execute();
			mySourceImg1.isUpdated();
		}else{
			myImg1->Update();
			myImg2->Update();

			this->Execute();
			mySourceImg1.isUpdated();
		}			
	}
	else
	{
		if(myImg1->getStatus())
		{
			this->Execute();
			mySourceImg1.isUpdated();
		}
		else
		{
			myImg1->Update();
			this->Execute();
			mySourceImg1.isUpdated();
		}
	}
}



*/
