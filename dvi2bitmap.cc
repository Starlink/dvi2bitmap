// Part of dvi2bitmap.
// Copyright 1999, Particle Physics and Astronomy Research Council.
// See file LICENCE for conditions.
//
// part of dvi2bitmap
// $Id$

#include "dvi2bitmap.h"
#include <vector>
#include <iostream>

#if NO_CSTD_INCLUDE
#include <stdio.h>		// for vsprintf
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#else
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cctype>
using std::exit;
#endif

#include "DviFile.h"
#include "PkFont.h"
#include "Bitmap.h"
#include "version.h"
#include "verbosity.h"

#ifdef ENABLE_KPATHSEA
#include "kpathsea.h"
#endif

typedef vector<string> string_list;

// bitmap_info keeps together all the detailed information about the
// bitmap to be written.
struct bitmap_info {
    bitmap_info()
	: blur_bitmap(false), make_transparent(true), bitmap_scale_factor(1),
	  ofile_pattern(""), ofile_type("") { }
    bool blur_bitmap;
    bool make_transparent;
    int bitmap_scale_factor;
    string ofile_pattern;
    string ofile_type;
};

void process_dvi_file (DviFile *, bitmap_info&, int resolution,
		       const PkFont *fallback_font);
string_list *tokenise_string (string s);
string get_ofn_pattern (string dviname);
void Usage (void);
char *progname;

verbosities verbosity = normal;
int bitmapH = -1;
int bitmapW = -1;

