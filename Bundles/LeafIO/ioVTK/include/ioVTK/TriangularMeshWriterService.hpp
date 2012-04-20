/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOVTK_TRIANGULARMESHWRITERSERVICE_HPP_
#define _IOVTK_TRIANGULARMESHWRITERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <io/IWriter.hpp>


#include "ioVTK/export.hpp"

namespace fwData
{
    class TringularMesh;
}


namespace ioVTK
{

/**
 * @brief   VTK mesh writer service.
 * @class   TriangularMeshWriterService.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * Service writing a VTK Mesh using the vtkIO lib.
 *
 * Service registered details : \n
 * REGISTER_SERVICE( ::io::IWriter , ::ioVTK::TriangularMeshWriterService , ::fwData::TriangularMesh )
 */
class IOVTK_CLASS_API TriangularMeshWriterService : public ::io::IWriter
{

public :
    fwCoreServiceClassDefinitionsMacro ( (TriangularMeshWriterService)( ::io::IWriter) ) ;

    /**
    * @brief Configure the mesh path.
    *
    * This method is used to find
    * the file path  using a file selector.
    */
    IOVTK_API virtual void configureWithIHM();


protected:

    /**
     * @brief   Constructor
     *
     */
    IOVTK_API TriangularMeshWriterService() throw();

    /**
     * @brief   Destructor
     */
    IOVTK_API virtual ~TriangularMeshWriterService() throw();

    IOVTK_API virtual ::io::IOPathType getIOPathType() const;


    /**
    * @brief Starting method.
    *
    * This method is used to initialize the service.
    */
    IOVTK_API virtual void starting() throw(::fwTools::Failed);

    /**
    * @brief Stopping method.
    *
    * The stopping method is empty for this service.
    */
    IOVTK_API virtual void stopping() throw(::fwTools::Failed);

    /**
    * @brief Updating method.
    *
    * This method is used to update the service.
    * The mesh is read.
    */
    IOVTK_API void updating() throw(::fwTools::Failed);

    /**
     * @brief React on modifications : default does nothing.
     *
     * @note This method is automaticaly called by update( msg ) method from base service ( ::fwServices::IService ).
     *
     * @param[in] _msg information message for modification
     */
    IOVTK_API void updating( ::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed){};

    /**
    * @brief Info method.
    *
    * This method is used to give
    * informations about the service.
    */
    IOVTK_API void info(std::ostream &_sstream ) ;


private :

    /**
    * @brief Save a VTK image.
    * @param[in] _vtkFile \b const ::boost::filesystem::path.
    * @param[out] _pTriangularMesh ::boost::shared_ptr< ::fwData::TriangularMesh >.
    *
    * This method is used to save an image using the file path.
    */
    void saveMesh( const ::boost::filesystem::path _vtkFile, ::boost::shared_ptr< ::fwData::TriangularMesh > _pTriangularMesh );

    /**
    * @brief the m_bServiceIsConfigured value is \b true
    * if the image path is known.
    */
    bool m_bServiceIsConfigured;

    /**
    * @brief Mesh path .
    */
    ::boost::filesystem::path m_fsMeshPath;

};

} // namespace ioVTK

#endif //_IOVTK_TRIANGULARMESHWRITERSERVICE_HPP_