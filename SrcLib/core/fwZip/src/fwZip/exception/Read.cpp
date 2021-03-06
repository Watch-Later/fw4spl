/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/Exception.hpp>

#include "fwZip/config.hpp"
#include "fwZip/exception/Read.hpp"

namespace fwZip
{

namespace exception
{

Read::Read ( const std::string &err ) : ::fwCore::Exception(err)
{}

} // namespace exception

} // namespace fwZip

