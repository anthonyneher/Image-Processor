//
// Created by Anthony Neher on 8/2/18.
//

#include "Image.h"

void Image::loadImage(string imagename) {
    ifstream file(imagename.c_str(), ios_base::binary|ios_base::in);

    if(file.is_open()){
        file.read(&imageheader.idLength,1);
        file.read(&imageheader.colourMapType,1);
        file.read(&imageheader.dataTypeCode,1);
        file.read((char *)&imageheader.colourMapOrigin,2);
        file.read((char *)&imageheader.colourMapLength,2);
        file.read(&imageheader.colourMapDepth,1);
        file.read((char *)&imageheader.xOrigin,2);
        file.read((char *)&imageheader.yOrigin,2);
        file.read((char *)&imageheader.width,2);
        file.read((char *)&imageheader.height,2);
        file.read(&imageheader.bitsPerPixel,1);
        file.read(&imageheader.imageDescriptor,1);
        numpixels = (int)imageheader.width*(int)imageheader.height;
        for(int i=0;i<numpixels;i++){
            Pixel p;
            file.read((char *)&p.Blue, 1);
            file.read((char *)&p.Green, 1);
            file.read((char *)&p.Red, 1);
            picture.push_back(p);
        }
    }
    else{
        cout<<"Unable to load \""<<string(imagename)<<"\"";
    }

    file.close();
}

void Image::writeFile(string filename) {
    ofstream file(filename.c_str(),ios_base::binary|ios_base::out);

    file.write(&imageheader.idLength,1);
    file.write(&imageheader.colourMapType,1);
    file.write(&imageheader.dataTypeCode,1);
    file.write((char *)&imageheader.colourMapOrigin,2);
    file.write((char *)&imageheader.colourMapLength,2);
    file.write(&imageheader.colourMapDepth,1);
    file.write((char *)&imageheader.xOrigin,2);
    file.write((char *)&imageheader.yOrigin,2);
    file.write((char *)&imageheader.width,2);
    file.write((char *)&imageheader.height,2);
    file.write(&imageheader.bitsPerPixel,1);
    file.write(&imageheader.imageDescriptor,1);

    for(int i=0;i<numpixels;i++){
        file.write((char *)&picture[i].Blue,1);
        file.write((char *)&picture[i].Green,1);
        file.write((char *)&picture[i].Red,1);
    }
    file.close();//cmake-build-debug/input/car.tga
}
void Image::Multiply(Image &A, Image &B) {
    auto mult = [](Pixel &A,Pixel &B){
        Pixel output;
        output.Green = (unsigned char)(((float)A.Green/255)*((float)B.Green/255)*255+0.5f);
        output.Blue = (unsigned char)(((float)A.Blue/255)*((float)B.Blue/255)*255+0.5f);
        output.Red = (unsigned char)(((float)A.Red/255)*((float)B.Red/255)*255+0.5f);
        return output;
    };
    imageheader = A.imageheader;
    numpixels = A.numpixels;
    for(int i=0;i<numpixels;i++){
        Pixel p = mult(A.picture[i],B.picture[i]);
        picture.push_back(p);
    }
}

void Image::Subtract(Image &A, Image &B) {
    auto diff = [](unsigned char a,unsigned char b){
        int value = (int)a - (int)b;
        if(value<0) value = 0;
        if(value>255) value = 255;
        return (unsigned char)value;
    };
    imageheader = A.imageheader;
    numpixels = A.numpixels;
    for(int i=0;i<numpixels;i++){
        Pixel output;
        output.Blue = diff(A.picture[i].Blue,B.picture[i].Blue);
        output.Red = diff(A.picture[i].Red,B.picture[i].Red);
        output.Green = diff(A.picture[i].Green,B.picture[i].Green);
        picture.push_back(output);
    }
}