main (int argc, char **argv)
{
    string dviname;
    int resolution = 72;	// in pixels-per-inch
    double magmag = 1.0;	// magnification of file magnification factor
    int show_font_list = 0;
    bitmap_info bm;
    bool do_process_file = true; // if true, then process DVI file
    bool all_fonts_present = true;
    bool no_font_present = true;

    //DviFile::verbosity(2);
    //PkFont::verbosity(2);
    //PkRasterdata::verbosity(2);
    //Bitmap::verbosity(2);

    progname = argv[0];

#ifdef ENABLE_KPATHSEA
    kpathsea::init (progname, resolution);
#endif

    for (argc--, argv++; argc>0; argc--, argv++)
	if (**argv == '-')
	    switch (*++*argv)
	    {
	      case 'b':
		switch (*++*argv)
		{
		  case 'h':
		    argc--, argv++; if (argc <= 0) Usage();
		    bitmapH = atoi (*argv);
		    break;
		  case 'w':
		    argc--, argv++; if (argc <= 0) Usage();
		    bitmapW = atoi (*argv);
		    break;
		  default:
		    Usage();
		    break;
		}
		break;
	      case 'f':		// set PK font path
		argc--, argv++;
		if (argc <= 0)
		    Usage();
		PkFont::setFontPath(*argv);
		break;
	      case 'r':		// set resolution
		argc--, argv++;
		if (argc <= 0)
		    Usage();
		resolution = atoi (*argv);
		break;
	      case 'm':		// set magnification
		argc--, argv++;
		if (argc <= 0)
		    Usage();
		magmag = atof (*argv);
		break;
	      case 'g':		// debugging...
		{
		    verbosities debuglevel = debug;
		    for (++*argv; **argv != '\0'; ++*argv)
			switch (**argv)
			{
			  case 'd': // debug DVI file
			    DviFile::verbosity(debuglevel);
			    break;
			  case 'p': // debug PK file
			    PkFont::verbosity(debuglevel);
			    break;
			  case 'r': // debug rasterdata parsing
			    PkRasterdata::verbosity(debuglevel);
			    break;
			  case 'i': // debug input
			    InputByteStream::verbosity(debuglevel);
			    break;
			  case 'b': // debug bitmap
			    Bitmap::verbosity(debuglevel);
			    break;
			  case 'm': // debug main program
			    verbosity = debuglevel;
			    break;
			  case 'g':
			    if (debuglevel == debug)
				debuglevel = everything;
			    break;
			  default:
			    Usage();
			}
		}
		break;
	      case 'q':		// run quietly
		DviFile::verbosity(quiet);
		PkFont::verbosity(quiet);
		PkRasterdata::verbosity(quiet);
		InputByteStream::verbosity(quiet);
		Bitmap::verbosity(quiet);
		verbosity = quiet;
		break;
	      case 'Q':		// run silently - no warnings or errors
		DviFile::verbosity(silent);
		PkFont::verbosity(silent);
		PkRasterdata::verbosity(silent);
		InputByteStream::verbosity(silent);
		Bitmap::verbosity(silent);
		verbosity = silent;
		break;
	      case 'l':		// show missing fonts
		show_font_list = 1;
		break;
	      case 'L':		// show all fonts
		show_font_list = 2;
		break;
	      case 'n':		// don't actually process the DVI file
		switch (*++*argv)
		{
		  case '\0':
		    do_process_file = false; // ...just the pre/postamble
		    PkFont::setMakeFonts (false);
		    break;
		  case 'f':
		    PkFont::setMakeFonts (false);
		    break;
		  default:
		    Usage();
		    break;
		}
		break;
	      case 'P':		// process the bitmap...
		while (*++*argv != '\0')
		    switch (**argv)
		    {
		      case 'b':	// blur bitmap
			bm.blur_bitmap = !bm.blur_bitmap;
			break;
		      case 't':	// make bitmap transparent
			bm.make_transparent = !bm.make_transparent;
			break;
		      default:
			Usage();
			break;
		    }
		break;
	      case 's':		// scale down
		argc--, argv++;
		if (argc <= 0)
		    Usage();
		bm.bitmap_scale_factor = atoi (*argv);
		break;
	      case 't':		// set output file type
		argc--, argv++;
		if (argc <= 0)
		    Usage();
		bm.ofile_type = *argv;
		break;
	      case 'o':		// set output filename pattern
		argc--, argv++;
		if (argc <= 0)
		    Usage();
		bm.ofile_pattern = *argv;
		break;
	      case 'V':		// display version
		cout << version_string << "\nOptions:\n";
#ifdef ENABLE_GIF
		cout << "ENABLE_GIF\n";
#endif
#ifdef ENABLE_KPATHSEA
		cout << "ENABLE_KPATHSEA\n";
#endif
#ifdef MKTEXPK
		cout << "MKTEXPK=" << MKTEXPK << '\n';
#endif
#ifdef MAKETEXPK
		cout << "MAKETEXPK=" << MAKETEXPK << '\n';
#endif
#ifdef DEFAULT_TEXMFCNF
		cout << "DEFAULT_TEXMFCNF=" << DEFAULT_TEXMFCNF << '\n';
#endif
		exit(0);	// ...and exit

	      default:
		Usage();
	    }
	else
	{
	    if (dviname.length() != 0)
		Usage();
	    dviname = *argv;
	}

    if (dviname.length() == 0)
	Usage();

    if (bm.ofile_pattern.length() == 0)
	bm.ofile_pattern = get_ofn_pattern (dviname);
    if (bm.ofile_pattern.length() == 0)
    {
	if (verbosity > silent)
	    cerr << "Can't make output filename pattern from "
		 << dviname << '\n';
	exit(1);
    }

    try
    {
	DviFile *dvif = new DviFile(dviname, resolution, magmag);
	if (dvif->eof())
	{
	    if (verbosity > silent)
		cerr << "Can't open file " << dviname << " to read\n";
	    exit(1);
	}

	all_fonts_present = true;
	no_font_present = true;
	const PkFont *fallback_font = 0;

	for (PkFont *f = dvif->firstFont();
	     f != 0;
	     f = dvif->nextFont())
	{
	    if (f->loaded())
	    {
		no_font_present = false;
		// Set the fallback font to be the first loaded font.
		// Could be more sophisticated - first cmr10?
		if (fallback_font == 0)
		    fallback_font = f;
	    }
	    else		// flag at least one missing
		all_fonts_present = false;
	    if (show_font_list > 0)
		if (show_font_list > 1 || !f->loaded())
		{
		    if (f->loaded()) // show_font_list is >1
			cout << "% ";
		    // write out font name, dpi, base-dpi, mag and MF mode
		    cout << f->name() << ' '
			 << f->dpiBase() << ' '
			 << f->dpi() << ' '
			 << magmag
			 << " localfont";
		    if (f->loaded())
		    {
			string fn = f->fontFilename();
			string unk = "unknown";
			cout << ' ' << (fn.length() > 0 ? fn : unk);
		    }
		    cout << '\n';
		}
	}


	if (do_process_file)
	{
	    if (no_font_present) // give up!
	    {
		if (verbosity > silent)
		    cerr << progname << ": no fonts found!  Giving up\n";
	    }
	    else
		process_dvi_file (dvif, bm, resolution, fallback_font);
	}

    }
    catch (DviBug& e)
    {
	e.print();
    }
    catch (DviError& e)
    {
	e.print();
    }

    // Exit non-zero if we were just checking the pre- and postambles,
    // and we found some missing fonts
    if (no_font_present || (!do_process_file && !all_fonts_present))
	exit (1);
    else
	exit (0);
}

