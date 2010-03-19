/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <wx/wx.h>
#include <wx/version.h>
#include <wx/event.h>

#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>

#include <fwCore/base.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Object.hpp>

#include <fwServices/Factory.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwServices/ObjectMsg.hpp>

#include <io/IReader.hpp>

#include <fwTools/System.hpp>

#include <ioXML/FwXMLGenericReaderService.hpp>

#include <fwWX/ProgressTowx.hpp>
#include <fwWX/wxZipFolder.hpp>
#include <fwWX/convert.hpp>

#include "ioXML/FwXMLGenericReaderService.hpp"


//------------------------------------------------------------------------------
//
namespace ioXML
{

REGISTER_SERVICE( ::io::IReader , ::ioXML::FwXMLGenericReaderService , ::fwTools::Object );

//------------------------------------------------------------------------------

FwXMLGenericReaderService::FwXMLGenericReaderService() throw() :
    m_bServiceIsConfigured(false),
    m_fsObjectPath("")
{}

//------------------------------------------------------------------------------

FwXMLGenericReaderService::~FwXMLGenericReaderService() throw()
{}

//------------------------------------------------------------------------------

void FwXMLGenericReaderService::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    // Test if in the service configuration the tag filename is defined. If it is defined, the image path is initialized and we tag the service as configured.
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
        m_fsObjectPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = ::boost::filesystem::exists(m_fsObjectPath);
        if(m_bServiceIsConfigured)
        {
            OSLM_TRACE("Filename found in service configuration : patient path = " << filename ) ;
        }
        else
        {
            OSLM_WARN("filename not exist = " <<  filename ) ;
        }
    }
}

//------------------------------------------------------------------------------

void FwXMLGenericReaderService::configureWithIHM()
{
    static wxString _sDefaultPath = _("");
    wxString title = ::fwWX::std2wx( this->getSelectorDialogTitle());
    wxString file = wxFileSelector(
            title,
            _sDefaultPath,
            wxT(""),
            wxT(""),
            wxT("fwXML (*.fxz;*.xml)|*.fxz;*.xml"),
#if wxCHECK_VERSION(2, 8, 0)
            wxFD_FILE_MUST_EXIST,
#else
            wxFILE_MUST_EXIST,
#endif
            wxTheApp->GetTopWindow() );

    if( file.IsEmpty() == false)
    {
        fixFilename(file);
        _sDefaultPath = wxConvertMB2WX( m_fsObjectPath.branch_path().string().c_str() );
    }
}

//------------------------------------------------------------------------------
//
void FwXMLGenericReaderService::fixFilename(wxString _filename)
{
    m_fsObjectPath = ::boost::filesystem::path( ::fwWX::wx2std(_filename), ::boost::filesystem::native );
    m_bServiceIsConfigured = true;

}

//------------------------------------------------------------------------------

void FwXMLGenericReaderService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE("FwXMLGenericReaderService::starting()");
#ifdef __WXMAC__
    wxFrame *frame = wxDynamicCast( wxTheApp->GetTopWindow() , wxFrame ) ;
    if (frame != NULL)
        frame->Connect( wxIDEventFwOpen, wxEventFwOpen, wxCommandEventHandler(wxEvtHandlerOpenFile::open) );
    else
        SLM_FATAL ("Window not found !")
#endif
}

//------------------------------------------------------------------------------

void FwXMLGenericReaderService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE("FwXMLGenericReaderService::stopping()");
}

//------------------------------------------------------------------------------

void FwXMLGenericReaderService::info(std::ostream &_sstream )
{
    _sstream << "FwXMLGenericReaderService::info" ;
}

//------------------------------------------------------------------------------

std::vector< std::string > FwXMLGenericReaderService::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    extensions.push_back(".xml");
    return extensions ;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

