/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/spyLog.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Integer.hpp>
#include <fwData/String.hpp>

#include <fwComEd/fieldHelper/BackupHelper.hpp>

#include <fwTools/fwID.hpp>

#include <fwServices/macros.hpp>

#include <fwComEd/PatientDBMsg.hpp>

#include "ctrlSelection/updater/PSAFromPDBSelectorUpdaterSrv.hpp"

namespace ctrlSelection
{

namespace updater
{

//-----------------------------------------------------------------------------

REGISTER_SERVICE( ::ctrlSelection::IUpdaterSrv, ::ctrlSelection::updater::PSAFromPDBSelectorUpdaterSrv, ::fwData::Composite ) ;

//-----------------------------------------------------------------------------

PSAFromPDBSelectorUpdaterSrv::PSAFromPDBSelectorUpdaterSrv() throw()
{}

//-----------------------------------------------------------------------------

PSAFromPDBSelectorUpdaterSrv::~PSAFromPDBSelectorUpdaterSrv() throw()
{}

//-----------------------------------------------------------------------------

void PSAFromPDBSelectorUpdaterSrv::updating( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed )
{
    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();
    for (   PSAManagedEvents::iterator it = m_psaManagedEvents.begin();
            it != m_psaManagedEvents.end();
            ++it )
    {
        // First element of tuple ( it->get<0>() ) is the event, test if message correspond to a defined event
        if( _msg->hasEvent( it->get<0>() ) )
        {
            ::fwData::Object::sptr obj = ::fwData::Object::dynamicCast( _msg->getSubject().lock() );
            SLM_ASSERT(obj,"Sorry, the subject of message is not a ::fwData::Object");

            // Test if we manage this event from this object message uid ( it->get<1>() )
            if( obj->getID() == it->get<1>() )
            {
                ::fwData::PatientDB::sptr patientDB = ::fwData::PatientDB::dynamicCast(obj);
                SLM_ASSERT("Sorry, the subject of message is not a ::fwData::PatientDB", patientDB);

                ::fwData::Patient::sptr pat;
                ::fwData::String::sptr patientName;
                ::fwData::String::sptr patientInfo;
                ::fwData::Study::sptr stu;
                ::fwData::Acquisition::sptr acq;
                if( it->get<7>() != REMOVE )
                {
                    // Get selection
                    pat = ::fwComEd::fieldHelper::BackupHelper::getSelectedPatient(patientDB);
                    stu = ::fwComEd::fieldHelper::BackupHelper::getSelectedStudy(patientDB);
                    acq = ::fwComEd::fieldHelper::BackupHelper::getSelectedAcquisition(patientDB);
                    patientName = ::fwData::String::NewSptr( pat->getCRefName() );
                    std::string acqDate = ::boost::posix_time::to_iso_extended_string( acq->getCreationDate() );
                    acqDate =  acqDate.substr(0,10) + " " + acqDate.substr(11,5);
                    std::stringstream info;
                    info << pat->getCRefName() << "\n" << acqDate;
                    patientInfo = ::fwData::String::NewSptr( info.str() );
                }
                // Udapte the composite object referenced by the composite key ( it->get<2>() )
                this->updateComposite(composite, pat, it->get<2>(), it->get<7>() );
                this->updateComposite(composite, stu, it->get<3>(), it->get<7>() );
                this->updateComposite(composite, acq, it->get<4>(), it->get<7>() );
                this->updateComposite(composite, patientName, it->get<5>(), it->get<7>() );
                this->updateComposite(composite, patientInfo, it->get<6>(), it->get<7>() );
            }
        }
    }
}

//-----------------------------------------------------------------------------

void PSAFromPDBSelectorUpdaterSrv::starting()  throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void PSAFromPDBSelectorUpdaterSrv::stopping()  throw ( ::fwTools::Failed )
{}

//-----------------------------------------------------------------------------

void PSAFromPDBSelectorUpdaterSrv::configuring()  throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    ::fwRuntime::ConfigurationElementContainer handleEvents = m_configuration->findAllConfigurationElement("update");

