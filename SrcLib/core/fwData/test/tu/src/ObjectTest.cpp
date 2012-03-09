/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwData/Object.hpp>

#include "ObjectTest.hpp"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwData::ut::ObjectTest );


namespace fwData
{
namespace ut
{

//------------------------------------------------------------------------------

void ObjectTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void ObjectTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void ObjectTest::fieldTest()
{
    const std::string FIELD_ID1 = "FIELD_ID1";
    const std::string FIELD_ID2 = "FIELD_ID2";
    const std::string FIELD_ID3 = "FIELD_ID3";

    ::fwData::Object::sptr nullobj;
    ::fwData::Object::sptr obj = ::fwData::Object::New();
    ::fwData::Object::sptr fieldObj1 = ::fwData::Object::New();
    ::fwData::Object::sptr fieldObj2 = ::fwData::Object::New();

    CPPUNIT_ASSERT(obj->getFields_NEWAPI().empty());

    obj->setField_NEWAPI(FIELD_ID1, fieldObj1);
    CPPUNIT_ASSERT_EQUAL(obj->getFields_NEWAPI().size(), size_t(1));
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID1), fieldObj1);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID2), nullobj);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID3), nullobj);

    ::fwData::Object::FieldMapType localFields = obj->getFields_NEWAPI();
    ::fwData::Object::FieldMapType localFieldsBackup = obj->getFields_NEWAPI();
    localFields.insert( ::fwData::Object::FieldMapType::value_type(FIELD_ID2,fieldObj2));

    CPPUNIT_ASSERT_EQUAL(obj->getFields_NEWAPI().size(), size_t(1));
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID1), fieldObj1);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID2), nullobj);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID3), nullobj);

    obj->updateFields_NEWAPI(localFields);
    CPPUNIT_ASSERT_EQUAL(obj->getFields_NEWAPI().size(), size_t(2));
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID1), fieldObj1);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID2), fieldObj2);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID3), nullobj);

    ::fwData::Object::FieldNameVectorType refFieldNames;
    refFieldNames.push_back(FIELD_ID1);
    refFieldNames.push_back(FIELD_ID2);
    ::fwData::Object::FieldNameVectorType fieldNames = obj->getFieldNames_NEWAPI();
    CPPUNIT_ASSERT(refFieldNames == fieldNames);

    obj->setFields_NEWAPI(localFieldsBackup);
    CPPUNIT_ASSERT_EQUAL(obj->getFields_NEWAPI().size(), size_t(1));
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID1), fieldObj1);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID2), nullobj);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID3), nullobj);

    fieldNames = obj->getFieldNames_NEWAPI();
    refFieldNames.clear();
    refFieldNames.push_back(FIELD_ID1);
    CPPUNIT_ASSERT(refFieldNames == fieldNames);

    obj->setField_NEWAPI(FIELD_ID1, fieldObj2);
    CPPUNIT_ASSERT_EQUAL(obj->getFields_NEWAPI().size(), size_t(1));
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID1), fieldObj2);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID2), nullobj);
    CPPUNIT_ASSERT_EQUAL(obj->getField_NEWAPI(FIELD_ID3), nullobj);

}

//------------------------------------------------------------------------------

void ObjectTest::methode1()
{
    const std::string FIELDID1 = "fieldid1" ;

    ::fwData::Object::NewSptr  o1;
    ::fwTools::Field::sptr f1 = ::fwTools::Field::New();
    ::fwTools::Field::sptr f0 ; //shared_ptr NULL
    f1 = o1->setField(FIELDID1) ;

    // check
    CPPUNIT_ASSERT_EQUAL( o1->::fwTools::Object::getField(FIELDID1),   f1 );
}

//------------------------------------------------------------------------------

void ObjectTest::methode2()
{
    const std::string FIELDID1 = "fieldid1" ;

    ::fwData::Object::NewSptr  o1;
    ::fwTools::Field::sptr f1 = ::fwTools::Field::New();
    ::fwData::Object::NewSptr  o2;
    f1 = o1->setFieldSingleElement(FIELDID1,  o2) ;

    // check
    CPPUNIT_ASSERT_EQUAL( o1->::fwTools::Object::getField(FIELDID1),   f1 );
}

void ObjectTest::methode3()
{
    const std::string FIELDID1 = "fieldid1" ;
    const std::string FIELDID2 = "fieldid2" ;

    ::fwData::Object::NewSptr o1;
    ::fwTools::Field::sptr f1 = ::fwTools::Field::New();
    ::fwTools::Field::sptr f0 ;
    ::fwData::Object::NewSptr  o2;
    f1 = o1->addFieldElement(FIELDID1, o2) ;

    // check
    CPPUNIT_ASSERT_EQUAL( o1->::fwTools::Object::getField(FIELDID1),   f1 );
    CPPUNIT_ASSERT_EQUAL( o1->::fwTools::Object::getField(FIELDID2),   f0 );
    CPPUNIT_ASSERT_EQUAL( o1->::fwTools::Object::getField(FIELDID1),   f1 );
}

} //namespace ut
} //namespace fwData
