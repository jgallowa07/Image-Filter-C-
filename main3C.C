#include <PNMreader.h>
#include <PNMwriter.h>
#include <filters.h>

int main(int argc, char *argv[])
{
    PNMreader reader(argv[1]);
    PNMwriter writer;
    Shrinker shrinker1;
    Shrinker shrinker2;
    LRConcat lrconcat1;
    LRConcat lrconcat2;
    LRConcat lrconcat3;
    TBConcat tbconcat1;
    TBConcat tbconcat2;
    Blender  blender;
    Mirror mirror;
    Rotate rotate;
    Subtract subtract;
    Grayscale gray;
    Blur blur;

    blender.SetFactor(0.8);

    shrinker1.SetInput(reader.GetOutput());

    lrconcat1.SetInput(shrinker1.GetOutput());
    lrconcat1.SetInput2(shrinker1.GetOutput());

    tbconcat1.SetInput(lrconcat1.GetOutput());
    tbconcat1.SetInput2(lrconcat1.GetOutput());

    shrinker2.SetInput(tbconcat1.GetOutput());

    lrconcat2.SetInput(shrinker2.GetOutput());
    lrconcat2.SetInput2(shrinker1.GetOutput());

    tbconcat2.SetInput(lrconcat2.GetOutput());
    tbconcat2.SetInput2(lrconcat1.GetOutput());

    blender.SetInput(tbconcat2.GetOutput());
    blender.SetInput2(reader.GetOutput());

    mirror.SetInput(blender.GetOutput());
    gray.SetInput(mirror.GetOutput());
    //subtract.SetInput(blender.GetOutput());
    //subtract.SetInput2(mirror.GetOutput());
    blur.SetInput(gray.GetOutput());

    lrconcat3.SetInput(gray.GetOutput());
    lrconcat3.SetInput2(blur.GetOutput());
    //rotate.SetInput(blur.GetOutput());
    lrconcat3.GetOutput()->Update();

    //subtract.GetOutput()->Update();


    writer.SetInput(lrconcat3.GetOutput());

    // reader.Execute();
    // shrinker1.Execute();
    // lrconcat1.Execute();
    // tbconcat1.Execute();
    // shrinker2.Execute();
    // lrconcat2.Execute();
    // tbconcat2.Execute();
    // blender.Execute();
    // mirror.Execute();
    //rotate.Execute();
    // subtract.Execute();
    writer.Write(argv[2]);
}