void process_dvi_file (DviFile *dvif, bitmap_info& b, int resolution,
		       const PkFont *fallback_font)
{
    DviFileEvent *ev;
    const PkFont *curr_font = 0;
    int pagenum = 0;
    string output_filename = "";
    Bitmap *bitmap = 0;
    bool end_of_file = false;
    int outcount = 0;		// characters written to output current line

    while (! end_of_file)
    {
	PkGlyph *glyph;

	ev = dvif->getEvent();
	if (verbosity > debug)
	    ev->debug();

	if (DviFilePage *page = dynamic_cast<DviFilePage*>(ev))
	    if (page->isStart)
	    {
		pagenum++;
		// Request a big-enough bitmap.  hSize and vSize are the
		// width and height of the widest and tallest pages,
		// as reported by the DVI file; however, the file doesn't
		// report the offsets of these pages.  Add an inch to
		// both and hope for the best.
		bitmap = new Bitmap
		    ((bitmapW > 0 ? bitmapW : dvif->hSize() + resolution),
		     (bitmapH > 0 ? bitmapH : dvif->vSize() + resolution));
		if (verbosity > quiet)
		{
		    int last, i;
		    // find last non-zero count
		    for (last=9; last>=0; last--)
			if (page->count[last] != 0)
			    break;
		    if (outcount + 4 + 2*(last-1) > 80)
		    {
			cout << '\n';
			outcount = 0;
		    }
		    else
		    {
			cout << ' ';
			outcount++;
		    }
		    cout << '[' << page->count[0];
		    outcount += 2;
		    for (i=1; i<=last; i++)
		    {
			cout << '.' << page->count[i];
			outcount += 2;
		    }
		}
	    }
	    else
	    {
		if (bitmap == 0)
		    throw DviBug ("bitmap uninitialised at page end");
		if (bitmap->empty())
		{
		    if (verbosity > silent)
			cerr << "Warning: page " << pagenum
			     << " empty: nothing written\n";
		}
		else
		{
		    if (bitmap->overlaps() && verbosity > silent)
		    {
			int *bb = bitmap->boundingBox();
			cerr << "Warning: p." << pagenum
			     << ": bitmap too big: occupies (" << bb[0] << ','
			     << bb[1] << ")...(" << bb[2] << ','
			     << bb[3] << ").  Requested "
			     << (bitmapW > 0 ? bitmapW : dvif->hSize() + resolution)
			     << 'x'
			     << (bitmapH > 0 ? bitmapH : dvif->vSize() + resolution)
			     << '\n';
		    }
		    bitmap->crop();
		    if (b.blur_bitmap)
			bitmap->blur();
		    if (b.make_transparent)
			bitmap->setTransparent(true);
		    if (b.bitmap_scale_factor != 1)
			bitmap->scaleDown (b.bitmap_scale_factor);
		    if (output_filename.length() == 0)
		    {
			char fn[100];
			sprintf (fn, b.ofile_pattern.c_str(), pagenum);
			output_filename = fn;
		    }
		    bitmap->write (output_filename, b.ofile_type);
		}
		output_filename = "";

		delete bitmap;
		bitmap = 0;

		if (verbosity > quiet)
		{
		    cout << ']';
		    outcount++;
		}
	    }
	else if (DviFileSetChar *sc = dynamic_cast<DviFileSetChar*>(ev))
	{
	    if (curr_font == 0 || bitmap == 0)
		throw DviBug ("curr_font or bitmap not initialised setting char");
	    glyph = curr_font->glyph(sc->charno);
	    if (verbosity > normal)
	    {
		cerr << "glyph `" << glyph->characterChar()
		     << "\' (" << glyph->characterCode() << ')';
		if (verbosity > debug)
		    cerr << " size " << glyph->w() << 'x' << glyph->h()
			 << " at position ("
			 << dvif->currH() << ',' << dvif->currV() << ')';
		cerr << '\n';
	    }
	    // calculate glyph positions, taking into account the
	    // offsets for the bitmaps, and the (1in,1in)=(72pt,72pt)
	    // = (resolution px,resolution px) offset of the TeX origin.
	    int x = dvif->currH() + glyph->hoff() + resolution;
	    int y = dvif->currV() + glyph->voff() + resolution;
	    bitmap->paint (x, y,
			   glyph->w(), glyph->h(),
			   glyph->bitmap());
	}
	else if (DviFileSetRule *sr = dynamic_cast<DviFileSetRule*>(ev))
	{
	    int x = dvif->currH() + resolution;
	    int y = dvif->currV() + resolution;
	    bitmap->rule (x,y,sr->w, sr->h);
	}
	else if (DviFileFontChange *fc =
		 dynamic_cast<DviFileFontChange*>(ev))
	{
	    const PkFont *f = fc->font;
	    curr_font = (f->loaded() ? f : fallback_font);
	}
	else if (DviFileSpecial* special =
		 dynamic_cast<DviFileSpecial*>(ev))
	{
	    string_list *l = tokenise_string (special->specialString);
	    if (l->size() > 2
		&& (*l)[0] == "dvi2bitmap"
		&& (*l)[1] == "outputfile")
	    {
		output_filename = (*l)[2];
		if (verbosity > normal)
		    cerr << "special: outputfile="
			 << output_filename << '\n';
	    }
	    else
		if (verbosity > silent)
		    cerr << "Warning: unrecognised special: "
			 << special->specialString
			 << '\n';
	    delete l;
	}
	else if (DviFilePostamble *post
		 = dynamic_cast<DviFilePostamble*>(ev))
	    end_of_file = true;

	delete ev;
    }

    if (verbosity > quiet)
	cout << '\n';
}

