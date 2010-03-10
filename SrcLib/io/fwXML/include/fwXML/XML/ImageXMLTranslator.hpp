/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef IMAGEXMLTRANSLATOR_HPP_
#define IMAGEXMLTRANSLATOR_HPP_

#include <boost/lexical_cast.hpp>

#include <fwCore/base.hpp>

#include <iostream>
#include <sstream>

#include <fwData/Image.hpp>


#include "fwXML/XML/XMLTranslator.hpp"


namespace fwXML
{

class FWXML_CLASS_API ImageXMLTranslator : public XMLTranslator
{
public:

    /// trivial constructor
    FWXML_API ImageXMLTranslator() ;

    /// trivial destructor
    FWXML_API virtual ~ImageXMLTranslator() ;

    /// get XML from current object
    FWXML_API xmlNodePtr getXMLFrom( ::boost::shared_ptr<fwTools::Object> obj );


    /// get Object from an XML node
    FWXML_API void updateDataFromXML( ::boost::shared_ptr<fwTools::Object> toUpdate,  xmlNodePtr source);

protected :

    /// manage extra serialization : read image from path extracted from filename attribut in <Buffer> xml node
    FWXML_API void manageLoadingBuffer( xmlNodePtr boostXMLBuffer /* FIXMEXPATH*/ , ::boost::shared_ptr< ::fwData::Image> );

    /// manage extra serialization : only inform <Buffer filename attribut />
    FWXML_API void manageSavingBuffer( xmlNodePtr boostXMLBuffer /* FIXMEXPATH*/ , ::boost::shared_ptr< ::fwData::Image> );
};

}

#endif /*IMAGEXMLTRANSLATOR_HPP_*/
