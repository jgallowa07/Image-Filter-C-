#include <image.h>
#include <source.h>

Image::Image(void)
{
	width = 0;
	height = 0;
	isUTD = false;
	buffer = NULL;
}
Image::Image(int w, int h)
{
	width = w;
	height = h;
	buffer = new Pixel[width*height];
	isUTD = false;

}
Image::Image(Image &r)
{
	width = r.getWidth();
	height = r.getHeight();
	buffer = new Pixel[width*height];
	//buffer = (Pixel*) malloc(sizeof(Pixel)*width*height);
	memcpy(buffer,r.buffer,sizeof(Pixel)*width*height);	
	isUTD = false;
}

Image::~Image()
{

	delete [] buffer; 

}

void	
Image::ResetSize(int width, int height)
{
	
	if(width < 0 || height < 0)
	{
		char msg[1024];
		char image[10];
		sprintf(msg,"Input 1 has been set as this output");
		sprintf(image,"Image");

		DataFlowException e(image,msg);
		throw e;
	}
	
	this->width = width;
	this->height = height;
	buffer = new Pixel[width*height];
	//buffer = (Pixel*) malloc(sizeof(Pixel)*width*height);
		
}
int
Image::getWidth() const
{
	return this->width;
}
int
Image::getHeight() const
{
	return this->height;
}
Pixel*
Image::getBuffer() const
{
	return this->buffer;
}
void
Image::isUpdated()
{
	this->isUTD = true;
}
void 
Image::setsource(source* S)
{

	mysource = S;

}
void 
Image::Update() const
{
	mysource->setFilterVector(getsocketVector());
	mysource->Update();
}

bool
Image::getStatus() const
{

	return isUTD;
}


void
Image::setsocketVector(vector<Filter*> &FV) const
{
		
	for(int i = 0; i < FV.size(); i++)
	{
		mysocketVector.push_back(FV[i]);
		
	}
	
	
}

vector<Filter*>&
Image::getsocketVector() const
{
	return mysocketVector;
}

Pixel&
Image::getPixel(int row, int col) const
{
	int index = row*this->width+col;
	
	return buffer[index];

}

void 
Image::setPixel(int row, int col, const Pixel& P)
{
	
	int index = row*this->width+col;
	
	buffer[index] = P;

}
	




