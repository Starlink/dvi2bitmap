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
//    $Id: GIFBitmap.h,v 1.15 2003/02/17 16:11:29 norman Exp $


#ifndef GIFBITMAP_HEADER_READ
#define GIFBITMAP_HEADER_READ 1

#include "BitmapImage.h"
#ifdef HAVE_CSTD_INCLUDE
#include <cstdio>
#else
#include <stdio.h>
#endif

typedef int             code_int;
#ifdef SIGNED_COMPARE_SLOW
typedef unsigned long int count_int;
typedef unsigned short int count_short;
#else /*SIGNED_COMPARE_SLOW*/
typedef long int          count_int;
#endif /*SIGNED_COMPARE_SLOW*/

class GIFBitmap : public BitmapImage {
 public:
    //GIFBitmap (int w, int h, Byte *b, int bpp=1);
    GIFBitmap (const int w, const int h, const int bpp=1);
    ~GIFBitmap ();
    void setBitmap (const Byte *b);
    void setBitmapRow (const Byte *b);
    //void setTransparent (const bool sw) { transparent_ = sw; }
    void write (const string filename);
    string fileExtension() const { return "gif"; }

 private:
    //const Byte *bitmap_;
    //Byte *allocBitmap_;
    //const int w_, h_;
    //const int bpp_;
    //int bitmapRows_;
    //bool transparent_;
    //bool myBitmap_;
    void GIFEncode(FILE* fp,
		  int GWidth, int GHeight,
		  int GInterlace,
		  int Background,
		  int Transparent,
		  int BitsPerPixel,
		  int *Red, int *Green, int *Blue);
    void BumpPixel(void);
    int GIFNextPixel(void);
    void Putword(int w, FILE* fp);
    void compress(int init_bits, FILE* outfile);
    void output(code_int code);
    void cl_block (void);
    void cl_hash(count_int hsize);
    void char_init(void);
    void char_out( int c );
    void flush_char();

    // remainder are the static globals which were in
    // the original GIFENCOD routine
    int Width, Height;
    int curx, cury;
    long CountDown;
    int Pass;
    int Interlace;

#if 0
    const int BITS = 12;
    const int HSIZE = 5003;	/* 80% occupancy */
#endif
#define BITS 12
#define HSIZE 5003
 
    int n_bits;			/* number of bits/code */
    int maxbits;                /* user settable max # bits/code */
    code_int maxcode;		/* maximum code, given n_bits */
    code_int maxmaxcode;	/* should NEVER generate this code */ 

    count_int htab [HSIZE];
    unsigned short codetab [HSIZE];    
    code_int hsize;		/* for dynamic table sizing */

    code_int free_ent;		/* first unused entry */

    int clear_flg;

    int offset;
    long int in_count;		/* length of input */
    long int out_count;		/* # of codes output (for debugging) */

    int g_init_bits;
    FILE* g_outfile;

    int ClearCode;
    int EOFCode;

    unsigned long cur_accum;
    int cur_bits;

    int a_count;
    char accum[ 256 ];

    static const unsigned long masks[];
};
#endif // #ifndef GIFBITMAP_HEADER_READ
