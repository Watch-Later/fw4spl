/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMCONVERSION_UT_METAHELPERTEST_HPP__
#define __FWATOMCONVERSION_UT_METAHELPERTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwAtomConversion
{
namespace ut
{

class AtomHelperTest :  public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( AtomHelperTest );
    CPPUNIT_TEST( dataToMeta );
    CPPUNIT_TEST( metaToData );
    CPPUNIT_TEST( visitPatientDataTest );
    CPPUNIT_TEST( visitPatientMetaDataTest );
    CPPUNIT_TEST( visitCompositeDataTest );
    CPPUNIT_TEST( retreiveCampObjectVisitorTest );
    CPPUNIT_TEST_SUITE_END();

public:

    // interface
    void setUp();
    void tearDown();

    void dataToMeta();
    void metaToData();
    void visitPatientDataTest();
    void visitPatientMetaDataTest();
    void visitCompositeDataTest();
    void retreiveCampObjectVisitorTest();

    void visitCampObject( const camp::UserObject & campObj );

};


}  // namespace ut
}  // namespace fwAtomConversion



#endif // __FWATOMCONVERSION_UT_METAHELPERTEST_HPP__
