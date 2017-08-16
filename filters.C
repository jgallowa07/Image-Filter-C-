
#include <stdio.h>
#include <iostream>
#include <filters.h>
#include <Filter.h>
#include <sys/time.h>


//--------------------------- Filter ---------------------------- 


void
Filter::setFilterVector(vector<Filter*> &FV)
{
	for(int i = 0; i < FV.size(); i++)
	{
		if(FV[i] == this)
		{
			char msg[1024];
			sprintf(msg,"Cyclical Update Flow");
			DataFlowException e(FilterName(),msg);
			throw e;
		}
		else
		{
			filterVector.push_back(FV[i]);
		}
	}
	filterVector.push_back(this);
}

vector<Filter*>&
Filter::getFilterVector()
{
	return filterVector;
}

void
Filter::Update()
{

	if(!Img1HasBeenSet)
	{
	
		char msg[1024];
		sprintf(msg,"One of the inputs has not been set");
		DataFlowException e(FilterName(),msg);
		throw e;
	}
	char msg[1024];
	if(hasTwo)
	{
		if(!Img2HasBeenSet)
		{
			char msg[1024];
			sprintf(msg,"Input 2 has not been set");
			DataFlowException e(FilterName(),msg);
			throw e;
		}
		if(!myImg1->getStatus() || !myImg2->getStatus())
		{	
			sprintf(msg,"%s: about to update\n", FilterName() );
			Logger::LogEvent(msg);
			if(myImg1 == &mySourceImg1)
			{	
				char msg[1024];
				sprintf(msg,"Input 1 has been set as this output");
				DataFlowException e(FilterName(),msg);
				throw e;
			}
			myImg1->setsocketVector(this->getFilterVector());				
			myImg1->Update();
			if(myImg2 == &mySourceImg1)
			{	
				char msg[1024];
				sprintf(msg,"Input 2 has been set as this output");
				DataFlowException e(FilterName(),msg);
				throw e;
			}
			myImg2->setsocketVector(this->getFilterVector());				
			myImg2->Update();
			sprintf(msg,"%s: done updating\n", FilterName() );
			Logger::LogEvent(msg);	
		}
		if(!mySourceImg1.getStatus())
		{		
			sprintf(msg,"%s: about to execute\n", FilterName() );
			Logger::LogEvent(msg);
	
			struct timeval startTime;	
			gettimeofday(&startTime,0);
			this->Execute();
			struct timeval endTime;
			gettimeofday(&endTime,0);
			
		double seconds = double(endTime.tv_sec - startTime.tv_sec) + double(endTime.tv_usec - startTime.tv_usec)/1000000.;

			mySourceImg1.isUpdated();
			sprintf(msg,"%s: done executing, time : %f\n", FilterName(),seconds );
			Logger::LogEvent(msg);
		}			
	}
	else
	{
		if(!myImg1->getStatus())
		{
			sprintf(msg,"%s: about to update\n", FilterName() );
			Logger::LogEvent(msg);
			if(myImg1 == &mySourceImg1)
			{	
				char msg[1024];
				sprintf(msg,"Input 1 has been set as this output");
				DataFlowException e(FilterName(),msg);
				throw e;
			}
			myImg1->setsocketVector(this->getFilterVector());				
			myImg1->Update();
			sprintf(msg,"%s: done updating\n", FilterName() );
			Logger::LogEvent(msg);
		}
		if(!mySourceImg1.getStatus())
		{
			sprintf(msg,"%s: about to execute\n", FilterName() );
			Logger::LogEvent(msg);

			struct timeval startTime;	
			gettimeofday(&startTime,0);
			this->Execute();
			struct timeval endTime;
			gettimeofday(&endTime,0);
			
		double seconds = double(endTime.tv_sec - startTime.tv_sec) + double(endTime.tv_usec - startTime.tv_usec)/1000000.;

			mySourceImg1.isUpdated();
			sprintf(msg,"%s: done executing, time : %f\n", FilterName(),seconds );
			Logger::LogEvent(msg);

		}
	}
}
				
//--------------------------- Shrinker ---------------------------- 

Shrinker::Shrinker()
{
	hasTwo = false;
	mySourceImg1.setsource(this);
}

const char*
Shrinker::FilterName()
{
	return "Shrinker";
}

void
Shrinker::FilterInputIsValid()
{
	if(!Img1HasBeenSet)
	{
		char msg[1024];
		sprintf(msg,"One of the inputs has not been set");
		DataFlowException e(FilterName(),msg);
		throw e;
	}
}

