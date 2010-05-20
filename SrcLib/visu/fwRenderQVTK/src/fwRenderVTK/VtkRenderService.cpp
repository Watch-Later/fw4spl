/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/function.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwComEd/CompositeEditor.hpp>
#include <fwComEd/CompositeMsg.hpp>
#include <fwTools/UUID.hpp>
#include <fwData/Color.hpp>
#include <fwRuntime/ConfigurationElementContainer.hpp>
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <fwComEd/CameraMsg.hpp>

#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkFrustumCoverageCuller.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLight.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>
#include <vtkInstantiator.h>
#include <vtkTransform.h>
#include <vtkCamera.h>

#include "fwRenderVTK/IVtkAdaptorService.hpp"
#include "fwRenderVTK/VtkRenderService.hpp"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

REGISTER_SERVICE( ::fwQtRender::IRender , ::fwRenderVTK::VtkRenderService , ::fwData::Composite ) ;

using namespace fwServices;


namespace fwRenderVTK
{

//-----------------------------------------------------------------------------

VtkRenderService::VtkRenderService() throw() :
     m_pendingRenderRequest(false)
{
    addNewHandledEvent( ::fwComEd::CompositeMsg::MODIFIED_FIELDS );
}

//-----------------------------------------------------------------------------

VtkRenderService::~VtkRenderService() throw() 
{
 
}

//-----------------------------------------------------------------------------

void VtkRenderService::configureRenderer( ConfigurationType conf )
{
    assert(conf->getName() == "renderer");

    std::string id = conf->getAttributeValue("id");
    std::string background = conf->getAttributeValue("background");

    if(m_renderers.count(id) == 0)
    {
        m_renderers[id] = vtkRenderer::New();

#ifdef USE_DEPTH_PEELING
        m_renderers[id]->SetUseDepthPeeling     ( 1  );
        m_renderers[id]->SetMaximumNumberOfPeels( 8  );
        m_renderers[id]->SetOcclusionRatio      ( 0. );
#elif USE_COVERAGE_CULLER
        vtkFrustumCoverageCuller *culler = vtkFrustumCoverageCuller::New();
        culler->SetSortingStyleToBackToFront();
        m_renderers[id]->AddCuller(culler);
        culler->Delete();
#endif

        if(conf->hasAttribute("layer") )
        {
            int layer = ::boost::lexical_cast< int >(conf->getAttributeValue("layer"));
            m_renderers[id]->SetLayer(layer);
        }
    }

    if ( !background.empty() )
    {
        if(background[0] == '#')
        {
            ::fwData::Color color;
            color.setRGBA(background);
            m_renderers[id]->SetBackground(color.getRefRGBA()[0], color.getRefRGBA()[1], color.getRefRGBA()[2]);
        }
        else
        {
            // compatibility with "old" color
            double color = ::boost::lexical_cast<double> (background);
            m_renderers[id]->SetBackground(color, color, color);
        }
    }
}

//-----------------------------------------------------------------------------

void VtkRenderService::configurePicker( ConfigurationType conf )
{
    assert(conf->getName() == "picker");

    std::string id = conf->getAttributeValue("id");
    std::string vtkclass = conf->getAttributeValue("vtkclass");

    if (vtkclass.empty())
    {
        vtkclass = "vtkCellPicker";
    }

    if(m_pickers.count(id) == 0)
    {
        m_pickers[id] =  vtkAbstractPropPicker::SafeDownCast(vtkInstantiator::CreateInstance(vtkclass.c_str()));
        OSLM_ASSERT("'" << vtkclass.c_str() << "' instantiation failled.",m_pickers[id]);
        m_pickers[id]->InitializePickList();
        m_pickers[id]->PickFromListOn();
        vtkPicker *picker = vtkPicker::SafeDownCast(m_pickers[id]);
        if (picker)
        {
            picker->SetTolerance(0);
        }
    }
}

//-----------------------------------------------------------------------------

void VtkRenderService::configureObject( ConfigurationType conf )
{
    assert(conf->getName() == "adaptor");
    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >() ;

    const std::string id            = conf->getAttributeValue("id");
    const std::string objectId      = conf->getAttributeValue("objectId");
    const std::string adaptor       = conf->getAttributeValue("class");
    const std::string uid           = conf->getAttributeValue("uid");
    const std::string compositeName = composite->getName();

    SLM_ASSERT( "'id' required attribute missing or empty"      , !id.empty() );
    SLM_ASSERT( "'objectId' required attribute missing or empty", !objectId.empty() );
    SLM_ASSERT( "'adaptor' required attribute missing or empty" , !adaptor.empty() );

    const unsigned int compositeObjectCount = composite->getRefMap().count(objectId);

    OSLM_TRACE_IF(objectId << " not found in composite. If it exist, associated Adaptor will be destroyed",
                  ! (compositeObjectCount == 1 || objectId == compositeName) );


    ::fwTools::Object::sptr object;
    if (compositeObjectCount)
    {
        object = ::fwTools::Object::dynamicCast(composite->getRefMap()[objectId]);
    }
    else if (objectId == compositeName)
    {
        object = ::fwTools::Object::dynamicCast(composite);
    }

    if ( m_sceneAdaptors.count(id) == 0 && object )
    {
        OSLM_TRACE ("Adding service : IVtkAdaptorService " << adaptor << " on "<< objectId );
        SceneAdaptor adaptee;
        adaptee.m_config = * (conf->begin());
        if (!uid.empty())
        {
            OSLM_TRACE("VtkRenderService::configureObject : uid = " << uid);
            adaptee.m_service = ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >( object , adaptor, uid);
        }
        else
        {
            adaptee.m_service = ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >( object , adaptor);
        }

        assert(adaptee.m_config->getName() == "config");
        assert(adaptee.getService());

        adaptee.getService()->setConfiguration(adaptee.m_config);
        adaptee.getService()->configure();
        adaptee.getService()->setRenderService(VtkRenderService::dynamicCast(this->shared_from_this()));
        adaptee.getService()->setName(id);

        if (this->isStarted())
        {
            adaptee.getService()->start();
        }

        m_sceneAdaptors[id] = adaptee;
    }
    else if(m_sceneAdaptors.count(id) == 1)
    {
        SceneAdaptor &adaptee = m_sceneAdaptors[id];
        SLM_ASSERT("Adaptor service expired !", adaptee.getService() );
        OSLM_ASSERT( ::fwTools::UUID::get(adaptee.getService()) <<  " is not started " ,adaptee.getService()->isStarted());
        if (object)
        {
            OSLM_TRACE ("Swapping IVtkAdaptorService " << adaptor << " on "<< objectId );
            if(adaptee.getService()->getObject() != object)
            {
                adaptee.getService()->swap(object);
            }
            else
            {
                OSLM_WARN(adaptor << "'s object already is '"
                        << adaptee.getService()->getObject()->getUUID()
                        << "', no need to swap");
            }
        }
        else
        {
            ::fwServices::erase(adaptee.getService());
            adaptee.m_service.reset();
            m_sceneAdaptors.erase(id);
        }
    }
    else
    {
            OSLM_TRACE ( "'"<< objectId << "' inexistant, passing by '" << adaptor << "'");
    }
}

//-----------------------------------------------------------------------------

void VtkRenderService::configureVtkObject( ConfigurationType conf )
{
    assert(conf->getName() == "vtkObject");

    std::string id = conf->getAttributeValue("id");
    std::string vtkClass = conf->getAttributeValue("class");

    assert( !id.empty() );
    assert( !vtkClass.empty() );

    if(m_vtkObjects.count(id) == 0)
    {
        m_vtkObjects[id] = vtkInstantiator::CreateInstance(vtkClass.c_str());
    }
}

//-----------------------------------------------------------------------------

void VtkRenderService::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    SLM_FATAL_IF( "Depreciated tag \"win\" in configuration", m_configuration->findConfigurationElement("win") );

