dnl
dnl   Configure script for dvi2bitmap
dnl
dnl   Copyright 1999--2005, Council of the Central Laboratory
dnl       of the Research Councils
dnl   See file LICENCE for conditions.


AC_INIT(dvi2bitmap, 0.14-alpha, norman@astro.gla.ac.uk)
AC_SUBST(RELEASEDATE, "2005 ???")
# Version-info specifications.  See SSN/78 for guidelines, and update the table
# below for ANY change of version number.  See also the libtool documentation,
# section `Libtool's versioning system'.
#
# Release    libdvi2bitmap.la
#  0.13b1         1:0:0
#  0.13b2         1:1:0
#  0.13b3	  1:2:0
#  0.13		  1:3:0
#  0.14b???       1:4:0
AC_SUBST(libdvi2bitmap_la_version_info, 1:4:0)



AM_INIT_AUTOMAKE
AC_REVISION($Revision: 1.40 $)

AC_PREREQ(2.59)

AC_CONFIG_SRCDIR([dvi2bitmap.cc])

dnl   Checks for programs.
dnl   By giving arguments (CC cxx c++ g++) and (cc gcc egcs), here, we
dnl   can attempt to check for system compilers first (CC and cxx are
dnl   the C++ compilers on Solaris and DUX respectively).  However
dnl   this isn't useful (the README states clearly enough that the
dnl   CXX environment variable can be used to control this), and it
dnl   possibly/potentially causes problems.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

dnl dnl   Macro AS_HELP_STRING is defined in 2.59, but not earlier.
dnl m4_ifdef([AS_HELP_STRING],,m4_define([AS_HELP_STRING],
dnl [m4_pushdef([AS_Prefix], m4_default([$3], [                          ]))dnl
dnl m4_pushdef([AS_Prefix_Format],
dnl            [  %-]m4_eval(m4_len(AS_Prefix) - 3)[s ])dnl [  %-23s ]
dnl m4_text_wrap([$2], AS_Prefix, m4_format(AS_Prefix_Format, [$1]))dnl
dnl m4_popdef([AS_Prefix_Format])dnl
dnl m4_popdef([AS_Prefix])dnl
dnl ]))




dnl  Use C to compile the AC_CHECK_FUNCS tests (using C++ can cause
dnl  spurious linking errors on, for example, Solaris)
AC_LANG([C])

AC_PATH_PROG(PERL, perl)
AC_PATH_PROG(TEX, tex, NO_TEX)
dnl  Test for doxygen; code here and in Makefile.am tests for
dnl  equality with 'NO_DOXYGEN'
AC_PATH_PROG(DOXYGEN, doxygen, NO_DOXYGEN)
if test "$DOXYGEN" = NO_DOXYGEN; then
    AC_MSG_NOTICE([Couldn't find Doxygen -- I'll just create a stub library document.  Doxygen's good though: you can get it from www.doxygen.org])
fi

dnl   The documentation is built using the Starlink SGMLKit, and we
dnl   need to find this.  We can't do it automatically.  FIXME: this
dnl   is ugly, but might need a rewrite of the documentation before
dnl   it can be fixed.
AC_PATH_PROGS(SGML2DOCS, sgml2docs, NOSGML2DOCS)
AC_PATH_PROGS(JADE, [openjade jade], NOJADE)
AC_PATH_PROGS(SGMLNORM, [osgmlnorm sgmlnorm], NOSGMLNORM)

AC_ARG_WITH(sgmldir,
    AS_HELP_STRING([--with-sgmldir],
                   [location of the SGML Kit tools (maintainer only)]),
    STARLINK_SGML_DIR=$withval,
   [if test "$SGML2DOCS" != NOSGML2DOCS; then
        STARLINK_SGML_DIR=`echo $SGML2DOCS | sed s,/bin/sgmldocs,/lib/sgmlkit,`
    else
        STARLINK_SGML_DIR=$STARLINK/lib
    fi])
AC_CHECK_FILE([$STARLINK_SGML_DIR/dtd/CATALOG],,[STARLINK_SGML_DIR=/UNKNOWN])
AC_SUBST(STARLINK_SGML_DIR)
AC_PATH_PROG(SGML2DOCS, sgml2docs, NOSGML2DOCS,
    [$STARLINK_SGML_DIR/../../bin:$PATH])

dnl   Checks for header files (note unistd.h and stdlib are
dnl   automatically checked for, and the corresponding HAVE_ macros
dnl   defined).
AC_HEADER_STDC
AC_CHECK_HEADERS([assert.h fcntl.h signal.h sys/signal.h sys/errno.h sys/stat.h sys/wait.h sys/time.h sys/mman.h sys/types.h])
AC_HEADER_STAT

dnl   Checks for library functions
AC_CHECK_FUNCS(pipe snprintf setenv putenv alarm setitimer)
AC_FUNC_MMAP


dnl   Now switch to doing header checks with C++, for those headers
dnl   which are specific to C++.
AC_LANG([C++])

dnl   Check for the type of mmap arguments.  POSIX says this should be void*,
dnl   but Solaris CC seems to want it to be caddr_t=char*.
AC_MSG_CHECKING([the type of mmap pointers])
for mman_t in 'void*' caddr_t NONE
do
  AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif
], [
int status;
$mman_t ptr;
status = munmap(ptr, 0);
],
  break)
