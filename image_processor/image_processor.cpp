#include "bmp.h"
#include "parses.h"
#include "filter_factory.h"

int main(int argc, char *argv[]) {

    parsing::Parser parser(argc, argv);
    auto image = BmpFile::LoadBMP(parser.InputFileGetter());
    FilterFactory application(parser.descriptor_);
    try {
        application.RunApplication(image);
        BmpFile::SaveBMP(image, parser.OutputFileGetter());
    } catch (std::exception) {
        throw MyException("warning!");
    }
}

/*Test 1
 int main(){
    auto image = BmpFile::LoadBMP("/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    Twirl twirl;
    twirl.Apply(image);
    BmpFile::SaveBMP(image, "/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/photo.bmp");
    return 0;
}
*/

/*
 Test 2
    int main(){
auto image = BmpFile::LoadBMP("/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    NegativeFilter nigative;
    nigative.Apply(image);
BmpFile::SaveBMP(image, "/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/photo.bmp");
    return 0;
}
 */

/*
   Test 3
    int main(){
auto image = BmpFile::LoadBMP("/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    GrayScale gray;
    gray.Apply(image);
BmpFile::SaveBMP(image, "/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/photo.bmp");
    return 0;
}
 */

/*
 Test 4
 int main(){
auto image = BmpFile::LoadBMP("/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    Sharp sharp;
    sharp.Apply(image);
BmpFile::SaveBMP(image, "/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/photo.bmp");
    return 0;
}
   */

/*
 Test 5
 int main(){
auto image = BmpFile::LoadBMP("/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    Edge edge{4};
    edge.Apply(image);
BmpFile::SaveBMP(image, "/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/photo.bmp");
    return 0;
}
 */

/*
 * Test 6
int main(){
auto image = BmpFile::LoadBMP("/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    MyFilter myfilter;
    myfilter.Apply(image);
BmpFile::SaveBMP(image, "/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/photo.bmp");
    return 0;
}
 */

/*
Test 7
int main(){
auto image = BmpFile::LoadBMP("/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    Gauss gauss;
    gauss.Apply(image);
BmpFile::SaveBMP(image, "/Users/timurkin/cpp-base-hse-2022/projects/image_processor/examples/photo.bmp");
    return 0;
}
*/
