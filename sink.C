
#include <sink.h>
sink::sink()
{
	Img1HasBeenSet = false;
	Img2HasBeenSet = false;
	myImg1 = NULL;
	myImg2 = NULL;	
	HasBeenSetTwice = false;
}
const Image*
sink::GetImg1()
{
	return this->myImg1;
}
const Image*
sink::GetImg2()
{
	return this->myImg2;
}
void
sink::SetInput(Image* input1)
{
	
	if(Img1HasBeenSet)
	{	
		HasBeenSetTwice = true;	
	}	

	this->myImg1 = input1;
	Img1HasBeenSet = true;
}
void
sink::SetInput2(Image* input2)
{
	if(Img2HasBeenSet)
	{
		HasBeenSetTwice = true;	
	}	
	this->myImg2 = input2;
	Img2HasBeenSet = true;
}
