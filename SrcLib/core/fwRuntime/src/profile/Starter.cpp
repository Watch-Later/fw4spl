/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#include "fwRuntime/profile/Starter.hpp"

#include <iostream>
#include <sstream>

#include "fwRuntime/Runtime.hpp"
#include "fwRuntime/Bundle.hpp"

#include <fwCore/base.hpp>

namespace fwRuntime
{

namespace profile
{



Starter::Starter( const std::string & identifier )
: m_identifier( identifier )
{}



void Starter::apply()
{
	 ::boost::shared_ptr< Bundle >	bundle = Runtime::getDefault()->findBundle(m_identifier);
	if( bundle == 0 )
	{
		OSLM_ERROR("Unable to start bundle " << m_identifier << ". Not found.");
	}
	else
	{
		try
		{
			bundle->start();
		}
		catch( const std::exception & e )
		{
			OSLM_ERROR("Unable to start bundle " << m_identifier << ". " << e.what());
		}
	}
}




} // namespace profile

} // namespace fwRuntime
