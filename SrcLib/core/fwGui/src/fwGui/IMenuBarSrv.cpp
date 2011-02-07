/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <fwCore/base.hpp>
#include <fwTools/fwID.hpp>
#include <fwServices/helper.hpp>

#include "fwGui/IMenuBarSrv.hpp"

namespace fwGui
{

IMenuBarSrv::IMenuBarSrv() : m_hideMenus(false)
{}

//-----------------------------------------------------------------------------

IMenuBarSrv::~IMenuBarSrv()
{}

//-----------------------------------------------------------------------------

void IMenuBarSrv::initialize()
{

        m_registrar = ::fwGui::registrar::MenuBarRegistrar::NewSptr(this->getID());
        // find ViewRegistryManager configuration
        std::vector < ConfigurationType > vectRegistrar = m_configuration->find("registry");
        SLM_ASSERT("Registry section is mandatory.", !vectRegistrar.empty() );

        if(!vectRegistrar.empty())
        {
            m_registrarConfig = vectRegistrar.at(0);
            m_registrar->initialize(m_registrarConfig);
        }

        // find gui configuration
        std::vector < ConfigurationType > vectGui = m_configuration->find("gui");
        SLM_ASSERT("Gui section is mandatory.", !vectGui.empty() );

        if(!vectGui.empty())
        {
            // find LayoutManager configuration
            std::vector < ConfigurationType > vectLayoutMng = vectGui.at(0)->find("layout");
            SLM_ASSERT("layout section is mandatory.", !vectLayoutMng.empty() );
            if(!vectLayoutMng.empty())
            {
                m_layoutConfig = vectLayoutMng.at(0);
                this->initializeLayoutManager(m_layoutConfig);
            }
        }
}

//-----------------------------------------------------------------------------

void IMenuBarSrv::create()
{
    ::fwGui::container::fwMenuBar::sptr menuBar = m_registrar->getParent();
    SLM_ASSERT("Parent menuBar is unknown.", menuBar);
    m_layoutManager->createLayout(menuBar);

    m_registrar->manage(m_layoutManager->getMenus());
}

//-----------------------------------------------------------------------------

void IMenuBarSrv::destroy()
{
    m_registrar->unmanage();
    m_layoutManager->destroyLayout();
}

//-----------------------------------------------------------------------------

void IMenuBarSrv::menuServiceStopping(std::string menuSrvSID)
{
    ::fwGui::container::fwMenu::sptr menu = m_registrar->getFwMenu(menuSrvSID, m_layoutManager->getMenus());

    if (m_hideMenus)
    {
        m_layoutManager->menuIsVisible(menu, false);
    }
    else
    {
        m_layoutManager->menuIsEnabled(menu, false);
    }
}

//-----------------------------------------------------------------------------

void IMenuBarSrv::menuServiceStarting(std::string menuSrvSID)
{
    ::fwGui::container::fwMenu::sptr menu = m_registrar->getFwMenu(menuSrvSID, m_layoutManager->getMenus());

    if (m_hideMenus)
    {
        m_layoutManager->menuIsVisible(menu, true);
    }
    else
    {
        m_layoutManager->menuIsEnabled(menu, true);
    }
}

//-----------------------------------------------------------------------------

void IMenuBarSrv::initializeLayoutManager(ConfigurationType layoutConfig)
{
    OSLM_ASSERT("Bad configuration name "<<layoutConfig->getName()<< ", must be layout",
            layoutConfig->getName() == "layout");

    m_layoutManager = ::fwTools::ClassFactoryRegistry::create< ::fwGui::layoutManager::IMenuBarLayoutManager >( ::fwGui::layoutManager::IMenuBarLayoutManager::REGISTRY_KEY );
    OSLM_ASSERT("ClassFactoryRegistry failed for class "<< ::fwGui::layoutManager::IMenuBarLayoutManager::REGISTRY_KEY, m_layoutManager);

    m_layoutManager->initialize(layoutConfig);
}

//-----------------------------------------------------------------------------

}
