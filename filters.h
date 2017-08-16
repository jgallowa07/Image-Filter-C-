

#include <iostream>
#include <Filter.h>
//#include <Filter.C>
#ifndef FILTERS_H
#define FILTERS_H

class Shrinker : public Filter
{
	public:
		Shrinker();
		virtual void Execute();	
		void HalveInSize(const Image &input, Image &output);
		virtual const char* FilterName();
		virtual void FilterInputIsValid();
};





class LRConcat : public Filter
{

	public:
		LRConcat();
		virtual void Execute();	
		void LeftRightConcatenate(const Image &leftInput,const Image &rightinput, Image &output);
		virtual const char* FilterName();
		virtual void FilterInputIsValid();
		
};


class TBConcat : public Filter
{
	public:
		TBConcat();
		virtual void Execute();	
		void TopBottomConcatenate(const Image &topInput,const Image &bottomInput, Image &output);
		virtual const char* FilterName();
		virtual void FilterInputIsValid();
};


class Blender : public Filter
{
	private:
		float factor;	
		
	public:
		Blender();
		virtual void Execute();
		void Blend(const Image &input1,const Image &input2, float factor, Image &output);
		void SetFactor(float);	
		virtual const char* FilterName();
		virtual void FilterInputIsValid();
};

class Mirror : public Filter 
{
	public: 
		Mirror();
		virtual void Execute();
		void flip(const Image &input1, Image &output);
		virtual const char* FilterName();
		virtual void FilterInputIsValid();	
};

class Rotate : public Filter
{
	public:
		Rotate();
		virtual void Execute();
		void clock90(const Image &input1, Image &output);
		virtual const char* FilterName();
		virtual void FilterInputIsValid();
};

class Subtract : public Filter
{
	public:
		Subtract();
		virtual void Execute();
		void minus(const Image &input1,const Image &input2, Image &output);
		virtual const char* FilterName();
		virtual void FilterInputIsValid();
};


class Grayscale : public Filter
{
	public:
		Grayscale();
		virtual void Execute();
		void blackandwhite(const Image &input1, Image &output);
		virtual const char* FilterName();
		virtual void FilterInputIsValid();	
};


class Blur : public Filter
{
	public:
		Blur();
		virtual void Execute();
		void blurrify(const Image& input1, Image &output);				
		virtual const char* FilterName();
		virtual void FilterInputIsValid();
};


class Color : public source
{
	int myR, myG, myB;
	public:
		Color(int width, int height, int R, int G, int B);		
		virtual void Update();
		virtual void Execute();
		virtual const char* SourceName();
		virtual void SourceInputIsValid();
		

};


class CheckSum : public sink
{
	public:
		CheckSum();
		void OutputCheckSum(const char* outputfile);
		virtual const char* SinkName();
		virtual void SinkInputIsValid();
};

#endif

