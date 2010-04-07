/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <iostream>
#include <boost/cstdint.hpp>

#include <zlib.h>

#include <fwTools/ClassRegistrar.hpp>

#include <fwData/Object.hpp>
#include <fwData/Image.hpp>
#include <fwData/location/SingleFile.hpp>

#include "fwDataIO/reader/GzBufferImageReader.hpp"


REGISTER_BINDING_BYCLASSNAME( ::fwDataIO::reader::IObjectReader , ::fwDataIO::reader::GzBufferImageReader, ::fwDataIO::reader::GzBufferImageReader );


namespace fwDataIO
{

namespace reader
{

//------------------------------------------------------------------------------

GzBufferImageReader::GzBufferImageReader() : ::fwData::location::enableSingleFile< IObjectReader >(this)
{}

//------------------------------------------------------------------------------

GzBufferImageReader::~GzBufferImageReader()
{}

//------------------------------------------------------------------------------

void GzBufferImageReader::read()
{
	assert( ::boost::dynamic_pointer_cast< ::fwData::location::SingleFile >(m_location) );
	::boost::filesystem::path file = ::boost::dynamic_pointer_cast< ::fwData::location::SingleFile >(m_location)->getPath();

	assert( file.empty() ==  false );

	::boost::shared_ptr< ::fwData::Image > image = getConcreteObject();
	::boost::uint32_t imageSizeInBytes = ::fwData::imageSizeInBytes(*image);

	char *ptr =new char[imageSizeInBytes];

	gzFile rawFile = gzopen(file.native_file_string().c_str(), "rb");

	assert(rawFile);
	if ( rawFile == 0 )
	{
		std::string str = "Unable to open ";
		str+= file.native_file_string();
		throw std::ios_base::failure(str);
	}

	unsigned int uncompressedbytesreaded=gzread(rawFile,ptr,imageSizeInBytes);

	//assert(uncompressedbytesreaded==m_nbBytes);
	if ( uncompressedbytesreaded!=imageSizeInBytes )
	{
		std::string str = "Unable to read ";
		str+= file.native_file_string();
		throw std::ios_base::failure(str);
	}

	image->setBuffer(ptr);

	gzclose(rawFile);
}



std::string  &GzBufferImageReader::extension()
{
	static std::string ext(".raw.gz");
	return ext;
}


} // namespace reader

} // namespace fwDataIO
