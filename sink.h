#ifndef SINK_H
#define SINK_H

#include <image.h>
#include <logging.h>
class sink
{
	protected:
		const Image* myImg1;
		const Image* myImg2;
		bool Img1HasBeenSet;
		bool Img2HasBeenSet;
		bool HasBeenSetTwice;
		
	public:
		sink();
		virtual const char* SinkName() = 0;
		virtual void SinkInputIsValid() = 0;
		const Image* GetImg1();
		const Image* GetImg2();
		void SetInput(Image*);
		void SetInput2(Image*);	

		
};
#endif
