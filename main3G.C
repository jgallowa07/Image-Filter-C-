#include <PNMreader.h>
#include <PNMwriter.h>
#include <filters.h>

int main(int argc, char *argv[])
{
    //PNMreader reader(argv[1]);

	Blender bl;

    Color red(250, 250, 255, 0, 0);
    Color blue(250, 250, 0, 0, 255);

	bl.setInput(red.getOutput());
	bl.setInput2(blue.getOutput());

    bl.GetOutput()->Update();

    PNMwriter writer;
    writer.SetInput(bl.GetOutput());
    writer.Write(argv[1]);

    CheckSum cs;
    cs.SetInput(tb3.GetOutput());
    cs.OutputCheckSum("proj3G_checksum");
}