void 
Shrinker::Execute()
{
	FilterInputIsValid();	
	HalveInSize(*this->myImg1,this->mySourceImg1);
}

void
Shrinker::HalveInSize(const Image &input, Image &output)
{
	output.ResetSize(input.getWidth()/2,input.getHeight()/2);
	int o_height = output.getHeight();
	int o_width = output.getWidth();	

	for(int i = 0 ; i < o_height ; i++)
	{
		for(int j = 0 ; j < o_width ; j++)
		{
			output.getBuffer()[(i*o_width)+j] = input.getBuffer()[(2*i*input.getWidth())+(2*j)];
		}
	}
		

}

//--------------------------- LRConcat ---------------------------- 

LRConcat::LRConcat()
{
	hasTwo = true;
	mySourceImg1.setsource(this);
}

const char*
LRConcat::FilterName()
{
	return "LRConcat";
}

void
LRConcat::FilterInputIsValid()
{
	if(!Img1HasBeenSet||!Img2HasBeenSet)
	{
		char msg[1024];
		sprintf(msg,"One of the inputs has not been set");
		DataFlowException e(FilterName(),msg);
		throw e;
	}
	
	if(myImg1->getHeight() != myImg2->getHeight())
	{
		char msg[1024];
		sprintf(msg,"Heights must match: %d %d",myImg1->getHeight(),myImg2->getHeight());
		DataFlowException e(FilterName(),msg);
		throw e;
	}
}

void
LRConcat::Execute()
{
	
	FilterInputIsValid();	
	LeftRightConcatenate(*this->myImg1,*this->myImg2,this->mySourceImg1);
	
}

void 
LRConcat::LeftRightConcatenate(const Image &leftInput,const Image &rightInput, Image &output)
{
	int in1_width = leftInput.getWidth();
	int in1_height = leftInput.getHeight();
	int in2_width = rightInput.getWidth();
	int in2_height = rightInput.getHeight();
	int out_width = in1_width + in2_width;	
	if(myImg1->getHeight() != myImg2->getHeight())
	{
		char msg[1024];
		sprintf(msg,"Heights must match: %d %d",myImg1->getHeight(),myImg2->getHeight());
		DataFlowException e(FilterName(),msg);
		throw e;
	}
	
	output.ResetSize(out_width,leftInput.getHeight());
	for(int row = 0; row < in1_height; row++)
	{
		for(int col = 0; col < in1_width; col++)
		{
			int out_col = col;
			int out_row = row;
			output.setPixel(out_row, out_col, leftInput.getPixel(row,col));
		}
	}
	for(int row = 0; row < in1_height; row ++)
	{
		for(int col = 0; col < in2_width; col++)
		{
			int out_col = col + in1_width;;
			int out_row = row;
			output.setPixel(out_row, out_col, rightInput.getPixel(row,col));
		}
	} 
}


//--------------------------- TBConcat ---------------------------- 

TBConcat::TBConcat()
{
	hasTwo = true;
	mySourceImg1.setsource(this);
}

const char*
TBConcat::FilterName()
{
	return "TBConcat";
}

void
TBConcat::FilterInputIsValid()
{
	if(!Img1HasBeenSet||!Img2HasBeenSet)
	{
		char msg[1024];
		sprintf(msg,"One of the inputs has not been set");
		DataFlowException e(FilterName(),msg);
		throw e;
	}

	if(myImg1->getWidth() != myImg2->getWidth())
	{
		char msg[1024];
		sprintf(msg,"Widths must match: %d %d",myImg1->getWidth(),myImg2->getWidth());
		DataFlowException e(FilterName(),msg);
		throw e;
	}
}

void
TBConcat::Execute()
{
	FilterInputIsValid();
	TopBottomConcatenate(*this->myImg1,*this->myImg2,this->mySourceImg1);

}	