    this->initClockRate();

    std::vector < ::fwRuntime::ConfigurationElement::sptr > vectConfig = m_configuration->find("scene");
    //assert(m_configuration->getName() == "scene");
    assert(!vectConfig.empty());
    m_sceneConfiguration = vectConfig.at(0);

    ::fwRuntime::ConfigurationElementContainer::Iterator iter;
    for (iter = m_sceneConfiguration->begin() ; iter != m_sceneConfiguration->end() ; ++iter)
    {
        if ((*iter)->getName() == "renderer")
        {
            this->configureRenderer(*iter);
        }
        else if ((*iter)->getName() == "picker")
        {
            this->configurePicker(*iter);
        }
        else if ((*iter)->getName() == "adaptor")
        {
            this->configureObject(*iter);
        }
        else if ((*iter)->getName() == "vtkObject")
        {
            this->configureVtkObject(*iter);
        }
        else
        {
            OSLM_ASSERT("Bad scene configurationType, unknown xml node : " << (*iter)->getName(), false);
        }
    }
}

//-----------------------------------------------------------------------------

void VtkRenderService::starting() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->initRender();
    this->startContext();

    //m_interactor->GetRenderWindow()->SetNumberOfLayers(m_renderers.size());
    m_widget->GetRenderWindow()->GetInteractor()->GetRenderWindow()->SetNumberOfLayers(m_renderers.size());
    for( RenderersMapType::iterator iter = m_renderers.begin(); iter != m_renderers.end(); ++iter )
    {
	vtkRenderer *renderer = vtkRenderer::New();
	renderer = (*iter).second;
	m_widget->GetRenderWindow()->AddRenderer(renderer);
	//m_renderWindow = widget->GetRenderWindow();
	//m_renderWindow->AddRenderer(renderer);
    }

    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >() ;


    SceneAdaptorsMapType::iterator adaptorIter ;

    for ( adaptorIter = m_sceneAdaptors.begin();
          adaptorIter != m_sceneAdaptors.end();
          ++adaptorIter)
    {
        adaptorIter->second.getService()->start();
        assert(adaptorIter->second.getService()->isStarted());
    }
}

