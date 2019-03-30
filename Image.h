//
// Created by Anthony Neher on 8/2/18.
//

#ifndef IMAGEPROCESSOR_IMAGE_H
#define IMAGEPROCESSOR_IMAGE_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
using namespace std;

class Image {
public:
    struct Header
    {
        char idLength;
        char colourMapType;
        char dataTypeCode;
        short colourMapOrigin;
        short colourMapLength;
        char colourMapDepth;
        short xOrigin;
        short yOrigin;
        short width;
        short height;
        char bitsPerPixel;
        char imageDescriptor;
    } imageheader;
    struct Pixel
    {
        unsigned char Blue;
        unsigned char Green;
        unsigned char Red;
    };
    int numpixels;
    vector<Pixel> picture;

    void loadImage(string imagename);
    void writeFile(string filename);
    void Multiply(Image & A,Image &B);
    void Subtract(Image &A, Image &B);
    void Screen(Image &A, Image &B);
    void Overlay(Image &A, Image &B);
    void ChannelAdd(int num,string color);
    void ChannelScale(float scale, string color);
    void SingleChannel(string color);
    void AddChannel(Image &image,string color);
    void Rotate();
    void Combine(Image &tl,Image &tr,Image &bl,Image &br);
    Image& operator= (const Image& image);
};


#endif //IMAGEPROCESSOR_IMAGE_H
