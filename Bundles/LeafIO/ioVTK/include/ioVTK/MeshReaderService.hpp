/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOVTK_MESHREADERSERVICE_HPP_
#define _IOVTK_MESHREADERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <io/IReader.hpp>


#include "ioVTK/export.hpp"

namespace fwData
{
    class TriangularMesh;
}

namespace ioVTK
{

/**
 * @brief   Mesh reader service.
 * @class   MeshReaderService .
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * Service reading a VTK mesh using the vtkIO lib.
 *
 * Service registered details : \n
 * REGISTER_SERVICE( ::io::IReader , ::ioVTK::MeshReaderService , ::fwData::TriangularMesh )
 */
class IOVTK_CLASS_API MeshReaderService : public ::io::IReader
{

public :

    /**
     * @brief   Constructor
     *
     */
    IOVTK_API MeshReaderService() throw();

    /**
     * @brief   Destructor
     */
    IOVTK_API virtual ~MeshReaderService() throw();

    /**
    * @brief Configure the image path.
    *
    * This method is used to find
    * the file path  using a file selector.
    */
    IOVTK_API virtual void configureWithIHM();


protected:

    /**
    * @brief Configuring method.
    *
    * XML configuration sample:
    * @verbatim
    <service implementation="::ioVTK::MeshReaderService" type="::io::IReader">
        <filename id=".../meshTest.vtk"/>
    </service>
    @endverbatim
    * This method is used to configure the service.
    */
    IOVTK_API virtual void configuring() throw(::fwTools::Failed) ;

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
    * The image is read.
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
    * @brief Load a VTK mesh.
    * @param[in] _vtkFile ::boost::filesystem::path.
    * @param[out] _pTriangularMesh ::boost::shared_ptr< ::fwData::TriangularMesh >.
    * @return bool.
    *
    * This method is used to load a mesh using the file path.
    */
    void loadMesh( const ::boost::filesystem::path _vtkFile, ::boost::shared_ptr< ::fwData::TriangularMesh > _pTriangularMesh );

    /**
     * @brief Notification method.
     *
     * This method is used to notify
     * the mesh services.
     */
    void notificationOfUpdate();

    /**
    * @brief the m_bServiceIsConfigured value is \b true
    * if the mesh path is known.
    */
    bool m_bServiceIsConfigured;

    /**
    * @brief Mesh path.
    */
    ::boost::filesystem::path m_fsMeshPath;

};

} // namespace ioVTK

#endif //_IOVTK_MESHREADERSERVICE_HPP_