void 
TBConcat::TopBottomConcatenate(const Image &topInput,const Image &bottomInput, Image &output)
{
	int in1_width = topInput.getWidth();
	int in1_height = topInput.getHeight();
	int in2_width = bottomInput.getWidth();
	int in2_height = bottomInput.getHeight();
	int out_height = in1_height + in2_height;	
	if(myImg1->getWidth() != myImg2->getWidth())
	{
		char msg[1024];
		sprintf(msg,"Heights must match: %d %d",myImg1->getHeight(),myImg2->getHeight());
		DataFlowException e(FilterName(),msg);
		throw e;
	}
	
	output.ResetSize(in1_width,out_height);
	for(int row = 0; row < in1_height; row++)
	{
		for(int col = 0; col < in1_width; col++)
		{
			int out_col = col;
			int out_row = row;
			output.setPixel(out_row, out_col, topInput.getPixel(row,col));
		}
	}
	for(int row = 0; row < in2_height; row ++)
	{
		for(int col = 0; col < in2_width; col++)
		{
			int out_col = col;
			int out_row = row + in1_height;;
			output.setPixel(out_row, out_col, bottomInput.getPixel(row,col));
		}
	} 
}

//--------------------------- Blender ---------------------------- 

Blender::Blender()
{
	mySourceImg1.setsource(this);
	hasTwo = true;
}

const char*
Blender::FilterName()
{
	return "Blender";
}

void
Blender::SetFactor(float f)
{
	this->factor = f;
}
 
void
Blender::FilterInputIsValid()
{
	if(this->factor < 0.0f || this->factor > 1.0f)
	{
		char msg[1024];
		sprintf(msg,"Invalid factor for blender: %f",this->factor);
		DataFlowException e(FilterName(),msg);
		throw e;		
	}
		

	if(!Img1HasBeenSet||!Img2HasBeenSet)
	{
		char msg[1024];
		sprintf(msg,"One of the inputs has not been set");
		DataFlowException e(FilterName(),msg);
		throw e;
	}
	if(myImg1->getWidth() != myImg2->getWidth() || myImg1->getHeight() != myImg2->getHeight())
	{
		char msg[1024];
		sprintf(msg,"Dimentions must match: %dX%d , %dX%d",myImg1->getWidth(),myImg1->getHeight(),myImg2->getWidth(),myImg2->getHeight());
		DataFlowException e(FilterName(),msg);
		throw e;
	}
}

void 
Blender::Execute()
{
	FilterInputIsValid();
	Blend(*this->myImg1,*this->myImg2,this->factor,this->mySourceImg1);

}

void 
Blender::Blend(const Image &input1,const Image &input2, float factor, Image &output)
{
	
	output.ResetSize(input1.getWidth(),input2.getHeight());
	int o_height = output.getHeight();
	int o_width = output.getWidth();
	
	for(int i = 0; i < o_height*o_width; i++)
	{
		output.getBuffer()[i].R = (factor * input1.getBuffer()[i].R)+((1-factor) * input2.getBuffer()[i].R);
		output.getBuffer()[i].G = (factor * input1.getBuffer()[i].G)+((1-factor) * input2.getBuffer()[i].G);
		output.getBuffer()[i].B = (factor * input1.getBuffer()[i].B)+((1-factor) * input2.getBuffer()[i].B);	
	}
}


//--------------------------- Mirror ---------------------------- 

Mirror::Mirror()
{
	mySourceImg1.setsource(this);
	hasTwo = false;
}

const char*
Mirror::FilterName()
{
	return "Mirror";
}

void
Mirror::FilterInputIsValid()
{
	
}

void
Mirror::Execute()
{
	FilterInputIsValid();
	flip(*this->myImg1,this->mySourceImg1);
}

void 
Mirror::flip(const Image &input1, Image &output)
{
	
	output.ResetSize(input1.getWidth(),input1.getHeight());
	int o_height = output.getHeight();
	int o_width = output.getWidth();
	
	for(int i = 0; i < o_height; i++)
	{
		for(int j = 0; j < o_width; j++)
		{
			output.getBuffer()[i*o_width + j] = input1.getBuffer()[i*o_width + (o_width - j-1)];
		}

	}
}
	


//--------------------------- Rotate ---------------------------- 

Rotate::Rotate()
{
	mySourceImg1.setsource(this);
	hasTwo = false;	
}

const char*
Rotate::FilterName()
{
	return "Rotate";
}

void
Rotate::FilterInputIsValid()
{

}

void 
Rotate::Execute()
{	
	FilterInputIsValid();
	clock90(*this->myImg1,this->mySourceImg1);
}

void
Rotate::clock90(const Image &input1, Image &output)
{

	output.ResetSize(input1.getHeight(),input1.getWidth());
	int o_height = output.getHeight();
	int o_width = output.getWidth();
	int i_height = input1.getHeight();
	int i_width = input1.getWidth();
	
	for(int i = 0; i < o_height; i++)
	{
		for(int j = 0; j < o_width; j++)
		{
			output.getBuffer()[i*o_width + j] = input1.getBuffer()[ (i_width)*(i_height-(j+1))+i];
		}
	}
}

