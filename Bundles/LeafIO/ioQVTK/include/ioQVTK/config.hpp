/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOQVTK_CONFIG_HPP_
#define _IOQVTK_CONFIG_HPP_


#ifdef _WIN32
    #pragma warning( disable : 4251 )


    #ifdef IOQVTK_EXPORTS
    #define IOQVTK_API __declspec(dllexport)
    #else
    #define IOQVTK_API __declspec(dllimport)
    #endif

    #define IOQVTK_CLASS_API
    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef IOQVTK_EXPORTS
    #define IOQVTK_API __attribute__ ((visibility("default")))
    #define IOQVTK_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define IOQVTK_API __attribute__ ((visibility("hidden")))
    #define IOQVTK_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define IOQVTK_API
    #define IOQVTK_CLASS_API

#endif

#endif // #ifndef _IOVTK_CONFIG_HPP_
