

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#ifndef IMAGE_H
#define IMAGE_H

using namespace::std;
class source;
class Filter;
struct Pixel
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	 
};
class Image
{
	private:
		int width;
		int height;
		Pixel* buffer;
		bool isUTD;
		source* mysource;
		mutable std::vector<Filter*>  mysocketVector;
	public:
		Image(void);
		Image(int w, int h);
		Image(Image &);
		~Image();		
	
		void ResetSize(int width, int height);
		bool getStatus() const;
		int getWidth() const;
		int getHeight() const;
		Pixel* getBuffer() const;
		void isUpdated ();
		void Update() const;
		void setsource(source*);
		void setsocketVector(vector<Filter*>&) const;
		vector<Filter*>& getsocketVector() const;
		Pixel& getPixel(int r, int c) const;
		void setPixel(int r, int c, const Pixel &p);
};

#endif 

