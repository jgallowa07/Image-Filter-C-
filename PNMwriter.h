#include <sink.h>

#ifndef PNMWRITER_H
#define PNMWRITER_H 

class PNMwriter : public sink
{

	public:
		void Write(char*);
		void WriteImage(char *filename, const Image &img);
		virtual const char* SinkName();
		virtual void SinkInputIsValid();
};

#endif