//-----------------------------------------------------------------------------

void VtkRenderService::stopping() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >() ;

    SceneAdaptorsMapType::iterator adaptorIter ;

    for ( adaptorIter = m_sceneAdaptors.begin();
          adaptorIter != m_sceneAdaptors.end();
          ++adaptorIter)
    {
        adaptorIter->second.getService()->stop();
    }

    this->stopContext();
    this->stopRender();
    m_sceneAdaptors.clear();
}

//-----------------------------------------------------------------------------

void VtkRenderService::updating( ::fwServices::ObjectMsg::csptr message ) throw(::fwTools::Failed)
{


    SLM_TRACE_FUNC();

    ::fwComEd::CompositeMsg::csptr compositeMsg = ::fwComEd::CompositeMsg::dynamicConstCast(message);

    if(compositeMsg && compositeMsg->hasEvent( ::fwComEd::CompositeMsg::MODIFIED_FIELDS ) )
    {
        std::vector< std::string > objectIds = compositeMsg->getEventModifiedFields();
        std::vector< std::string >::iterator iter;

        assert ( m_sceneConfiguration );

        BOOST_FOREACH( std::string objectId, objectIds)
        {
            BOOST_FOREACH(
                    ConfigurationType cfg,
                    m_sceneConfiguration->find("adaptor","objectId",objectId))
            {
                this->configureObject(cfg);
            }
        }
    }
}

//-----------------------------------------------------------------------------

void VtkRenderService::updating() throw(fwTools::Failed)
{
    //m_interactor->Render();
    m_widget->GetRenderWindow()->GetInteractor()->Render();
}

//-----------------------------------------------------------------------------

void VtkRenderService::render()
{
 //  m_interactor->Render();
  m_widget->GetRenderWindow()->GetInteractor()->Render();
}


//-----------------------------------------------------------------------------

bool VtkRenderService::isShownOnScreen()
{
   return true;//m_interactor->IsShownOnScreen();
}

//-----------------------------------------------------------------------------

void VtkRenderService::startContext()
{
    // Create the window manager
    assert( m_container ) ;

    m_widget = new QVTKWidget(m_container);

    QVBoxLayout *layout = new QVBoxLayout();

    m_widget->resize(m_container->width(), m_container->height());
    m_widget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    layout->addWidget(m_widget);
    m_container->setLayout(layout);
    
    m_render = vtkRenderer::New();

    // IMPORTANT : use QVTKWidget own objects (vtkRenderWindow and vtkRenderWindowInteractor)
    m_widget->GetRenderWindow()->AddRenderer(m_render);   
}

//-----------------------------------------------------------------------------

void VtkRenderService::stopContext()
{
    SLM_TRACE_FUNC();

    for( RenderersMapType::iterator iter = m_renderers.begin(); iter != m_renderers.end(); ++iter )
    {
        vtkRenderer *renderer = iter->second;
        renderer->InteractiveOff();
	m_widget->GetRenderWindow()->RemoveRenderer(renderer);
	
	if(renderer != 0)
	{
	 renderer->Delete(); 
	}
        
    }
    m_renderers.clear();
    
    if(m_render != 0)
    {
      m_render->Delete();
      m_render = 0;
    }
   
    if(m_widget != 0)
    {
      //delete m_widget;
      m_widget->deleteLater();
      m_widget=0;
    }
  
    if(m_container != 0)
    {
      m_container->deleteLater();
      m_container=0;
    }
   
   
}

//-----------------------------------------------------------------------------

vtkRenderer * VtkRenderService::getRenderer(RendererIdType rendererId)
{
    OSLM_ASSERT("Renderer not found : '" << rendererId << "'" , m_renderers.count(rendererId) == 1);

    return m_renderers[rendererId];
}

//-----------------------------------------------------------------------------

vtkAbstractPropPicker * VtkRenderService::getPicker(PickerIdType pickerId)
{
    OSLM_ASSERT("Picker '" << pickerId << "' not found",
            pickerId.empty() ||  ( !pickerId.empty() && m_pickers.count(pickerId) == 1));

    if ( pickerId.empty() )
    {
        return NULL;
    }
    return m_pickers[pickerId];
}

//-----------------------------------------------------------------------------

vtkObject * VtkRenderService::getVtkObject(VtkObjectIdType objectId)
{
    OSLM_WARN_IF("vtkObject '" << objectId << "' not found",
            !(objectId.empty() ||  ( !objectId.empty() && m_vtkObjects.count(objectId) == 1)));

    if ( objectId.empty() )
    {
        return NULL;
    }
    return m_vtkObjects[objectId];
}

} //namespace fwRenderVTK



