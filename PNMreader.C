#include <PNMreader.h>
#include <iostream>

void
PNMreader::Update()
{
	char msg[128];
	sprintf(msg,"%s: about to execute\n", SourceName() );
	Logger::LogEvent(msg);
	Execute();
	mySourceImg1.isUpdated();
	sprintf(msg,"%s: done executing\n", SourceName() );
	Logger::LogEvent(msg);
}

PNMreader::PNMreader(char* input)
{
	mySourceImg1.setsource(this);
	int numChar = strlen(input);
	filename = new char[numChar+1];
	strcpy(filename,input);	
}


void
PNMreader::Execute()
{
	FILE* f_in = fopen(this->filename,"r");
	if(f_in == NULL)
	{
		char msg[1024];
		sprintf(msg,"Given Filename is not a valid file to read from");
		DataFlowException e(SourceName(),msg);
		throw e;
	}
	char magicNum[128];
 	int width, height, maxval;
 	fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);  
	//myImg1.SetWidth(width);
	//myImg1.SetHeight(height); 
	mySourceImg1.ResetSize(width,height);
	fread(mySourceImg1.getBuffer(), sizeof(Pixel), width * height, f_in);
	fclose(f_in);
//	mySourceImg1.isUpdated();
	
}
const char* 
PNMreader::SourceName()
{
	return "PNMreader";
}
PNMreader::~PNMreader()
{
	delete [] filename;
}
void
PNMreader::SourceInputIsValid()
{
}

