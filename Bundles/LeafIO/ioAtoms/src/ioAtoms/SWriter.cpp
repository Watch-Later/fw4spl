/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/path.hpp>

#include <fwAtomsBoostIO/Writer.hpp>

#include <fwAtomsHdf5IO/Writer.hpp>

#include <fwAtomConversion/convert.hpp>

#include <fwData/Composite.hpp>
#include <fwData/location/SingleFile.hpp>
#include <fwData/location/Folder.hpp>

#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <fwServices/macros.hpp>

#include <fwZip/WriteDirArchive.hpp>
#include <fwZip/WriteZipArchive.hpp>

#include <fwDataCamp/visitor/RecursiveLock.hpp>

#include "ioAtoms/SWriter.hpp"

namespace ioAtoms
{

//-----------------------------------------------------------------------------

fwServicesRegisterMacro( ::io::IWriter , ::ioAtoms::SWriter , ::fwData::Object );

//-----------------------------------------------------------------------------

void SWriter::starting() throw(::fwTools::Failed)
{}

//-----------------------------------------------------------------------------

void SWriter::stopping() throw(::fwTools::Failed)
{}

//-----------------------------------------------------------------------------

void SWriter::updating() throw(::fwTools::Failed)
{
    if( this->hasLocationDefined() )
    {
        ::fwData::Object::sptr obj = this->getObject< ::fwData::Object >();

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);
        try
        {
            const ::boost::filesystem::path& filePath = this->getFile();
            const ::boost::filesystem::path folderPath = filePath.parent_path();
            const ::boost::filesystem::path filename = filePath.filename();
            const std::string extension = ::boost::filesystem::extension(filePath);

            FW_RAISE_IF( "Extension is empty", extension.empty() );

            // Mutex data lock
            ::fwDataCamp::visitor::RecursiveLock recursiveLock (obj);

            // Convert data to atom
            ::fwAtoms::Base::sptr atom = ::fwAtomConversion::convert(obj);

            if (extension == ".hdf5")
            {
                ::fwAtomsHdf5IO::Writer(atom).write( filePath );
            }
            else
            {
                // Write atom
                ::fwZip::IWriteArchive::sptr writeArchive;
                ::fwAtomsBoostIO::Writer::FormatType format;
                ::boost::filesystem::path archiveRootName;
                if ( extension == ".json" )
                {
                    writeArchive = ::fwZip::WriteDirArchive::New(folderPath.string());
                    archiveRootName = filename;
                    format = ::fwAtomsBoostIO::Writer::JSON;
                }
                else if ( extension == ".jsonz" )
                {
                    if ( ::boost::filesystem::exists( filePath ) )
                    {
                        ::boost::filesystem::remove( filePath );
                    }
                    writeArchive = ::fwZip::WriteZipArchive::New(filePath.string());
                    archiveRootName = "root.json";
                    format = ::fwAtomsBoostIO::Writer::JSON;
                }
                else if ( extension == ".xml" )
                {
                    writeArchive = ::fwZip::WriteDirArchive::New(folderPath.string());
                    archiveRootName = filename;
                    format = ::fwAtomsBoostIO::Writer::XML;
                }
                else if ( extension == ".xmlz" )
                {
                    if ( ::boost::filesystem::exists( filePath ) )
                    {
                        ::boost::filesystem::remove( filePath );
                    }
                    writeArchive = ::fwZip::WriteZipArchive::New(filePath.string());
                    archiveRootName = "root.xml";
                    format = ::fwAtomsBoostIO::Writer::XML;
                }
                else
                {
                    FW_RAISE( "This file extension '" << extension << "' is not managed" );
                }

                ::fwAtomsBoostIO::Writer(atom).write( writeArchive, archiveRootName, format );
                writeArchive.reset();
            }
        }
        catch( std::exception & e )
        {
            OSLM_ERROR( e.what() );
            ::fwGui::dialog::MessageDialog::showMessageDialog("Medical data writer failed",
                    e.what(),
                    ::fwGui::dialog::MessageDialog::CRITICAL);
        }
        catch( ... )
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog("Medical data writer failed",
                    "Writing process abort",
                    ::fwGui::dialog::MessageDialog::CRITICAL);
        }
        cursor.setDefaultCursor();
    }
}

//-----------------------------------------------------------------------------

::io::IOPathType SWriter::getIOPathType() const
{
    return ::io::FILE;
}

//-----------------------------------------------------------------------------

void SWriter::configureWithIHM()
{
   static ::boost::filesystem::path _sDefaultPath;

   ::fwGui::dialog::LocationDialog dialogFile;
   dialogFile.setTitle("Enter file name");
   dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
   dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);
   dialogFile.setType(::fwGui::dialog::LocationDialog::SINGLE_FILE);

   dialogFile.addFilter( "JSON", "*.json");
   dialogFile.addFilter( "Zipped JSON", "*.jsonz");
   dialogFile.addFilter( "XML", "*.xml");
   dialogFile.addFilter( "Zipped XML", "*.xmlz");
   dialogFile.addFilter( "HDF5", "*.hdf5");

   ::fwData::location::SingleFile::sptr result
       = ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );

   if (result)
   {
       _sDefaultPath = result->getPath();
       this->setFile( _sDefaultPath );
       dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath.parent_path()) );
   }
   else
   {
       this->clearLocations();
   }
}

//-----------------------------------------------------------------------------

} // namespace ioAtoms