done
if test "$mman_t" = NONE; then
  AC_MSG_RESULT([void* (I hope)])
  mman_t='void*'
else
  AC_MSG_RESULT($mman_t)
fi
AC_DEFINE_UNQUOTED(MMAP_TYPE, $mman_t,
                   [Define as the type of the first argument to munmap])

dnl   Check if we've got sstream; if not, hope we have the older strstream.
AC_CHECK_HEADERS(sstream strstream cassert)

dnl   What type of system are we on?
dnl   The alpha cxx compiler currently (v6.0) needs to be invoked with 
dnl   __USE_STD_IOSTREAM defined, so that it uses a standards-conforming 
dnl   streams library.  See Digital C++ v6.0 manual, section 7.1.2.
dnl   This is defined by default when the compiler is invoked with
dnl   '-std strict_ansi', but that's more drastic than we want to
dnl   have on by default.
dnl
dnl   I don't quite know how, but AC_CANONICAL_HOST appears to be
dnl   invoked implicitly somewhere above this: if I include it here I
dnl   get a warning from autoconf that it has been invoked multiple
dnl   times.  Just check, though...
if test -z "$host"; then
  echo "Ooops, we _do_ need to include ac_canonical_host again!"
  exit 1
fi
dnl AC_CANONICAL_HOST
case "$host" in
  alpha*) test "$CXX" = "cxx" && 
  	  AC_DEFINE(__USE_STD_IOSTREAM,
		    1,
		    [The alpha cxx compiler currently (v6.0) needs to
		    be invoked with __USE_STD_IOSTREAM defined, so
		    that it uses a standards-conforming streams
		    library.  See Digital C++ v6.0 manual, section
		    7.1.2.  This is defined by default when the
		    compiler is invoked with `-std strict_ansi', but
		    that's a little harder to arrange in
		    here. (there's probably a better way of arranging
		    this...)])
 	  ;;
esac

dnl   use C++ to compile the remaining test programs
AC_LANG([C++])

dnl   Do we have (standard) <cname> for C libraries, or just <name.h>?
AC_CACHE_CHECK([whether compiler has <cname> instead of just <name.h>],
               [ac_cv_compiler_has_cname],
               [AC_TRY_COMPILE([#include <cstdio>],,
                               ac_cv_compiler_has_cname=yes,
                               ac_cv_compiler_has_cname=no)])
if test $ac_cv_compiler_has_cname = yes; then
    AC_DEFINE(HAVE_CSTD_INCLUDE, 1,
	      [Define as 1 if you DO have <cstdio> headers.  Otherwise,
	      we use <stdio.h> rather than standard <cstdio>])
fi

dnl   Are the standard functions in the std namespace?
dnl   We only need to check <ostream> here, but <iostream> is on more
dnl   machines (specifically, and irritatingly, <ostream> appears to
dnl   be missing from egcs-2.91.66)
AC_CACHE_CHECK([whether standard functions are in namespace std],
   [my_cv_stdnamespace],
   [my_cv_stdnamespace=
    AC_TRY_COMPILE([#include <iostream>], [std::cout << "Hello\n";],
        my_cv_stdnamespace=yes,
       [AC_TRY_COMPILE([#include <iostream>], [cout<<"Hello\n";],
           my_cv_stdnamespace=no,
           my_cv_stdnamespace=error)])])
case $my_cv_stdnamespace in
  yes)
    stdprefix=std
    ;;
  no)
    stdprefix=
    ;;
  *)
    AC_MSG_ERROR([cannot compile C++])
    ;;
esac
AC_DEFINE_UNQUOTED(STD,
	 	   $stdprefix,
                   [Define as `std' (no quotes) if standard functions are in 
                    the std namespace; define as empty otherwise])

