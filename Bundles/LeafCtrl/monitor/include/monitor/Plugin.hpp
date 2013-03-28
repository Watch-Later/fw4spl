/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _MONITOR_PLUGIN_HPP_
#define _MONITOR_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>

namespace monitor
{

/// Class called when the bundle is loaded and stopped
struct Plugin : public ::fwRuntime::Plugin
{

    /// Destructor
    ~Plugin() throw();

    // Install a callback which print the backtrace on a SIGSEV and SIGUSR1 signal
    void start() throw(::fwRuntime::RuntimeException);

    // Overrides
    void stop() throw();

};


} // namespace monitor


#endif //_MONITOR_PLUGIN_HPP_
