/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwData/Port.hpp>

#include <fwCamp/UserObject.hpp>

fwCampImplementDataMacro((fwData)(Port))
{
    builder
        .tag("object_version", "1")
        .tag("lib_name", "fwData")
        .base< ::fwData::Object>()
        .property("identifier", &::fwData::Port::m_identifier)
        .property("type", &::fwData::Port::m_type)
        ;
}
