/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWRUNTIME_PROFILE_PROFILE_HPP_
#define _FWRUNTIME_PROFILE_PROFILE_HPP_

#include <vector>
#include <boost/utility.hpp>

#include "fwCore/base.hpp"

#include "fwRuntime/config.hpp"


namespace fwRuntime
{

/**
 * @namespace 	::fwRuntime::profile
 * @brief 		Namespace ::fwRuntime::profile
 * @date	2007-2009
 * @author 	IRCAD (Research and Development Team).
 */
namespace profile
{



struct Activater;
struct Starter;
struct Stopper;



/**
 * @brief	Implements a bundle set profile.
 * @struct	Profile
 * @date	2007-2009
 * @author 	IRCAD (Research and Development Team).
 */
struct Profile : public ::fwCore::BaseObject

{

	fwCoreClassDefinitionsWithFactoryMacro( (Profile)(BaseObject::Baseclass), (()), new Profile) ;

	/**
	 * @brief	Constructor : does nothing.
	 */
	FWRUNTIME_API Profile();

	/**
	 * @brief		Adds a new activator.
	 *
	 * @param[in]	activater	a shared pointer to an activator
	 */
	FWRUNTIME_API void add( ::boost::shared_ptr< Activater > activater );

	/**
	 * @brief		Adds a new starter.
	 *
	 * @param[in]	starter	a shared pointer to a starter
	 */
	FWRUNTIME_API void add( ::boost::shared_ptr< Starter > starter );

	/**
	 * @brief	Starts the profile.
	 */
	FWRUNTIME_API void start();
	FWRUNTIME_API void stop();

	/**
	 * @brief	Return profile name.
	 */
	FWRUNTIME_API std::string getName() { return m_sName; }

	/**
	 * @brief		Set profile name.
	 *
	 * @param[in]	_sName	profile name
	 */
	FWRUNTIME_API void setName(std::string _sName) { m_sName = _sName; }


private:

	typedef std::vector< ::boost::shared_ptr< Activater > >	ActivaterContainer;
	typedef std::vector< ::boost::shared_ptr< Starter > >	StarterContainer;
	typedef std::vector< ::boost::shared_ptr< Stopper > >	StopperContainer;

	ActivaterContainer	m_activaters;	///< all managed activators
	StarterContainer	m_starters;		///< all managed starters
	StopperContainer	m_stoppers;		///< all managed stoppers
	std::string			m_sName;		///< name profile

};



} // namespace profile

} // namespace fwRuntime



#endif /*_FWRUNTIME_PROFILE_PROFILE_HPP_*/