dnl  The C++ standard specifies that the <cctype> contents are
dnl  functions in the std namespace, rather than macros.  This means
dnl  that calling either isalnum() will lose if the compiler is being
dnl  run in a strict mode, but std::isalnum() will lose if it's being
dnl  less strict.  So check this case (this behaviour is generally
dnl  switchable -- for example with option -[no]pure-cname on Tru64,
dnl  which defines __PURE_CNAME -- but of course that's a poor strategy).
AC_CACHE_CHECK([whether ctype functions are in std],
   [my_cv_ctype_in_std],
   [AC_TRY_COMPILE([#include <cctype>], [bool b=std::isalnum('x');],
       my_cv_ctype_in_std=yes,
       my_cv_ctype_in_std=no)])
if test $my_cv_ctype_in_std = yes; then
    AC_DEFINE(CCTYPE_IN_STD, 1, 
       [Define as 1 if, after including <cctype>, the isxxx functions
        are in the std namespace])
fi

dnl  The C++ <cstdio> definition does _not_ list snprintf as one of
dnl  the members, though it is listed in the definition of <stdio.h>
dnl  in the C standard.  So different compilers seem to have different
dnl  ideas of where it should be.  Find out.
AC_CACHE_CHECK([where snprintf is],
   [my_cv_snprintfns],
   [my_cv_snprintfns=
    AC_TRY_COMPILE([
#include <iostream>
#if HAVE_CSTD_INCLUDE
#include <cstdio>
#else
#include <stdio.h>
#endif
],
	[char s[1]; int x = std::snprintf(s, 1, "");],
	 my_cv_snprintfns=std,
	[AC_TRY_COMPILE([
#include <iostream>
#if HAVE_CSTD_INCLUDE
#include <cstdio>
#else
#include <stdio.h>
#endif
],
	    [char s[1]; int x = ::snprintf(s, 1, "");],
	     my_cv_snprintfns=global,
	    [AC_TRY_COMPILE([
#include <iostream>
#if HAVE_CSTD_INCLUDE
#include <cstdio>
#else
#include <stdio.h>
#endif
],
	        [char s[1]; int x = snprintf(s, 1, "");],
		 my_cv_snprintfns=nons,
		 my_cv_snprintfns=broken)])])])
case $my_cv_snprintfns in
  std)    nscode=3 ;;
  global) nscode=2 ;;
  nons)   nscode=1 ;;
  *)      nscode=0 ;; # this case shouldn't really happen
esac
AC_DEFINE_UNQUOTED(SNPRINTF_NAMESPACE, $nscode,
  [Define to 3 if snprintf is in the std:: namespace; to 2 if it is in
   the global namespace; and 1 if it is in no namespace.  Test only if
   HAVE_SNPRINTF is true.])

