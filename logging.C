
#include <logging.h>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
FILE* Logger::logger = NULL;
void 
Logger::LogEvent(const char* event)
{

		
	if(logger == NULL)
	{	
		logger = fopen("logger","w");
	}
	fprintf(logger,"%s",event);

}

void
Logger::Finalize()
{

	fclose(logger);

}

DataFlowException::DataFlowException(const char *type, const char *error)
{

	//strcpy(this->msg,"(",type,"): ",error);
	sprintf(this->msg,"Throwing Exception (%s): %s\n",type,error);
	Logger::LogEvent(msg);
	

}
