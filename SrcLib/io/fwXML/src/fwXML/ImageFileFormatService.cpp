/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwXML/ImageFileFormatService.hpp"

#include <fwData/Image.hpp>
#include <fwData/location/SingleFile.hpp>
#include <fwCore/base.hpp>

#include <fwTools/Failed.hpp>

#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/macros.hpp>

//#include <fwMemory/MemoryMonitor.hpp>

#include <fwDataIO/writer/GzBufferImageWriter.hpp>
#include <fwDataIO/reader/GzBufferImageReader.hpp>


REGISTER_SERVICE( ::fwXML::IFileFormatService , fwXML::ImageFileFormatService , ::fwData::Image);

namespace fwXML
{



ImageFileFormatService::ImageFileFormatService()
{
    RWPoliciesInstall();
}



ImageFileFormatService::~ImageFileFormatService()
{
}






void ImageFileFormatService::RWPoliciesInstall()
{
    if ( m_reader.get() == 0 ) // reader not set
    {
        // try to install inrReader
        ::boost::shared_ptr< ::fwDataIO::reader::IObjectReader > reader;

        reader = fwTools::ClassFactoryRegistry::create< ::fwDataIO::reader::IObjectReader, std::string >("::vtkIO::ImageReader" );
        if(reader)
        {
            setReader(reader);
        }
        else
        {
            reader = fwTools::ClassFactoryRegistry::create< ::fwDataIO::reader::IObjectReader, std::string >( "::itkIO::ImageReader" );
            if ( reader )
            {
                setReader( reader );
            }
            else
            {
                setReader( ::boost::shared_ptr< ::fwDataIO::reader::GzBufferImageReader >( new ::fwDataIO::reader::GzBufferImageReader() ));
            }
        }
    }
    if ( m_writer.get() == 0 ) // writer not set
    {
        // try to install inrWriterer
         ::boost::shared_ptr< ::fwDataIO::writer::IObjectWriter > writer;
         writer = fwTools::ClassFactoryRegistry::create< ::fwDataIO::writer::IObjectWriter, std::string >( "::vtkIO::ImageWriter" );
         if ( writer )
         {
             setWriter( writer );
         }
        else
        {
            writer = fwTools::ClassFactoryRegistry::create< ::fwDataIO::writer::IObjectWriter, std::string >( "::itkIO::ImageWriter" );
            if ( writer )
            {
                setWriter( writer );
            }
            else
            {
                setWriter( ::boost::shared_ptr< ::fwDataIO::writer::GzBufferImageWriter >( new ::fwDataIO::writer::GzBufferImageWriter() ));
            }

        }
    }
}



 void ImageFileFormatService::load()
{
    // precondition

    assert( !m_filename.empty() );
    // assert( !m_localFolder.empty() ); not mandatory can be loaded at root Folder

    //RWPoliciesInstall();
    assert( m_reader );

     ::boost::shared_ptr< ::fwData::Image > image = this->getObject< ::fwData::Image >() ;
    assert( image ) ;

    m_reader->setObject(image);
    ::boost::shared_ptr< ::fwData::location::SingleFile > path( new ::fwData::location::SingleFile() );
    path->setPath( this->getFullPath() );
    m_reader->setLocation( path );

    //FIXME JMO
    // reserve memory (300Mo)
    /*if ( ! ::fwMemory::MemoryMonitor::getDefault()->reserveMemory(1024*1024*300) )
    {
        throw ::fwTools::Failed("can not reserve enough memory");
    }*/

    m_reader->read();
}



void ImageFileFormatService::save()
{
    SLM_TRACE("ImageFileFormatService::save");

    assert( !m_filename.empty() );
    // assert( !m_localFolder.empty() ); not mandatory can be saved at root Folder

    //RWPoliciesInstall();
    assert( m_writer );

     ::boost::shared_ptr< ::fwData::Image > image = this->getObject< ::fwData::Image >() ;
    assert( image ) ;

    if (image->getBuffer() )
    {
        m_writer->setObject(image);
        this->extension() = m_writer->extension();
        ::boost::shared_ptr< ::fwData::location::SingleFile > path( new ::fwData::location::SingleFile() );
        path->setPath( this->getFullPath() );
        m_writer->setLocation( path );
        m_writer->write();
    }

}





}
