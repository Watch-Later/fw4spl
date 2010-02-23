/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/shared_ptr.hpp>
#include <fwServices/IService.hpp>



namespace guiQt
{

/**
 * @namespace ::gui::aspect
 * @brief This namespace contains the classes which define graphical user interfaces (menu, toolbar...)
 */
namespace aspect
{

/**
 * @brief   Defines the service interface managing GUI aspect, including menu actions and views
 * @class   IAspect.
 * @author  IRCAD (Research and Development Team).

 * @date    2009.
 *
 * @todo    ACH: Generic actions should not be defined in the config aspect.
 * @todo    ACH: Using shared pointer in order to reference action and menu services is dubious and should be dangerous.
 */
class  IQtAspect : public ::fwServices::IService
{

public :
    fwCoreNonInstanciableClassDefinitionsMacro ( (IQtAspect)( ::fwServices::IService::Baseclass) ) ;

    /**
     * @brief   Constructor. Do nothing (Just initializes parameters).
     *
     * By default, the IAspect::m_name is set to "NO NAME". IAspect::m_minSizeHeight and IAspect::m_minSizeWidth are set to -1.
     */
     IQtAspect() throw() ;

    /// Destructor. Do nothing.
     virtual ~IQtAspect() throw() ;

    /**
     * @brief This method returns the application title, IAspect::m_name.
     *
     *@return Application title.
     */
    std::string getName() ;

    /**
     * @brief This method returns the application minimum height and width size, IAspect::m_minSizeHeight and IAspect::m_minSizeWidth.
     *
     * @return wxSize Application minimum size.
     */
   // GUI_API wxSize getMinSize() ;

protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */
    /**
     * @brief   This method initializes class member parameters from configuration elements.
     *
     * @todo ACH: It should be useful to verify the config is well formed.
     */
     virtual void configuring() throw( ::fwTools::Failed ) ;

    /// This method first registers the aspect gui manager and it starts the menus contained in the IAspect::m_menus.
    virtual void starting() throw( ::fwTools::Failed );

    /// This method first unregisters the aspect gui manager and it stops the menus contained in the IAspect::m_menus.
    virtual void stopping() throw( ::fwTools::Failed );

    /**
     * @brief   Normally, perform some computations according to object (this service is attached to) attribute values and its internal state. Do nothing here.
     *
     * @todo ACH: This method should not be implemented, but imposed to its children classes.
     */
     virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw( ::fwTools::Failed ) = 0;

    /**
     * @brief   Normally, perform some computations according to object (this service is attached to) attribute values and its internal state. Do nothing here.
     *
     * @todo ACH: This method should not be implemented, but imposed to its children classes.
     */
     virtual void updating() throw( ::fwTools::Failed ) = 0 ;

     /// Gives informations about the class. Do nothing.
     virtual void info(std::ostream &_sstream ) ;

    ///@}

    /// Service vector of ::gui::view::IView type.  List of the view declared in the aspect config.
  //  std::vector< ::gui::view::IQtView::sptr >         m_views ;

    /// Service vector of ::gui::aspect::IMenu UUID.  List of the menus declared in the aspect config.
    //std::vector< std::string >                      m_menusUUID ;

    /// Application name.
    std::string                                     m_name ;

    /// Application minimum height.
    int                                             m_minSizeHeight;

    /// Application minimum width.
    int                                             m_minSizeWidth;
    
    // Menu
    //::guiQt::aspect::IQtMenu m_menu;

  
};



} // namespace aspect
} // namespace gui



