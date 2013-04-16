/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/operations.hpp>
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <fwRuntime/helper.hpp>

#include "monitorQt/Plugin.hpp"

namespace monitor
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::monitorQt::Plugin");

//------------------------------------------------------------------------------

Plugin::~Plugin() throw()
{}

//------------------------------------------------------------------------------

void Plugin::start() throw(::fwRuntime::RuntimeException)
{}

//------------------------------------------------------------------------------

void Plugin::stop() throw()
{}

//------------------------------------------------------------------------------

} // namespace monitor
