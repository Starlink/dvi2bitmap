//    This file is part of dvi2bitmap.
//    Copyright 1999--2002, Council for the Central Laboratory of the Research Councils
//    
//    This program is part of the Starlink Software Distribution: see
//    http://www.starlink.ac.uk 
//
//    dvi2bitmap is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    dvi2bitmap is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with dvi2bitmap; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    The General Public License is distributed along with this
//    program in the file LICENCE.
//
//    Author: Norman Gray <norman@astro.gla.ac.uk>
//    $Id: BitmapImage.h,v 1.16 2003/02/16 21:48:06 norman Exp $


#ifndef BITMAPIMAGE_HEADER_READ
#define BITMAPIMAGE_HEADER_READ 1

#include "Byte.h"
#include "verbosity.h"
#include "Bitmap.h"		// for BitmapColour

class BitmapImage {
 public:
    BitmapImage(const int w, const int h, const int bpp=1);
    virtual ~BitmapImage();
    void setBitmap (const Byte *B);
    void setBitmapRow (const Byte *B);
    //virtual void setColourTable (???) = 0;
    void setTransparent (const bool sw) { isTransparent_ = sw; };
    void setRGB (const bool fg, const Bitmap::BitmapColour* rgb) {
	if (fg)
	{
	    fg_.red=rgb->red; fg_.green=rgb->green; fg_.blue=rgb->blue;
	}
	else
	{
	    bg_.red=rgb->red; bg_.green=rgb->green; bg_.blue=rgb->blue;
	}
    };
    virtual void write (const string filename) = 0;
    virtual string fileExtension() const = 0;

    // Information about environment
    enum infoFields {SOFTWAREVERSION, INPUTFILENAME, FURTHERINFO};
    static void setInfo (const infoFields which, const string *s);

    static BitmapImage *newBitmapImage
	(const string format, const int w, const int h, const int bpp=1);
    static bool supportedBitmapImage (const string format);
    // Return default bitmap format
    static const char* firstBitmapImageFormat();
    // After a call to firstBitmapImageFormat, successive calls to
    // nextBitmapImageFormat return further allowable formats.
    static const char* nextBitmapImageFormat();
    static void verbosity (const verbosities level) { verbosity_ = level; }

 protected:
    int w_, h_;
    const int bpp_;
    const Byte *bitmap_;
    Byte *allocBitmap_;
    bool myBitmap_;
    int bitmapRows_;
    bool isTransparent_;
    //Byte fg_red_, fg_green_, fg_blue_, bg_red_, bg_green_, bg_blue_;
    Bitmap::BitmapColour fg_, bg_;

    static const string *softwareversion;
    static const string *inputfilename;
    static const string *furtherinfo;
    static const char* formats[];
    static const int nformats;
    static int iterator_index;
    static verbosities verbosity_;
};
#endif // #ifndef BITMAPIMAGE_HEADER_READ
