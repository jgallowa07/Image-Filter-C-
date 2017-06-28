#include <PNMwriter.h>

void 
PNMwriter::Write(char* filename)
{
	WriteImage(filename,*this->myImg1); 	
}

void
PNMwriter::WriteImage(char *filename, const Image &img)
{
	FILE* f_out = fopen(filename, "w");

	int w = img.getWidth();
	int h = img.getHeight();	

	fprintf(f_out,"%s\n","P6");
	fprintf(f_out,"%d %d\n",w,h);
	fprintf(f_out,"%d\n",255);
	
	fwrite(img.getBuffer(), sizeof(Pixel),w*h,f_out);
	fclose(f_out);
	
}
const char*
PNMwriter::SinkName()
{
	return "PNMwriter";
}
void
PNMwriter::SinkInputIsValid()
{
	if(!Img1HasBeenSet)
	{	
		char msg[1024];
		sprintf(msg,"One of the Images has not been set");
		DataFlowException e(SinkName(),msg);
		throw e;
		
	}
}