::fwTools::Object::sptr FwXMLGenericReaderService::loadData( const ::boost::filesystem::path inrFileDir )
{
    SLM_TRACE("FwXMLGenericReaderService::createObject");
    ::fwXML::reader::FwXMLObjectReader myLoader;
    ::fwTools::Object::sptr pObject;

    myLoader.setFile(inrFileDir);

    try
    {
        ::fwWX::ProgressTowx progressMeterGUI("Loading Image ");
        myLoader.addHandler( progressMeterGUI );
        myLoader.read();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        wxString msg = _("Warning during loading : ");
        ss << wxConvertWX2MB(msg.c_str()) << e.what();
        wxString wxStmp( ss.str().c_str(), wxConvLocal );
        wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );
        return pObject;
    }
    catch( ... )
    {
        std::stringstream ss;
        ss << "Warning during loading : ";
        wxString wxStmp( ss.str().c_str(), wxConvLocal );
        wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );
        return pObject;
    }

    pObject = ::fwTools::Object::dynamicCast( myLoader.getObject() );

    return pObject;
}

//------------------------------------------------------------------------------

void FwXMLGenericReaderService::updating() throw(::fwTools::Failed)
{
    OSLM_TRACE("FwXMLGenericReaderService::updating()  m_fsObjectPath:"<<  m_fsObjectPath);

    SLM_TRACE_FUNC();

    if( !m_reader.getFile().empty() )
    {
        ::fwTools::Object::sptr obj; // object loaded

        wxBeginBusyCursor();
        m_reader.setFile( correctFileFormat( m_reader.getFile() ));
        if ( isAnFwxmlArchive( m_reader.getFile() ) )
        {
            obj = manageZipAndLoadData( m_reader.getFile() );
        }
        else
        {
            obj = loadData(m_reader.getFile() );
        }
        if (obj)
        {
			// Retrieve dataStruct associated with this service
			::fwTools::Object::sptr associatedObject = this->getObject< ::fwTools::Object >();
			assert( associatedObject ) ;

			associatedObject->shallowCopy( obj );

			notificationOfUpdate();
        }
        wxEndBusyCursor();
    }
}


//------------------------------------------------------------------------------

void FwXMLGenericReaderService::notificationOfUpdate()
{
    SLM_TRACE("FwXMLGenericReaderService::notificationOfUpdate");
    ::fwTools::Object::sptr object = this->getObject< ::fwTools::Object >();
    assert( object );
//
//    ::fwComEd::ObjectMsg::NewSptr msg;
//    msg->addEvent( ::fwComEd::ObjectMsg::NEW_PATIENT, pDPDB );
//    msg->addEvent( ::fwComEd::ObjectMsg::NEW_LOADED_PATIENT );
//
//    ::fwServices::IEditionService::notify( this->getSptr(),  pDPDB, msg );
}

//------------------------------------------------------------------------------

bool FwXMLGenericReaderService::isAnFwxmlArchive( const ::boost::filesystem::path filePath )
{
    return ( ::boost::filesystem::extension(filePath) == ".fxz" );
}

//------------------------------------------------------------------------------

::fwTools::Object::sptr FwXMLGenericReaderService::manageZipAndLoadData( const ::boost::filesystem::path _pArchivePath )
{

    ::fwTools::Object::sptr obj;

    // Unzip folder
    ::boost::filesystem::path destFolder = ::fwTools::System::getTemporaryFolder() / "fwxmlArchiveFolder";
    ::boost::filesystem::path xmlfile = destFolder / "root.xml";

    OSLM_DEBUG("srcZipFileName = " << _pArchivePath );
    OSLM_DEBUG("destFolderName = " << destFolder );

    wxString srcZipFileName ( wxConvertMB2WX( _pArchivePath.string().c_str() ) );
    wxString destFolderName ( wxConvertMB2WX( destFolder.string().c_str() ) );
    ::fwWX::wxZipFolder::unpackFolder( srcZipFileName, destFolderName );

    // Load
    obj = loadData( xmlfile );

    // Remove temp folder
    ::boost::filesystem::remove_all( destFolder );

    return obj;
}

//------------------------------------------------------------------------------

} // namespace ioXML