void Image::Screen(Image &A, Image &B) {
    auto screen = [](unsigned char a,unsigned char b){
        float value = (1-(1-(float)a/255)*(1-(float)b/255))*255+0.5f;
        return (unsigned char)(value);
    };
    imageheader = A.imageheader;
    numpixels = A.numpixels;
    for(int i=0;i<numpixels;i++){
        Pixel p;
        p.Blue = screen(A.picture[i].Blue,B.picture[i].Blue);
        p.Red = screen(A.picture[i].Red,B.picture[i].Red);
        p.Green = screen(A.picture[i].Green,B.picture[i].Green);
        picture.push_back(p);
    }
}
void Image::Overlay(Image &A, Image &B) {
    auto overlay = [](unsigned char a,unsigned char b){
        float B=(float)b/255;
        float A=(float)a/255;
        float value;
        if(B>0.5){
            value = (1-(1-(float)a/255)*(1-B)*2)*255+0.5f;
        }
        else{
            value = (2*A*B)*255+0.5f;
        }
        return (unsigned char)(value);
    };
    imageheader = A.imageheader;
    numpixels = A.numpixels;
    for(int i=0;i<numpixels;i++){
        Pixel p;
        p.Blue = overlay(A.picture[i].Blue,B.picture[i].Blue);
        p.Green = overlay(A.picture[i].Green,B.picture[i].Green);
        p.Red = overlay(A.picture[i].Red, B.picture[i].Red);
        picture.push_back(p);
    }
}
void Image::ChannelAdd(int num, string color) {
    auto addto = [](int number, unsigned char color){
        int output = number + (int)color;
        if(output>255) output = 255;
        if (output<0) output = 0;
        return (unsigned char)output;
    };
    if(color == "red"){
        for(int i=0;i<numpixels;i++){
            picture[i].Red = addto(num,picture[i].Red);
        }
    }
    else if(color == "green"){
        for(int i=0;i<numpixels;i++){
            picture[i].Green = addto(num,picture[i].Green);
        }
    }
    else if(color == "blue"){
        for(int i=0;i<numpixels;i++){
            picture[i].Blue = addto(num,picture[i].Blue);
        }
    }
}
void Image::ChannelScale(float scale, string color) {
    auto scaleby = [](float number, unsigned char color){
        float output = number *(int)color+0.5f;
        if(output>255) output = 255;
        if (output<0) output = 0;
        return (unsigned char)output;
    };
    if(color == "red"){
        for(int i=0;i<numpixels;i++){
            picture[i].Red = scaleby(scale,picture[i].Red);
        }
    }
    else if(color == "green"){
        for(int i=0;i<numpixels;i++){
            picture[i].Green = scaleby(scale,picture[i].Green);
        }
    }
    else if(color == "blue"){
        for(int i=0;i<numpixels;i++){
            picture[i].Blue = scaleby(scale,picture[i].Blue);
        }
    }
}
void Image::SingleChannel(string color) {
    if(color == "red"){
        for(int i=0;i<numpixels;i++){
            picture[i].Green = picture[i].Red;
            picture[i].Blue = picture[i].Red;
        }
    }
    else if(color == "green"){
        for(int i=0;i<numpixels;i++){
            picture[i].Red = picture[i].Green;
            picture[i].Blue = picture[i].Green;
        }
    }
    else if(color == "blue"){
        for(int i=0;i<numpixels;i++){
            picture[i].Green = picture[i].Blue;
            picture[i].Red = picture[i].Blue;
        }
    }
}
void Image::AddChannel(Image &image,string color) {
    imageheader=image.imageheader;
    numpixels=image.numpixels;
    if(color == "red"){
        for(int i=0;i<numpixels;i++){
            picture[i].Red = image.picture[i].Red;
        }
    }
    else if(color == "green"){
        for(int i=0;i<numpixels;i++){
            picture[i].Green = image.picture[i].Green;
        }
    }
    else if(color == "blue"){
        for(int i=0;i<numpixels;i++){
            picture[i].Blue = image.picture[i].Blue;
        }
    }
}

Image& Image::operator=(const Image &image) {//assignment operator
    imageheader=image.imageheader;
    numpixels= image.numpixels;
    picture = image.picture;
    return *this;
}
void Image::Rotate() {
    reverse(picture.begin(),picture.end());
}

void Image::Combine(Image &tl, Image &tr, Image &bl, Image &br) {
    imageheader=tl.imageheader;
    imageheader.width = (short)2*imageheader.width;
    imageheader.height = (short)2*imageheader.height;
    numpixels= imageheader.height*imageheader.width;
    int tleft=0;
    int tright=0;
    int bleft=0;
    int bright=0;
    for (int i=0;i<numpixels;i++){
        if(i<numpixels/2){
            if(i%imageheader.width <(imageheader.width/2)){
                picture.push_back(bl.picture[bleft]);
                bleft++;
            }
            else{
                picture.push_back(br.picture[bright]);
                bright++;
            }
        }
        else{
            if(i%imageheader.width <(imageheader.width/2)){
                picture.push_back(tl.picture[tleft]);
                tleft++;
            }
            else{
                picture.push_back(tr.picture[tright]);
                tright++;
            }
        }
    }
}