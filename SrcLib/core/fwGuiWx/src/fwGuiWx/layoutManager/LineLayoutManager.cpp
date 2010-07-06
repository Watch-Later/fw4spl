/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <wx/panel.h>
#include <wx/sizer.h>

#include <boost/foreach.hpp>

#include <fwCore/base.hpp>

#include <fwWX/convert.hpp>

#include "fwGuiWx/layoutManager/LineLayoutManager.hpp"

namespace fwGui
{

//-----------------------------------------------------------------------------

LineLayoutManager::LineLayoutManager()
{}

//-----------------------------------------------------------------------------

LineLayoutManager::~LineLayoutManager()
{}

//-----------------------------------------------------------------------------

void LineLayoutManager::createLayout( ::fwGui::fwContainer::sptr parent )
{
    SLM_TRACE_FUNC();
    m_parentContainer = ::fwGuiWx::container::WxContainer::dynamicCast(parent);
    SLM_ASSERT("dynamicCast fwContainer to WxContainer failed", m_parentContainer);

    wxWindow* wxContainer = m_parentContainer->getWxContainer();
    Orientation orientation = this->getOrientation();
    int wxOrientation = ( (orientation==VERTICAL) ? wxVERTICAL : wxHORIZONTAL);
    wxBoxSizer* boxSizer = new wxBoxSizer( wxOrientation );
    wxContainer->SetSizer( boxSizer );
    wxContainer->Layout();

    std::list< ViewInfo> views = this->getViewsInfo();
    BOOST_FOREACH ( ViewInfo viewInfo, views)
    {
        wxPanel * viewPanel = new wxPanel(  wxContainer, wxNewId() , wxDefaultPosition, wxSize( viewInfo.m_minSize.first, viewInfo.m_minSize.second ), wxNO_BORDER | wxTAB_TRAVERSAL );
        ::fwGuiWx::container::WxContainer::NewSptr subContainer;
        subContainer->setWxContainer(viewPanel);
        m_subViews.push_back(subContainer);
#ifndef __MACOSX__
        if(viewInfo.m_caption.first)
        {
            wxStaticBoxSizer* sizer = new wxStaticBoxSizer( wxOrientation, wxContainer, ::fwWX::std2wx(viewInfo.m_caption.second));
            sizer->Add( viewPanel, 1, wxALL|wxEXPAND, viewInfo.m_border);
            boxSizer->Add(sizer, viewInfo.m_proportion, wxALL|wxEXPAND);
        }
        else
        {
            boxSizer->Add( viewPanel, viewInfo.m_proportion, wxALL|wxEXPAND, viewInfo.m_border);
        }
#else
        boxSizer->Add( viewPanel, viewInfo.m_proportion, wxALL|wxEXPAND, viewInfo.m_border);
#endif
    }
}

//-----------------------------------------------------------------------------

void LineLayoutManager::destroyLayout()
{
    this->destroySubViews();
    m_parentContainer->clean();
}

//-----------------------------------------------------------------------------

} // namespace fwGui



