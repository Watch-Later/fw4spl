/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUI_ILAYOUTMANAGER_HPP_
#define _FWGUI_ILAYOUTMANAGER_HPP_

#include <fwCore/base.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "fwGui/IGuiContainer.hpp"
#include "fwGui/config.hpp"

namespace fwGui
{

/**
 * @brief   Defines the generic layout manager for IHM.
 * @class   ILayoutManager.
 * @author  IRCAD (Research and Development Team).
 * @date    2009-2010.
 *
 */
class FWGUI_CLASS_API ILayoutManager : public ::fwCore::BaseObject
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro( (ILayoutManager)(::fwCore::BaseObject) )

    /// Constructor. Do nothing.
    FWGUI_API ILayoutManager();

    /// Destructor. Do nothing.
    FWGUI_API virtual ~ILayoutManager();

    /**
     * @brief Configure the layout before creation.
     */
    FWGUI_API virtual void initialize( ::fwRuntime::ConfigurationElement::sptr configuration) = 0;

    /**
     * @brief Returns all sub containers managed by this layout.
     */
    FWGUI_API virtual std::vector< ::fwGui::fwContainer::sptr > getSubViews() = 0;

    /**
     * @brief Instantiate layout with parent container.
     */
    FWGUI_API virtual void createLayout( ::fwGui::fwContainer::sptr parent ) = 0;

    /**
     * @brief Destroy local layout with sub containers.
     * Be careful services using this sub containers must be stopped before!
     */
    FWGUI_API virtual void destroyLayout() = 0;

protected:

    /// All sub containers managed by this layout.
    std::vector< ::fwGui::fwContainer::sptr > m_subViews;

};

} // namespace fwGui

#endif /*_FWGUI_ILAYOUTMANAGER_HPP_*/


