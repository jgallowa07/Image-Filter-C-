#include <source.h>
#include <iostream>

#ifndef PNMREADER_H
#define PNMREADER_H

class PNMreader : public source
{
	private:
		char* filename;
	
	public:
		virtual void Execute();
		virtual void Update();
		PNMreader(char*);
		~PNMreader();
		virtual const char* SourceName();
		virtual void SourceInputIsValid();
		
};





#endif