    SLM_ASSERT("Problem with configuration for PSAFromPDBSelectorUpdaterSrv type, missing element \"update\"", handleEvents.size() != 0 );
    OSLM_DEBUG( "handleEvents.size() = " << handleEvents.size() );
    m_psaManagedEvents.clear();
    for(    ::fwRuntime::ConfigurationElementContainer::Iterator item = handleEvents.begin();
            item != handleEvents.end();
            ++item )
    {
        SLM_FATAL_IF( "Sorry, attribute \"patientKey\" is missing", !(*item)->hasAttribute("patientKey") );
        std::string patientKey =  (*item)->getExistingAttributeValue("patientKey");

        SLM_FATAL_IF( "Sorry, attribute \"studyKey\" is missing", !(*item)->hasAttribute("studyKey") );
        std::string studyKey =  (*item)->getExistingAttributeValue("studyKey");

        SLM_FATAL_IF( "Sorry, attribute \"acquisitionKey\" is missing", !(*item)->hasAttribute("acquisitionKey") );
        std::string acquisitionKey =  (*item)->getExistingAttributeValue("acquisitionKey");

        SLM_FATAL_IF( "Sorry, attribute \"patientNameKey\" is missing", !(*item)->hasAttribute("patientNameKey") );
        std::string patientNameKey =  (*item)->getExistingAttributeValue("patientNameKey");

        SLM_FATAL_IF( "Sorry, attribute \"patientInfoKey\" is missing", !(*item)->hasAttribute("patientInfoKey") );
        std::string patientInfoKey =  (*item)->getExistingAttributeValue("patientInfoKey");

        SLM_FATAL_IF( "Sorry, attribute \"onEvent\" is missing", !(*item)->hasAttribute("onEvent") );
        std::string onEvent =  (*item)->getExistingAttributeValue("onEvent");

        SLM_FATAL_IF( "Sorry, attribute \"fromUID\" is missing", !(*item)->hasAttribute("fromUID") );
        std::string fromUID =  (*item)->getExistingAttributeValue("fromUID");

        SLM_FATAL_IF( "Sorry, attribute \"actionType\" is missing", !(*item)->hasAttribute("actionType") );
        std::string actionType =  (*item)->getExistingAttributeValue("actionType");

        ActionType action;
        if ( actionType == "ADD" )                    { action = ADD; }
        else if ( actionType == "SWAP" )              { action = SWAP; }
        else if ( actionType == "REMOVE" )            { action = REMOVE; }
        else if ( actionType == "ADD_OR_SWAP" )       { action = ADD_OR_SWAP; }
        else if ( actionType == "REMOVE_IF_PRESENT" ) { action = REMOVE_IF_PRESENT; }
        else if ( actionType == "DO_NOTHING" )        { action = DO_NOTHING; }
        else
        {
            SLM_FATAL("Sorry this type of \"actionType\" is not managed by PSAFromPDBSelectorUpdaterSrv type");
        }

        OSLM_INFO( "Manage event "<< onEvent <<" from this object "<< fromUID <<" and "<< actionType << " "<< patientKey << "  " << studyKey << "  " << acquisitionKey << "  " << patientNameKey << " in my composite.");
        PSAManagedEvent managedEvent (onEvent, fromUID, patientKey, studyKey, acquisitionKey, patientNameKey, patientInfoKey, action);
        m_psaManagedEvents.push_back( managedEvent );
        addNewHandledEvent( onEvent );
    }
}

//-----------------------------------------------------------------------------

void PSAFromPDBSelectorUpdaterSrv::reconfiguring()  throw ( ::fwTools::Failed )
{}

//-----------------------------------------------------------------------------

void PSAFromPDBSelectorUpdaterSrv::updating() throw ( ::fwTools::Failed )
{}

//-----------------------------------------------------------------------------

void PSAFromPDBSelectorUpdaterSrv::info( std::ostream &_sstream )
{}

//-----------------------------------------------------------------------------

} // updater
} // ctrlSelection
