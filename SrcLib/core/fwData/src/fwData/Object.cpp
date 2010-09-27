/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>
#include <fwTools/ClassRegistrar.hpp>
#include <fwTools/UUID.hpp>

#include "fwData/visitor/IVisitor.hpp"

#include "fwData/Object.hpp"

// HACK
REGISTER_BINDING_BYCLASSNAME( ::fwTools::Object, ::fwTools::Field, ::fwTools::Field );
namespace fwTools
{
/// END HACK
}

namespace fwData
{


//------------------------------------------------------------------------------

Object::Object()
{

}

//------------------------------------------------------------------------------

Object::~Object()
{

}

//------------------------------------------------------------------------------


} // namespace fwData