//--------------------------- Subtract -------------------------- 


Subtract::Subtract()
{
	mySourceImg1.setsource(this);
	hasTwo = true;
}

const char*
Subtract::FilterName()
{
	return "Subtract";
}

void
Subtract::FilterInputIsValid()
{
	
	if(myImg1->getWidth() != myImg2->getWidth() || myImg1->getHeight() != myImg2->getHeight())
	{
		char msg[1024];
		sprintf(msg,"Dimentions must match: %dX%d , %dX%d",myImg1->getWidth(),myImg1->getHeight(),myImg2->getWidth(),myImg2->getHeight());
		DataFlowException e(FilterName(),msg);
		throw e;
	}
}

void
Subtract::Execute()
{
	FilterInputIsValid();
	minus(*this->myImg1,*this->myImg2,this->mySourceImg1);
}

void 
Subtract::minus(const Image &input1,const Image &input2, Image &output)
{
	output.ResetSize(input1.getWidth(), input2.getHeight());
	int o_width = output.getWidth();
	int o_height = 	output.getHeight();
	int total = o_width * o_height;

	for(int i = 0 ; i < total ; i++)
	{
		int r = input1.getBuffer()[i].R - input2.getBuffer()[i].R;
		if (r >= 0)
		{
			output.getBuffer()[i].R = r;
		}
		else
		{
			output.getBuffer()[i].R = 0;
		}
		int g = input1.getBuffer()[i].G - input2.getBuffer()[i].G;
		if (g >= 0)
		{
			output.getBuffer()[i].G = g;
		}
		else
		{
			output.getBuffer()[i].G = 0;
		}
		int b = input1.getBuffer()[i].B - input2.getBuffer()[i].B;
		if (b >= 0)
		{
			output.getBuffer()[i].B = b;
		}
		else
		{
			output.getBuffer()[i].B = 0;
		}
	}

}


//--------------------------- Grayscale ------------------------- 

Grayscale::Grayscale()
{
	mySourceImg1.setsource(this);
	hasTwo = false;
}

const char*
Grayscale::FilterName()
{
	return "Grayscale";
}

void
Grayscale::FilterInputIsValid()
{

}

void
Grayscale::Execute()
{
	FilterInputIsValid();
	blackandwhite(*this->myImg1,this->mySourceImg1);
}

void 
Grayscale::blackandwhite(const Image &input, Image &output)
{
	output.ResetSize(input.getWidth(), input.getHeight());
	int o_width = output.getWidth();
	int o_height = 	output.getHeight();
	int total = o_width * o_height;

	for(int i = 0 ; i < total ; i++)
	{
		output.getBuffer()[i].R = input.getBuffer()[i].R / 5 + input.getBuffer()[i].G / 2 + input.getBuffer()[i].B / 4;
		output.getBuffer()[i].G = input.getBuffer()[i].R / 5 + input.getBuffer()[i].G / 2 + input.getBuffer()[i].B / 4;
		output.getBuffer()[i].B = input.getBuffer()[i].R / 5 + input.getBuffer()[i].G / 2 + input.getBuffer()[i].B / 4;
	}
}
//--------------------------- Blur   ---------------------------- 

Blur::Blur()
{
	mySourceImg1.setsource(this);
	hasTwo = false;
}

const char*
Blur::FilterName()
{
	return "Blur";
}

void
Blur::FilterInputIsValid()
{
	if(!Img1HasBeenSet)
	{
		char msg[1024];
		sprintf(msg,"One of the inputs has not been set");
		DataFlowException e(FilterName(),msg);
		throw e;
	}
}

void 
Blur::Execute()
{
	FilterInputIsValid();
	blurrify(*this->myImg1,this->mySourceImg1);
}