DviError::DviError(const char *fmt,...)
{
    char *p = new char[2*strlen(fmt)];
    va_list ap;
    va_start(ap,fmt);
    vsprintf (p, fmt, ap);
    va_end(ap);
    problem_ = p;
    delete[] p;
}

void DviError::print() const { cerr << "DVI error: " << problem_ << '\n'; }
void DviBug::print() const { cerr << "BUG: " << problem_ << '\n'; }

DviBug::DviBug(const char *fmt,...)
{
    char *p = new char[2*strlen(fmt)];
    va_list ap;
    va_start(ap,fmt);
    vsprintf (p, fmt, ap);
    va_end(ap);
    problem_ = p;
    delete[] p;
}

string get_ofn_pattern (string dviname)
{
    // strip path and extension from filename
    int string_index = dviname.rfind(path_separator);
    string dvirootname;
    if (string_index < 0)
	dvirootname = dviname;
    else
	dvirootname = dviname.substr(string_index+1);
    string_index = dvirootname.rfind('.');
    if (string_index >= 0) // it's there
	dvirootname = dvirootname.substr(0,string_index);

    return dvirootname + "-page%d";
}

// Tokenise string at whitespace.  There's a more C++-ish way of doing
// this, I'm sure....
string_list *tokenise_string (string str)
{
    if (verbosity > normal)
	cerr << "tokenise_string: string=<" << str << ">\n";
    string_list *l = new string_list();
    int unsigned i=0, wstart;
    // skip leading whitespace
    while (i < str.length() && isspace(str[i]))
	i++;
    while (i < str.length())
    {
	wstart = i;
	while (i < str.length() && !isspace(str[i]))
	    i++;
	string t = str.substr(wstart,i-wstart);
	if (verbosity > normal)
	    cerr << "tokenise:" << t << ":\n";
	l->push_back(t);
	while (i < str.length() && isspace(str[i]))
	    i++;
    }
    return l;
}


void Usage (void)
{
    cerr << "Usage: " << progname << " [-lLqQV] [-b(h|w) size] [-f PKpath ] [-r resolution]\n\t[-P[bt]] [-s scale-factor] [-o outfile-pattern] [-m magmag ]\n\t[-t xbm"
#if ENABLE_GIF
	 << "|gif"
#endif
	 << "] [-n[f]]\n\tdvifile" << '\n';
    exit (1);
}
