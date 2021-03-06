/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <fwServices/macros.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwServices/Base.hpp>

#include <fwData/Graph.hpp>

#include <fwComEd/GraphMsg.hpp>


#include "ctrlSelection/wrapper/GraphWrapperSrv.hpp"
#include <fwServices/IEditionService.hpp>


fwServicesRegisterMacro( ::ctrlSelection::IWrapperSrv, ::ctrlSelection::wrapper::GraphWrapperSrv, ::fwData::Graph ) ;

namespace ctrlSelection
{

namespace wrapper
{

//-----------------------------------------------------------------------------

GraphWrapperSrv::GraphWrapperSrv() throw()
{
    //TODO addNewHandledEvent( ::fwServices:: ObjectMsg::NEW_OBJECT );
    //handlingEventOff ::fwServices::ObjectMsg::UPDATED_OBJECT );
    //TODO addNewHandledEvent( ::fwServices:: ObjectMsg::DELETE_OBJECT );
}

//-----------------------------------------------------------------------------

GraphWrapperSrv::~GraphWrapperSrv() throw()
{}

//-----------------------------------------------------------------------------

void GraphWrapperSrv::receiving( ::fwServices::ObjectMsg::csptr message ) throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    if ( message->hasEvent( ::fwServices::ObjectMsg::UPDATED_OBJECT ) )
    {
        assert( message->getDataInfo( ::fwServices::ObjectMsg::UPDATED_OBJECT   ) == this->getObject() );
        fwComEd::GraphMsg::sptr msg = fwComEd::GraphMsg::New();
        msg->addEvent( fwComEd::GraphMsg::NEW_GRAPH , this->getObject() );
        ::fwServices::IEditionService::notify(this->getSptr(), this->getObject(), msg);
    }
    //TODO other event
}

//-----------------------------------------------------------------------------

} // wrapper
} // ctrlSelection