void
Blur::blurrify(const Image &input,Image &output)
{
	output.ResetSize(input.getWidth(), input.getHeight());
	int o_width = output.getWidth();
	int o_height = 	output.getHeight();

	for(int i = 0 ; i < o_height ; i++)
	{
		for(int j = 0 ; j < o_width  ; j++)
		{
			if(i != 0 && j != 0 && i != o_height-1 && j != o_width - 1)
			{
				output.getBuffer()[i*o_width + j].R = ((input.getBuffer()[(i-1)*o_width + j - 1].R)/8) + ((input.getBuffer()[(i-1)*o_width + j].R)/8) + ((input.getBuffer()[(i-1)*o_width + j + 1].R)/8) + ((input.getBuffer()[(i)*o_width + j - 1].R)/8) + ((input.getBuffer()[(i)*o_width + j + 1].R)/8) + ((input.getBuffer()[(i+1)*o_width + j - 1].R)/8) + ((input.getBuffer()[(i+1)*o_width + j].R)/8) + ((input.getBuffer()[(i+1)*o_width + j + 1].R)/8);

				output.getBuffer()[i*o_width + j].G = ((input.getBuffer()[(i-1)*o_width + j - 1].G)/8) + ((input.getBuffer()[(i-1)*o_width + j].G)/8) + ((input.getBuffer()[(i-1)*o_width + j + 1].G)/8) + ((input.getBuffer()[(i)*o_width + j - 1].G)/8) + ((input.getBuffer()[(i)*o_width + j + 1].G)/8) + ((input.getBuffer()[(i+1)*o_width + j - 1].G)/8) + ((input.getBuffer()[(i+1)*o_width + j].G)/8) + ((input.getBuffer()[(i+1)*o_width + j + 1].G)/8);

				output.getBuffer()[i*o_width + j].B = ((input.getBuffer()[(i-1)*o_width + j - 1].B)/8) + ((input.getBuffer()[(i-1)*o_width + j].B)/8) + ((input.getBuffer()[(i-1)*o_width + j + 1].B)/8) + ((input.getBuffer()[(i)*o_width + j - 1].B)/8) + ((input.getBuffer()[(i)*o_width + j + 1].B)/8) + ((input.getBuffer()[(i+1)*o_width + j - 1].B)/8) + ((input.getBuffer()[(i+1)*o_width + j].B)/8) + ((input.getBuffer()[(i+1)*o_width + j + 1].B)/8);
			} 
			else
			{
				output.getBuffer()[i*o_width + j] = input.getBuffer()[i*o_width + j];
			}
		}
	}
	
}
//--------------------------- Concstant Color ------------------- 

Color::Color(int width, int height, int red, int green, int blue)
{
	mySourceImg1.setsource(this);
	mySourceImg1.ResetSize(width, height);
	myR = red;
	myG = green;
	myB = blue;	
}

const char*
Color::SourceName()
{
	return "Constant Color";
}

void
Color::SourceInputIsValid()
{
	if( myR < 0 || myG < 0 || myB < 0 || myR > 256 || myG > 256 || myB > 256)
	{
		char msg[1024];
		sprintf(msg,"Solid Color input is not valid");
		DataFlowException e(SourceName(),msg);
		throw e;
		
	}
}

void
Color::Update()
{
	SourceInputIsValid();	
	char msg[128];
	sprintf(msg,"%s: about to execute\n", SourceName() );
	Logger::LogEvent(msg);
	Execute();
	mySourceImg1.isUpdated();
	sprintf(msg,"%s: done executing\n", SourceName() );
	Logger::LogEvent(msg);
}

void 
Color::Execute()
{
	int width = mySourceImg1.getWidth();
	int height = mySourceImg1.getHeight();
	int total = width * height;
	for(int i = 0; i < total; i++)
	{
		mySourceImg1.getBuffer()[i].R = myR;
		mySourceImg1.getBuffer()[i].G = myG;
		mySourceImg1.getBuffer()[i].B = myB;
	}
}


//--------------------------- Checksum -------------------------- 

CheckSum::CheckSum()
{
	
}
const char*
CheckSum::SinkName()
{

	return "CheckSum";

}

void
CheckSum::SinkInputIsValid()
{
	
}

void
CheckSum::OutputCheckSum(const char* outputfile)
{
	
	int total = myImg1->getWidth() * myImg1->getHeight();
	int Red = 0;
	int Green = 0;
	int Blue = 0;
	for(int i = 0 ; i < total ; i++)
	{
		Red += myImg1->getBuffer()[i].R; 
		Green += myImg1->getBuffer()[i].G; 
		Blue += myImg1->getBuffer()[i].B; 
	}
	
	Red = Red % 256;
	Green = Green % 256;
	Blue = Blue % 256;
	
	FILE* f = fopen(outputfile,"w");		
	fprintf(f, "CHECKSUM: %d, %d, %d\n",Red,Green,Blue);
	fclose(f);
}









