
#include <PNMreader.h>
#include <PNMwriter.h>
#include <filters.h>
#include <logging.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using std::cerr;
using std::endl;
using std::ofstream;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <username>" << endl;
        exit(EXIT_FAILURE);
    }

    char event[1024];
    sprintf(event, "Entered program from %s\n", argv[1]);
    Logger::LogEvent(event);




    PNMreader reader1("./myImages/Chalk1.pnm");
    PNMreader reader2("./myImages/Chalk2.pnm");
    PNMreader reader3("./myImages/Chalk3.pnm");
    PNMreader reader4("./myImages/Chalk4.pnm");
	
	TBConcat tb1;
	TBConcat tb2;
	LRConcat lr1;
	LRConcat lr2;
	LRConcat lr3;
	Blender bl1;
	Blender bl2;
	Mirror mr1;
	Mirror mr2;
	Mirror mr3;
	Rotate r1;
	Rotate r2;
	Rotate r3;
	Rotate r4;
	Rotate r5;
	Shrinker sr;
	Grayscale g1;	
	/*
	Color c1(2160,1440,0,255,255);

	r1.SetInput(reader2.GetOutput());
	mr1.SetInput(r1.GetOutput());
	
	lr1.SetInput(mr1.GetOutput());
	lr1.SetInput2(r1.GetOutput());
	
	r2.SetInput(lr1.GetOutput());
	r3.SetInput(r2.GetOutput());
	
	bl1.SetFactor(.5);
	
	bl1.SetInput(r3.GetOutput());
	bl1.SetInput2(lr1.GetOutput());

	bl2.SetFactor(.8);
	bl2.SetInput(bl1.GetOutput());
	bl2.SetInput2(c1.GetOutput());
	
	r4.SetInput(bl2.GetOutput());	
*/
	
	
	bl1.SetFactor(.3);

	r1.SetInput(reader1.GetOutput());
	mr1.SetInput(r1.GetOutput());
	
	lr1.SetInput(r1.GetOutput());
	lr1.SetInput2(mr1.GetOutput());

	r2.SetInput(lr1.GetOutput());
	mr2.SetInput(r2.GetOutput());
	
	lr2.SetInput(r2.GetOutput());
	lr2.SetInput2(mr2.GetOutput());
	
	sr.SetInput(lr2.GetOutput());
	
	bl1.SetInput(sr.GetOutput());
	bl1.SetInput2(reader4.GetOutput());

	r3.SetInput(bl1.GetOutput());
	mr3.SetInput(r3.GetOutput());
	
	lr3.SetInput(r3.GetOutput());
	lr3.SetInput2(mr3.GetOutput());
			
	//g1.SetInput(bl1.GetOutput());
			
    Image *finalImage = lr3.GetOutput();

    try
    {
        finalImage->Update();
    }
    catch (DataFlowException &)
    {
        ofstream ofile("my_exception");
        if (ofile.fail())
        {
             cerr << "Something is wrong ... can't open my_exception"
                  << " for opening" << endl;
             exit(EXIT_FAILURE);
        }
        ofile << "Exception found!" << endl;
        exit(EXIT_SUCCESS);
    }

    CheckSum cs;
    cs.SetInput(finalImage);
    cs.OutputCheckSum("my_checksum");

    if (argc == 3)
    {
        PNMwriter writer;
        writer.SetInput(finalImage);
        writer.Write("3H.pnm");
    }
    Logger::Finalize();
}
