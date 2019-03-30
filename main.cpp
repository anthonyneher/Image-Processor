#include "Image.h"
#include <chrono>
using namespace std::chrono;
void QuickCompare();
bool check(string file1,string file2);
bool compare(string file1,string file2);
int main() {

    Image layer1;
    layer1.loadImage("input/layer1.tga");
    Image layer2;
    layer2.loadImage("input/layer2.tga");
    Image layer_blue;
    layer_blue.loadImage("input/layer_blue.tga");
    Image layer_green;
    layer_green.loadImage("input/layer_green.tga");
    Image layer_red;
    layer_red.loadImage("input/layer_red.tga");
    Image pattern1;
    pattern1.loadImage("input/pattern1.tga");
    Image pattern2;
    pattern2.loadImage("input/pattern2.tga");
    Image text;
    text.loadImage("input/text.tga");
    Image text2;
    text2.loadImage("input/text2.tga");
    Image car;
    car.loadImage("input/car.tga");
    Image circles;
    circles.loadImage("input/circles.tga");
    //part 1
    Image part1;
    part1.Multiply(layer1,pattern1);
    part1.writeFile("output/part1.tga");
    //part 2
    Image part2;
    part2.Subtract(car,layer2);
    part2.writeFile("output/part2.tga");
    //part 3
    Image temp;
    temp.Multiply(layer1,pattern2);
    Image part3;
    part3.Screen(text,temp);
    part3.writeFile("output/part3.tga");
    //part 4
    Image temp2;
    temp2.Multiply(layer2,circles);
    Image part4;
    part4.Subtract(temp2,pattern2);
    part4.writeFile("output/part4.tga");
    //part 5
    Image part5;
    part5.Overlay(layer1,pattern1);
    part5.writeFile("output/part5.tga");
    //part 6
    Image part6;
    part6 = car;
    part6.ChannelAdd(200, "green");
    part6.writeFile("output/part6.tga");
    //part 7
    Image part7;
    part7 = car;
    part7.ChannelScale(4,"red");
    part7.ChannelScale(0,"blue");
    part7.writeFile("output/part7.tga");
    //part 8
    Image part8_r;
    part8_r=car;
    part8_r.SingleChannel("red");
    part8_r.writeFile("output/part8_r.tga");
    Image part8_g;
    part8_g=car;
    part8_g.SingleChannel("green");
    part8_g.writeFile("output/part8_g.tga");
    Image part8_b;
    part8_b=car;
    part8_b.SingleChannel("blue");
    part8_b.writeFile("output/part8_b.tga");
    //part 9
    Image part9;
    part9=layer_blue;
    part9.AddChannel(layer_green,"green");
    part9.AddChannel(layer_red,"red");
    part9.writeFile("output/part9.tga");
    //part10
    Image part10;
    part10 = text2;
    part10.Rotate();
    part10.writeFile("output/part10.tga");


    QuickCompare();
    Image extra;
    extra.Combine(car,circles,text,pattern1);
    extra.writeFile("output/extracredit.tga");


    return 0;
}
bool check(string output,string example){
    size_t buffer = 10000;
    ifstream ofile(output.c_str(), ifstream::in | ifstream::binary);
    ifstream efile(example.c_str(), ifstream::in | ifstream::binary);

    if(!ofile.is_open() || !efile.is_open())return false;

    auto *outputbuf = new char[buffer]();
    auto *examplebuf = new char[buffer]();

    do {
        ofile.read(outputbuf, buffer);
        efile.read(examplebuf, buffer);

        if (std::memcmp(outputbuf, examplebuf, buffer) != 0)
        {
            delete[] examplebuf;
            delete[] outputbuf;
            return false;
        }
    } while (ofile.good() || efile.good());

    delete[] examplebuf;
    delete[] outputbuf;
    return true;
}
bool compare(string file1, string file2) {
    Image compare;
    compare.loadImage(file1);
    Image file;
    file.loadImage(file2);
    if(compare.imageheader.height!=file.imageheader.height) return false;
    if(compare.imageheader.width!=file.imageheader.width) return false;
    if(compare.imageheader.imageDescriptor!=file.imageheader.imageDescriptor) return false;
    if(compare.imageheader.bitsPerPixel!=file.imageheader.bitsPerPixel) return false;
    if(compare.imageheader.yOrigin!=file.imageheader.yOrigin)return false;
    if(compare.imageheader.xOrigin!=file.imageheader.xOrigin)return false;
    if(compare.imageheader.colourMapDepth!=file.imageheader.colourMapDepth)return false;
    if(compare.imageheader.colourMapLength!=file.imageheader.colourMapLength)return false;
    if(compare.imageheader.colourMapOrigin!=file.imageheader.colourMapOrigin)return false;
    if(compare.imageheader.dataTypeCode!=file.imageheader.dataTypeCode)return false;
    if(compare.imageheader.colourMapType!=file.imageheader.colourMapType)return false;
    if(compare.imageheader.idLength!=file.imageheader.idLength)return false;
    if(compare.picture.size()!=file.picture.size()) return false;
    for(int i=0;i<file.numpixels;i++){
        if(file.picture[i].Green!=compare.picture[i].Green) return false;
        if(file.picture[i].Red!=compare.picture[i].Red)return false;
        if(file.picture[i].Blue!=compare.picture[i].Blue)return false;
    }
    return true;
}
void QuickCompare(){
    auto start = high_resolution_clock::now();
    int points = 0;
    for (int i=1;i<11;i++){
        string file1 = "output/part" + to_string(i) + ".tga";
        string file2 = "examples/EXAMPLE_part" + to_string(i) + ".tga";
        if(i==8){
            file1 = "output/part8_b.tga";
            file2 = "examples/EXAMPLE_part8_b.tga";
            bool blue = check(file1,file2);
            file1 = "output/part8_g.tga";
            file2 = "examples/EXAMPLE_part8_g.tga";
            bool green = check(file1,file2);
            file1 = "output/part8_r.tga";
            file2 = "examples/EXAMPLE_part8_r.tga";
            bool red = check(file1,file2);
            if(red && green && blue){
                cout<<"Part "<<to_string(i)<<" successful! +20 points"<<endl;
                points +=20;
            }
            else{
                cout<<"Part "<<to_string(i)<<" unsuccessful"<<endl;
            }
        }
        else{
            if(check(file1,file2)){
                cout<<"Part "<<to_string(i)<<" successful! +20 points"<<endl;
                points +=20;
            }
            else{
                cout<<"Part "<<to_string(i)<<" unsuccessful"<<endl;
            }
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout<<"Total score: "<<points<<endl;
    cout<<"Quick binary load and compare of data: "<<(float)duration.count()/1000000<<" seconds.\n";
    start = high_resolution_clock::now();
    points = 0;
    for (int i=1;i<11;i++){
        string file1 = "output/part" + to_string(i) + ".tga";
        string file2 = "examples/EXAMPLE_part" + to_string(i) + ".tga";
        if(i==8){
            file1 = "output/part8_b.tga";
            file2 = "examples/EXAMPLE_part8_b.tga";
            bool blue = compare(file1,file2);
            file1 = "output/part8_g.tga";
            file2 = "examples/EXAMPLE_part8_g.tga";
            bool green = compare(file1,file2);
            file1 = "output/part8_r.tga";
            file2 = "examples/EXAMPLE_part8_r.tga";
            bool red = compare(file1,file2);
            if(red && green && blue){
                cout<<"Part "<<to_string(i)<<" successful! +20 points"<<endl;
                points +=20;
            }
            else{
                cout<<"Part "<<to_string(i)<<" unsuccessful"<<endl;
            }
        }
        else{
            if(compare(file1,file2)){
                cout<<"Part "<<to_string(i)<<" successful! +20 points"<<endl;
                points +=20;
            }
            else{
                cout<<"Part "<<to_string(i)<<" unsuccessful"<<endl;
            }
        }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout<<"Total score: "<<points<<endl;
    cout<<"Loading files manually and comparing: "<<(float)duration.count()/1000000<<" seconds.";
}