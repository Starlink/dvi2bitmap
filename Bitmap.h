// $Id$

#include "dvi2bitmap.h"

class Bitmap {
 public:
    Bitmap (int,int);
    ~Bitmap();
    void paint (int x, int y, int w, int h, Byte* b);
    void rule (int x, int y, int w, int h);
    enum imageFormats { gif, debugbitmap };
    void write (string filename, imageFormats format=gif);
    void crop ();
    static debug (bool sw) { debug_ = sw; }

    // pointer to bitmap.  Pixel (x,y) is at B[y*W + x];
    Byte *B;
 private:
    // width and height of bitmap
    const int W, H;
    // bounding box - 
    // bbL and bbT are the leftmost and topmost blackened pixels,
    // bbR and bbB are one more than the rightmost and lowest blackened pixels
    int bbL, bbR, bbT, bbB;
    // cropX is the value of bbX when the crop() method was called
    int cropL, cropR, cropT, cropB;
    bool cropped_;
    static bool debug_;

    void write_debugbitmap (string filename);
    void write_gif (string filename);
};

class BitmapError : public DviError {
 public:
    BitmapError(string s) : DviError(s) { };
};
