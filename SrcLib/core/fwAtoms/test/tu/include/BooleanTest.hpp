/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMS_UT_BOOLEANTEST_HPP__
#define __FWATOMS_UT_BOOLEANTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwAtoms
{
namespace ut
{

class BooleanTest :  public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( BooleanTest );
    CPPUNIT_TEST( conversion );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void conversion();
};


}  // namespace ut
}  // namespace fwAtoms



#endif // __FWATOMS_UT_BOOLEANTEST_HPP__

