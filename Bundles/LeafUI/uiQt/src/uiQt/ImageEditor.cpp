/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QMainWindow>
#include <QDockWidget>


#include <fwComEd/LocationMsg.hpp>
#include <fwData/location/SingleFile.hpp>

#include <fwServices/Base.hpp>

#include "uiQt/ImageEditor.hpp"

#include <QLayout>
#include <QVBoxLayout>


#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTextSource.h>
#include <vtkVectorText.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>



REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiQt::ImageEditor , ::fwData::location::SingleFile) ;


namespace uiQt
{

ImageEditor::ImageEditor() throw()
{
    SLM_TRACE_FUNC();

    addNewHandledEvent( ::fwComEd::LocationMsg::LOCATION_IS_MODIFIED );
}

//-----------------------------------------------------------------------------

ImageEditor::~ImageEditor() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ImageEditor::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ImageEditor::starting() throw(fwTools::Failed)
{
   QWidget* container;
   QWidget* view;

   QLayout *layout = new QVBoxLayout();
   QLayout *PicLayout = new QVBoxLayout();
   
   ::guiQt::editor::IEditor::starting();
   OSLM_TRACE("Servie UUID : "<<this->getUUID()<<"\n");

   //container = m_globalUIDToQtContainer[this->getUUID()];
   container = m_globalUIDToQtContainer.find(this->getUUID())->second;
   view = new QWidget(m_container);
  // view->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

//     view->setMinimumHeight(container->height());
//     view->setMinimumWidth(container->width());
//       view->resize(container->width(), container->height());

   //layout->setAlignment(Qt::AlignCenter);
   
   imageLabel = new QLabel();
   PicLayout->addWidget(imageLabel);
   view->setLayout(PicLayout);
 
   layout->addWidget(view);
   m_container->setLayout(layout);

   // important le setParent
 //   imageLabel->setParent(view)



//  imageLabel->setMinimumHeight(view->height());
//    imageLabel->setMinimumWidth(view->width());
  // imageLabel->setMargin(20);





/*
 QVTKWidget *widget = new QVTKWidget(container);
 widget->resize(500, 500);

 vtkTextSource *text = vtkTextSource::New();
  text->SetText("Hello World!");
  text->BackingOff();
 vtkVectorText *vectorText = vtkVectorText::New();
  vectorText->SetText("QVTKWidget");
 vtkPolyDataMapper *textMapper = vtkPolyDataMapper::New();
  textMapper->SetInput(text->GetOutput());
 vtkPolyDataMapper *vectorTextMapper = vtkPolyDataMapper::New();
  vectorTextMapper->SetInput(vectorText->GetOutput());
 vtkActor *textActor = vtkActor::New();
  textActor->SetMapper(textMapper);
 vtkActor *vectorTextActor = vtkActor::New();
  vectorTextActor->SetMapper(vectorTextMapper);
 vtkRenderer *renderer = vtkRenderer::New();
  renderer->SetBackground(0.4,0.6,0.8);
  renderer->AddActor(textActor);
  renderer->AddActor(vectorTextActor);
 vtkRenderWindow *renderWindow = vtkRenderWindow::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetStereoTypeToDresden();

 widget->SetRenderWindow(renderWindow);
container->show();
 */

}

//-----------------------------------------------------------------------------

void ImageEditor::stopping() throw(fwTools::Failed)
{
  ::guiQt::editor::IEditor::stopping();

}

//-----------------------------------------------------------------------------

void ImageEditor::updating() throw(fwTools::Failed)
{
  // Recuperation de l'objet
    ::fwData::location::SingleFile::sptr file = this->getObject < ::fwData::location::SingleFile >();
    OSLM_INFO("FileMsg : " << file->getPath());

    image = new QImage(file->getPath().string().c_str());

     if (image->isNull())
     {
    //QMessageBox::information(this, QObject::tr("Image Viewer"),QObject::tr("Cannot load %1.").arg(file->getPath().string()));
        return;
      }
      imageLabel->setPixmap(QPixmap::fromImage(*image));
      imageLabel->setScaledContents(true);
      imageLabel->setAlignment(Qt::AlignHCenter);
    //  imageLabel->show();

}

//-----------------------------------------------------------------------------

void ImageEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{


  ::fwComEd::LocationMsg::csptr fileMsg =  ::fwComEd::LocationMsg::dynamicConstCast( _msg );

    if ( fileMsg && fileMsg->hasEvent( ::fwComEd::LocationMsg::LOCATION_IS_MODIFIED ) )
    {
        this->updating();
    }

}

//-----------------------------------------------------------------------------

}
