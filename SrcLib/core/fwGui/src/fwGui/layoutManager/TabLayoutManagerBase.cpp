/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

#include <fwCore/base.hpp>

#include "fwGui/layoutManager/TabLayoutManagerBase.hpp"

namespace fwGui
{

namespace layoutManager
{
//-----------------------------------------------------------------------------

const TabLayoutManagerBase::RegistryKeyType TabLayoutManagerBase::REGISTRY_KEY =  "::fwGui::CardinalLayoutManager";

//-----------------------------------------------------------------------------

TabLayoutManagerBase::TabLayoutManagerBase()
{}

//-----------------------------------------------------------------------------

TabLayoutManagerBase::~TabLayoutManagerBase()
{}

//-----------------------------------------------------------------------------

void TabLayoutManagerBase::initialize( ConfigurationType configuration)
{
    OSLM_ASSERT("Bad configuration name "<<configuration->getName()<< ", must be layout",
            configuration->getName() == "layout");

    std::vector < ConfigurationType > vectViews = configuration->find("view");
    SLM_TRACE_IF("No view define.", vectViews.empty() );
    m_views.clear();
    BOOST_FOREACH (ConfigurationType view, vectViews)
    {
        ViewInfo vi;
        if( view->hasAttribute("name") )
        {
            vi.m_name = view->getExistingAttributeValue("name") ;
        }
        if( view->hasAttribute("selected") )
        {
            std::string isSelected = view->getExistingAttributeValue("selected");
            OSLM_ASSERT("Sorry, value "<<isSelected<<" is not correct for attribute isSelected.",
                    isSelected == "yes" || isSelected == "no");
            vi.m_isSelect = (isSelected == "yes");
        }
        m_views.push_back(vi);
    }
}

//-----------------------------------------------------------------------------

} // namespace layoutManager
} // namespace fwGui



