/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#include <iostream>

#include <boost/regex.hpp>

#include "fwServices/ObjectMsg.hpp"
#include "fwServices/IService.hpp"

namespace fwServices
{

//-----------------------------------------------------------------------------

ObjectMsg::ObjectMsg()
{}

//-----------------------------------------------------------------------------

ObjectMsg::~ObjectMsg()
{}

//-----------------------------------------------------------------------------

void ObjectMsg::addEvent( std::string _eventId , ::boost::shared_ptr< ::fwData::Object > _pDataInfo )
{
	m_eventId2DataInfo[ _eventId ] = _pDataInfo;
}

//-----------------------------------------------------------------------------

bool ObjectMsg::hasEvent( std::string _eventId ) const
{
	return m_eventId2DataInfo.find( _eventId ) != m_eventId2DataInfo.end() ;
}

//-----------------------------------------------------------------------------

std::vector< std::string > ObjectMsg::getEventIds() const
{
	std::vector< std::string > result ;
	std::map< std::string , ::fwData::Object::sptr >::const_iterator iter ;
	for( iter = m_eventId2DataInfo.begin() ; iter != m_eventId2DataInfo.end() ; ++iter )
	{
		result.push_back( iter->first ) ;
	}
	return result ;
}

//-----------------------------------------------------------------------------

::boost::shared_ptr< const ::fwData::Object > ObjectMsg::getDataInfo( std::string _eventId ) const
{
	return m_eventId2DataInfo.find( _eventId )->second;
}

//-----------------------------------------------------------------------------

std::string ObjectMsg::convertToLightString( std::string _initialString )
{
	std::string lightString = _initialString;

	::boost::regex toolsRegex ("::fwTools::(.*)");
	::boost::regex dataRegex ("::fwData::(.*)");
	const std::string machine_format("\\1");

	if ( ::boost::regex_match( _initialString, toolsRegex ) )
	{
		lightString = ::boost::regex_replace( _initialString, toolsRegex, machine_format, boost::match_default | boost::format_sed );
	}
	else if ( ::boost::regex_match( _initialString, dataRegex ) )
	{
		lightString = ::boost::regex_replace( _initialString, dataRegex, machine_format, boost::match_default | boost::format_sed );
	}

	return lightString;
}

//-----------------------------------------------------------------------------

std::string ObjectMsg::getGeneralInfo() const
{
	std::string msgUUID = convertToLightString( this->getUUID() );
	std::string sourceUUID = convertToLightString( m_source.lock()->getUUID() );
	std::string destUUID = convertToLightString( m_subject.lock()->getUUID() );

	std::stringstream eventstream;
	for( 	std::map< std::string, ::fwData::Object::sptr >::const_iterator itEvent2Data = m_eventId2DataInfo.begin();
			itEvent2Data != m_eventId2DataInfo.end();
			++itEvent2Data )
	{
		eventstream << "| " << itEvent2Data->first << " |";
	}


	std::stringstream sstream;
	sstream << msgUUID << " | " << sourceUUID << " ===> " << destUUID << eventstream.str();

	return sstream.str();
}

//-----------------------------------------------------------------------------

std::ostream & operator<<(std::ostream & _ostream, const ObjectMsg& _message)
{
	_message.info( _ostream ) ;
	return _ostream ;
}

//-----------------------------------------------------------------------------

void ObjectMsg::info(std::ostream &_sstream ) const
{
	_sstream << getGeneralInfo();
}

//-----------------------------------------------------------------------------

void ObjectMsg::setSource(::boost::weak_ptr< ::fwServices::IService > _source)
{
	m_source = _source;
}

//-----------------------------------------------------------------------------

::boost::weak_ptr< ::fwServices::IService > ObjectMsg::getSource() const
{
	return m_source;
}

//-----------------------------------------------------------------------------

void ObjectMsg::setSubject( ::boost::weak_ptr< ::fwTools::Object > _subject)
{
	m_subject = _subject;
}

//-----------------------------------------------------------------------------

::boost::weak_ptr< ::fwTools::Object > ObjectMsg::getSubject() const
{
	return m_subject;
}

//-----------------------------------------------------------------------------

} // namespace fwServices