dnl  Do we have string clear() method? (it's in the standard, but not
dnl  every compiler supports it)
AC_CACHE_CHECK([whether string.clear() is supported],
   [my_cv_string_clear],
   [AC_TRY_COMPILE([#include <string>],
       [STD::string teststring = "hello";
        teststring.clear();],
        my_cv_string_clear=yes,
        my_cv_string_clear=no)])
if test $my_cv_string_clear = yes; then
    AC_DEFINE(HAVE_STRING_CLEAR, 1,
      [Define to 1 if the C++ string.clear() method is supported])
fi


dnl   Extra .lo objects, to be conditionally compiled
AC_SUBST(EXTRALO)
EXTRALO=

dnl   Specify support for Kpathsea library
dnl
dnl   The texmfcnf and fake-progname options
dnl   are suppressed if the support for kpathsea is turned off.  This is
dnl   because they largely depend on kpathsea searching being present.
dnl
dnl   Note, however, that font-generation is NOT, now, disabled if kpathsea
dnl   is.  This can create puzzling errors: fonts might be generated even when
dnl   they're not really missing (because the non-kpathsea search mechanism
dnl   can't find them), but the program still can't find them afterwards 
dnl   (because the font-generation method has put them where only kpathsea
dnl   expects to find them).  From experience with user feedback,
dnl   explaining this problem and running the test script in
dnl   test/test-font-gen.sh (`make test'), which explains what
dnl   DVI2BITMAP_PK_PATH should be set to, is less confusing and
dnl   inconvenient than having to rebuild dvi2bitmap with
dnl   font-generation forced on.
dnl
needfontfinding=1
AC_ARG_WITH(kpathsea,
[  --with-kpathsea[[=path]]  Use kpathsea, giving optional path to library
  --without-kpathsea      Do not use kpathsea library],
  if test "$withval" = "yes"; then
    extrapath=default
  elif test "$withval" = "no"; then
    extrapath=
  else
    extrapath=$withval
  fi,
  extrapath=default)
if test -n "$extrapath"; then
  if test "$extrapath" != default; then
    CPPFLAGS="$CPPFLAGS -I$extrapath/include"
    LIBS="$LIBS -L$extrapath/lib"
  fi
  AC_CHECK_HEADER(kpathsea/pathsearch.h,
     [AC_CHECK_LIB(kpathsea, kpse_find_file,
         [AC_DEFINE(ENABLE_KPATHSEA,
          1,
          [Define as 1 if you want to enable use of the
           kpathsea library for font-searching.  The font
           _generation_ programs (usually) use this library
           when they work their magic, but that's entirely
           separate, and controlled only by ENABLE_FONT_GEN.])]
          EXTRALO="$EXTRALO KarlPathSearcher.lo"
          LIBS="$LIBS -lkpathsea"
          needfontfinding=0)])
fi

dnl   Specify an alternative, or additional, font-finding script.
dnl   This should be expressed as a `font-string template', as
dnl   described in the README
AC_ARG_WITH(fontfinder,
  AS_HELP_STRING([--with-fontfinder=script],
                 [Specify a script to find fonts]),
  if test "$withval" != yes -a "$withval" != no; then
    [AC_DEFINE_UNQUOTED(FONT_SEARCH_SCRIPT,
			"$withval",
			[Specify a command-line which searches for
			fonts according to a specification given in
			its argument list, and returns a single line
			containing a found font file, or EOF if it
			can't find anything. It's a `font-string
			template', as described in the README.  For
			example: 'kpsewhich %f.%dpk'])]
    needfontfinding=0
  fi)

dnl   Test whether we have the kpse* commands, by searching through the path
dnl   for kpsewhich
AC_PATH_PROG(KPSEWHICH, kpsewhich)

dnl   If we're not using the kpselib, and we haven't specified a
dnl   fontfinder script, then use kpsewhich
if test $needfontfinding != 0 -a -n "$ac_cv_path_KPSEWHICH"; then
  AC_DEFINE_UNQUOTED(FONT_SEARCH_SCRIPT, "$ac_cv_path_KPSEWHICH pk %f.%dpk")
  needfontfinding=0
fi


dnl --disable-texmfcnf
if test -n "$ac_cv_path_KPSEWHICH"; then
AC_ARG_ENABLE(texmfcnf,
  AS_HELP_STRING([--disable-texmfcnf],
                 [Disable setting of TEXMFCNF in environment]),
  test "$enableval" = "yes" && T=`kpsewhich cnf texmf.cnf`
  if test -n "$T"; then
    AC_DEFINE_UNQUOTED(DEFAULT_TEXMFCNF,
		       "`dirname $T`",
		       [Set DEFAULT_TEXMFCNF to be the location of the
		       texmf.cnf file which the kpathsea library is to
		       use.  This will be overridden by a value of
		       TEXMFCNF set in the program's environment at
		       run time.  It is not necessary to set this if
		       the program will be installed alongside the
		       other dvi-ware (if they use kpathsea), since
		       the kpathsea library can find its own
		       configuration file relative to that location.
		       This affects only the kpathsea font-searching
		       parts of this program (and so has an effect
		       only if ENABLE_KPATHSEA is 1), and does not
		       affect font-generation.])
  fi,
  T=`kpsewhich cnf texmf.cnf`
  test -n "$T" && AC_DEFINE_UNQUOTED(DEFAULT_TEXMFCNF, "`dirname $T`"))
fi

dnl   Do we enable automatic font-generation?
dnl   If kpathsea is enabled, then the default is yes, otherwise no.
dnl
dnl   Well, not really.  I can't make my mind up about this, but I think
dnl   it's best, in general, if font-generation is always on by default,
dnl   as long as there's enough warning of the problems about this in
dnl   the documentation.
dnl
dnl   This option allows the user to specify a template for the
dnl   font-generation command, in the form of a `font-string
dnl   template', as described in config.h.  If no such template is
dnl   given, then we search for mktexpk and MakeTeXPK in turn, and
dnl   supply the well-known template for whichever one of those we
dnl   find.
dnl
AC_ARG_ENABLE(fontgen,
[  --enable-fontgen[[=template]]
                          Enable automatic font gen., opt. giving template],
  if test "$enableval" = "yes"; then
    getFontGenTemplate=true
  elif test "$enableval" = "no"; then
    getFontGenTemplate=false
  else
    AC_DEFINE_UNQUOTED(FONT_GEN_TEMPLATE,
		       "${enableval}",
		       [Define FONT_GEN_TEMPLATE to be a command which
		       generates fonts, and returns a single line
		       giving the name of the generated file.  It's a
		       `font-string template', as described in the
		       README.  For example:
		       '/usr/local/teTeX/bin/mktexpk --dpi %d --bdpi
		       %b --mag %m --mfmode %M %f' or
		       '/usr/local/teTeX/bin/MakeTeXPK %f %d %b %m
		       %M'.  Undefine to disable.])
    getFontGenTemplate=false
  fi,
  getFontGenTemplate=true)


dnl   --with-mktexpk.  See also --enable-fontgen.
AC_PATH_PROG(MKTEXPK_PATH, mktexpk)
AC_ARG_WITH(mktexpk,
[  --with-mktexpk[[=prog]]   Enable mktexpk (default), giving optional program],
  if test "$withval" = "no"
  then unset MKTEXPK_PATH
dnl  elif test "$withval" != "yes" && -n "$KPATHSEAOBJ"
  elif test "$withval" != "yes"
  then MKTEXPK_PATH=$withval
  fi)

if $getFontGenTemplate && test -n "$MKTEXPK_PATH"; then
  AC_DEFINE_UNQUOTED(FONT_GEN_TEMPLATE, "${MKTEXPK_PATH}  --dpi %d --bdpi %b --mag %m --mfmode %M %f")
  getFontGenTemplate=false  # skip test below
fi

dnl   Use MakeTeXPK.  Don't use this if mktexpk was found
AC_PATH_PROG(MAKETEXPK_PATH, MakeTeXPK)
AC_ARG_WITH(maketexpk,
[  --with-maketexpk[[=prog]]   Enable MakeTeXPK, giving optional alternative],
  if test "$withval" = "no"; then
    unset MAKETEXPK_PATH
  elif test "$withval" != "yes"; then
    MAKETEXPK_PATH=$withval
  fi)

if $getFontGenTemplate && test -n "$MAKETEXPK_PATH"; then
  AC_DEFINE_UNQUOTED(FONT_GEN_TEMPLATE, "${MAKETEXPK_PATH} %f %d %b %m %M")
fi


dnl   Given that we are indeed generating fonts, if we want to use a
dnl   non-default Metafont mode, we can specify it here.
AC_ARG_WITH(fontgen-mode,
[  --with-fontgen-mode=mode,res
                          Specify mode,res for automatic font gen],
  if test "$withval" != "yes" -q "$withval" != "no"; then
    [eval `echo $withval | awk -F, '{printf "GENMODE=%s GENRES=%s",$1,$2}'`]
    AC_DEFINE_UNQUOTED(DEFAULT_MFMODE,
		       "$GENMODE",
		       [Define DEFAULT_MFMODE and DEFAULT_RESOLUTION
		       to be the default Metafont mode and resolution
		       which should be used when generating fonts.
		       These two parameters must be consistent with
		       each other. For examples, see the file
		       modes.mf, which is either available as part of
		       your TeX distribution, or from CTAN.  If not
		       defined, internal defaults will be used.])
    test -n "$GENRES" && AC_DEFINE_UNQUOTED(DEFAULT_RESOLUTION,
					    $GENRES,
					    [See DEFAULT_MFMODE])
  fi)


dnl   Do we want to use the fake_progname feature in KarlPathSearcher.cc?
dnl   (following makes no sense if we don't have kpsewhich, and so
dnl   presumably aren't using the texmf.cnf, but this is a
dnl   use-at-your-own-risk option anyway, so presumably the user knows
dnl   what they're doing)
AC_PATH_PROG(XDVI_PATH, xdvi)
AC_ARG_ENABLE(fake-progname,
[  --enable-fake-progname[[=prog]]
                          Have dvi2bitmap lie about its name (see full docs)],
  if test "$enableval" = "yes"
  then AC_DEFINE_UNQUOTED(FAKE_PROGNAME,
			  "${XDVI_PATH}",
			  [The texmf.cnf file uses the value of
			  variables SELFAUTO..., and sets these based
			  on the location of the program's binary.
			  Some (broken!) configuration files have the
			  VARTEXFONTS variable depending on these, and
			  this means that font generation fails unless
			  the dvi2bitmap program is installed along
			  with the other dvi-ware binaries.  If you
			  have such a configuration file, and you
			  can't or don't want to install the program
			  with the other dvi-ware, then you can define
			  this variable to the location of a binary
			  that is in the `correct' place.  Has an
			  effect only if ENABLE_KPATHSEA is 1.])
  elif test "$enableval" != "no"
  then AC_DEFINE_UNQUOTED(FAKE_PROGNAME, "${enableval}")
  fi)

dnl   Specify support for PNG output.  Test library for function
dnl   png_libpng_ver.  At present, PNGBitmap.cc is
dnl   written to use a palette rather than a greyscale (but see the
dnl   comments about GREYSCALE_BITMAP in that module).  If we were to
dnl   revert to using the colour model PNG_COLOR_TYPE_GRAY, then we
dnl   need the function png_set_write_user_transform_fn (which means later
dnl   than version 0.96).
dnl
dnl   Test this by default (ie, unless the option --disable-png is given).
AC_ARG_WITH(png,
[  --with-png[[=path]]       Generate png files, giving optional path to library
  --without-png           Do not use png library],
  if test "$withval" = "yes"; then
    extrapath=default
  elif test "$withval" = "no"; then
    extrapath=
  else
    extrapath=$withval
  fi,
  extrapath=default)
if test -n "$extrapath"; then
  if test "$extrapath" != default; then
    CPPFLAGS="$CPPFLAGS -I$extrapath/include"
    LIBS="$LIBS -L$extrapath/lib"
  fi
  AC_CHECK_HEADER(png.h,
    [AC_CHECK_LIB(png, png_libpng_ver,
        [AC_DEFINE(ENABLE_PNG,
                   1,
                   [Define as 1 if you want to enable generation of PNGs])]
         EXTRALO="$EXTRALO PNGBitmap.lo"
         LIBS="$LIBS -lpng -lz",
        echo "png library not found or too old -- not including PNG support",
        -lz)])
fi

dnl   Specify support for GIF output
AC_ARG_ENABLE(gif,
  AS_HELP_STRING([--enable-gif],
                 [Enable output of GIF files (default yes)]),
  [case $enableval in
     yes) compile_gifs=yes ;;
     no)  compile_gifs=no ;;
   esac],
   compile_gifs=yes)
if test $compile_gifs = yes; then
  AC_DEFINE(ENABLE_GIF,
	    1,
	    [Define as 1 if you want to enable generation of GIFs])
  EXTRALO="$EXTRALO GIFBitmap.lo"
fi


dnl   Now check whether getopt and co. are defined.  Do this last,
dnl   since some libraries, such as libkpathsea, can provide this, so
dnl   we can get linker errors if we don't notice that.  We need to
dnl   use C for these tests.
AC_LANG([C])

dnl   We ought not to have to worry about getopt.h, getopt{,1}.c,
dnl   since they can always be unconditionally compiled and linked.  See
dnl   http://mail.gnu.org/archive/html/autoconf/2000-12/msg00049.html
dnl   and surrounding thread for discussion.  However that doesn't
dnl   appear to work in fact, and the function in getopt_long.c has
dnl   different magic to avoid getting horribly tangled up.
dnl
dnl   Note that the getopt checks must appear _after_ the checks for
dnl   kpathsea.  This is because the kpathsea library provides an
dnl   implementation for getopt so that, if kpathsea is included, we
dnl   must make sure that we pick up that one.  If this test is
dnl   placed before kpathsea, we can end up defining getopt twice.
AC_CHECK_FUNCS(getopt)
AC_CHECK_DECLS([getopt,getsubopt,getopt_long,setenv,putenv])
AC_REPLACE_FUNCS(getopt_long getsubopt)

dnl   getopt_long.c uses (and provides a replacement for)
dnl   this glibc utility function
AC_CHECK_FUNCS(__strchrnul)


dnl   Select path separator characters.  Specify as a pair of
dnl   characters, first filesystem-path, then search path.  For
dnl   example, unix would have --with-path-seps=/:
AC_DEFINE_UNQUOTED(FSPATH_SEP,
		   '/',
		   [Define filesystem path separator: '/' for unix,
		   and '\' for DOS])
AC_DEFINE_UNQUOTED(SRCHPATH_SEP,
		   ':',
		   [Define search path separator: ':' for unix,
		   and ';' for DOS])
AC_ARG_WITH(path-seps,
[  --with-path-seps=cc     Specify path separators (default --with-path-seps=/:)],
  if test "$withval" != "yes" -q "$withval" != "no"; then
    # xx below allows us to deal with the case where $withval has other
    # than two characters.  Note no \? in basic REs.
    # Quoting below is subtle -- note that eval re-parses its argument.
    [eval `echo "$withval"xx | sed "s/\(.\)\(.\).*/SLASH='\\1' COLON='\\2'/"`]
    AC_DEFINE_UNQUOTED(FSPATH_SEP,'$SLASH')
    test -n "$COLON" && AC_DEFINE_UNQUOTED(SRCHPATH_SEP,'$COLON')
  fi)



dnl   DOXYGEN_OUTPUT_DIRECTORY is the directory into which Doxygen
dnl   output will be placed
AC_SUBST(DOXYGEN_OUTPUT_DIRECTORY)
DOXYGEN_OUTPUT_DIRECTORY="doc_libdvi2bitmap"

AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile doc/Makefile test/Makefile
                 Doxyfile version.h html/index.html dvi2bitmap.1])
AC_CONFIG_FILES(test/t8.pl, [chmod +x test/t8.pl])
AC_OUTPUT

