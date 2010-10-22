/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/lexical_cast.hpp>
#include <fwTools/ClassFactoryRegistry.hpp>
#include "fwXML/XML/NodeXMLTranslator.hpp"
#include <fwData/Node.hpp>
#include <fwData/Port.hpp>

#include "fwXML/visitor/SerializeXML.hpp"
#include "fwData/visitor/accept.hpp"

#include "fwXML/XML/XMLTranslatorHelper.hpp"

#include "fwXML/XML/XMLParser.hpp"

namespace fwXML
{

NodeXMLTranslator::NodeXMLTranslator() {};

NodeXMLTranslator::~NodeXMLTranslator() {};






xmlNodePtr NodeXMLTranslator::getXMLFrom( ::boost::shared_ptr<fwTools::Object> obj )
{
    ::boost::shared_ptr< ::fwData::Node> graphNode = boost::dynamic_pointer_cast< ::fwData::Node>(obj);
    assert(graphNode);

    // create master node with className+id
    xmlNodePtr node = XMLTranslatorHelper::MasterNode( obj );

    xmlNodePtr objectXMLNode;
    if (graphNode->getObject())
    {
        ::visitor::SerializeXML visitor;
        ::fwData::visitor::accept( graphNode->getObject(), &visitor );
        objectXMLNode = visitor.m_correspondance[graphNode->getObject()];
    }
    else
    {
        objectXMLNode = xmlNewNode(NULL,  BAD_CAST "nullObject");
    }
    xmlAddChild(node,objectXMLNode);

    // append input ports
    xmlNodePtr inputsList = XMLTranslatorHelper::homogeneousContainerToXml("inputs",
                                                                            graphNode->getInputPorts().begin(),
                                                                            graphNode->getInputPorts().end()    );
    xmlAddChild(node,inputsList);

    // append output ports
    xmlNodePtr outputsList = XMLTranslatorHelper::homogeneousContainerToXml("outputs",
                                                                            graphNode->getOutputPorts().begin(),
                                                                            graphNode->getOutputPorts().end()   );
    xmlAddChild(node,outputsList);




    return node;

}


void NodeXMLTranslator::updateDataFromXML( ::boost::shared_ptr<fwTools::Object> toUpdate,  xmlNodePtr source)
{
    xmlNodePtr nodeObject = XMLParser::nextXMLElement(source->children);
    OSLM_DEBUG("NodeXMLTranslator::updateDataFromXML first XML child" << (const char *) nodeObject->name );

    ::boost::shared_ptr< ::fwData::Node> graphNode = boost::dynamic_pointer_cast< ::fwData::Node>(toUpdate);
    assert(graphNode);

    xmlNodePtr nullObj = XMLParser::findChildNamed(source,"nullObject");
    if ( ! nullObj)
    {
        ::boost::shared_ptr< ::fwData::Object> subObject = boost::dynamic_pointer_cast< ::fwData::Object >( XMLTH::fromXML(nodeObject) );
        assert(subObject);
        graphNode->setObject( subObject );
    }

    xmlNodePtr inputs = XMLParser::findChildNamed(source,"inputs");
    XMLTH::containerFromXml(inputs, back_inserter( graphNode->getInputPorts() ) );

    xmlNodePtr outputs = XMLParser::findChildNamed(source,"outputs");
    XMLTH::containerFromXml(outputs, back_inserter( graphNode->getOutputPorts() ) );


}


}
