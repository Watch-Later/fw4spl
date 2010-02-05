#include "fwXML/XML/CompositeXMLTranslator.hpp"
#include <fwData/String.hpp>

#include "fwXML/XML/XMLParser.hpp"
#include "fwXML/XML/XMLTranslatorHelper.hpp"
#include "fwCore/base.hpp"

#include "fwXML/Serializer.hpp"

namespace fwXML
{

CompositeXMLTranslator::CompositeXMLTranslator() {};

CompositeXMLTranslator::~CompositeXMLTranslator() {};






xmlNodePtr CompositeXMLTranslator::getXMLFrom( ::boost::shared_ptr<fwTools::Object> obj )
{

    ::boost::shared_ptr< ::fwData::Composite >  cmp= boost::dynamic_pointer_cast< ::fwData::Composite >(obj);
    assert( cmp );

    // create master node with className+id
    xmlNodePtr node = XMLTranslatorHelper::MasterNode( obj );

    ::fwData::Composite::Container::iterator iter;
    for (iter = cmp->getRefMap().begin() ; iter != cmp->getRefMap().end() ; ++iter)
    {
        xmlNodePtr elementNode = xmlNewNode(NULL, BAD_CAST "element");
        xmlAddChild(node, elementNode);

        xmlNodePtr keyNode = xmlNewNode(NULL, BAD_CAST "key");
        xmlNodeAddContent( keyNode,  xmlStrdup( BAD_CAST iter->first.c_str() ) );
        xmlAddChild(elementNode, keyNode);

        xmlNodePtr valueNode = xmlNewNode(NULL, BAD_CAST "value");
        // xmlNodePtr trueValueNode = XMLTranslatorHelper::toXML( iter->second );
        xmlNodePtr trueValueNode = XMLTranslatorHelper::toXMLRecursive( iter->second );
        xmlAddChild(elementNode, valueNode);
        xmlAddChild(valueNode, trueValueNode);
    }

    return node;

}


void CompositeXMLTranslator::updateDataFromXML( ::boost::shared_ptr<fwTools::Object> toUpdate,  xmlNodePtr source)
{
    assert( toUpdate ); // object should exist
    //get its label
    ::fwData::Composite *cmp=dynamic_cast< ::fwData::Composite *>(toUpdate.get());

    xmlNodePtr elementNode = xmlNextElementSibling(source->children);
    while (elementNode )
    {
        std::string nodeName = (const char *) elementNode->name;
        if ( nodeName == "element" )
        {
            xmlNodePtr keyNode   = XMLParser::findChildNamed( elementNode, "key");
            xmlNodePtr valueNode = XMLParser::findChildNamed( elementNode, "value");
            assert( keyNode );
            assert( valueNode );
            OSLM_INFO( "CompositeXMLTranslator::updateDataFromXML"  << BAD_CAST xmlNodeGetContent(keyNode) );

            std::string key ( (char *)xmlNodeGetContent(keyNode)) ;


            //xmlNodePtr ConcretevalueNode = XMLParser::nextXMLElement( valueNode->children );
            xmlNodePtr ConcretevalueNode = xmlNextElementSibling( valueNode->children );
            assert( ConcretevalueNode  );


            ::fwTools::Object::sptr valueObj;
            valueObj = Serializer().ObjectsFromXml( ConcretevalueNode, true, true );

            assert( valueObj );
            assert( ::fwData::Object::dynamicCast( valueObj ));
            cmp->getRefMap()[key] = ::fwData::Object::dynamicCast( valueObj );
        }


        //elementNode = XMLParser::nextXMLElement(elementNode->next);
        elementNode = xmlNextElementSibling(elementNode->next);
        
    }

}


}
