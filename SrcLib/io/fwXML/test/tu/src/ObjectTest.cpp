/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <time.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <fwTools/System.hpp>

#include <fwXML/Serializer.hpp>
#include <fwXML/policy/NeverSplitPolicy.hpp>
#include <fwXML/policy/UniquePathPolicy.hpp>

#include "ObjectTest.hpp"
#include "ObjectGenerator.hpp"
#include "ObjectComparator.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ObjectTest );

//------------------------------------------------------------------------------

void ObjectTest::setUp()
{
    // Set up context before running a test.
    srand(time(NULL));
}

//------------------------------------------------------------------------------

void ObjectTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

::fwData::Object::sptr ObjectTest::serializeOldVersion(std::string fileName, ::fwData::Object::sptr obj1)
{
    ::boost::filesystem::path testFile = ::fwTools::System::getTemporaryFolder() / "ObjectTest" / fileName;
    // save Array in fwXML
    ::boost::filesystem::create_directories( testFile.parent_path() );

    ::fwXML::Serializer serializer;
    ::boost::shared_ptr< ::fwXML::NeverSplitPolicy > spolicy( new ::fwXML::NeverSplitPolicy() );
    serializer.setSplitPolicy( spolicy );

#if BOOST_FILESYSTEM_VERSION > 2
    ::boost::shared_ptr< ::fwXML::UniquePathPolicy > pPathPolicy ( new ::fwXML::UniquePathPolicy( testFile.filename().string() ) );
#else
    ::boost::shared_ptr< ::fwXML::UniquePathPolicy > pPathPolicy ( new ::fwXML::UniquePathPolicy( testFile.leaf() ) );
#endif
    serializer.setPathPolicy( pPathPolicy );

    serializer.rootFolder() = testFile.parent_path().string();
    bool doSaveSchema = true;
    serializer.serialize(obj1, doSaveSchema);
    CPPUNIT_ASSERT(::boost::filesystem::exists(testFile));

    // load object
    ::fwData::Object::sptr obj2 = ::fwData::Object::dynamicCast(serializer.deSerialize(testFile, true));

    // check object
    CPPUNIT_ASSERT(obj2);

    ::boost::filesystem::remove_all( testFile.parent_path().string() );

    return obj2;
}

//------------------------------------------------------------------------------

void ObjectTest::testStructureTraitsDictionary()
{
    ::fwData::StructureTraitsDictionary::sptr structureDico1 = ObjectGenerator::createStructureTraitsDictionary();
    ::fwData::StructureTraitsDictionary::sptr structureDico2 = ::fwData::StructureTraitsDictionary::dynamicCast(ObjectTest::serializeOldVersion("StructureTraitsDictionary.xml", structureDico1));
    CPPUNIT_ASSERT(structureDico2);
    ObjectComparator::compareStructureTraitsDictionary(structureDico1, structureDico2);
}

//------------------------------------------------------------------------------

void ObjectTest::testStructureTraits()
{
    ::fwData::StructureTraits::sptr structure1 = ObjectGenerator::createStructureTraits();
    ::fwData::StructureTraits::sptr structure2 = ::fwData::StructureTraits::dynamicCast(ObjectTest::serializeOldVersion("StructureTraits.xml", structure1));
    CPPUNIT_ASSERT(structure2);
    ObjectComparator::compareStructureTraits(structure1, structure2);
}

//------------------------------------------------------------------------------

void ObjectTest::testROITraits()
{
    ::fwData::Composite::sptr roiCompo1 = ObjectGenerator::createROITraits();
    ::fwData::Composite::sptr roiCompo2 = ::fwData::Composite::dynamicCast(ObjectTest::serializeOldVersion("ROITraits.xml", roiCompo1));
    CPPUNIT_ASSERT(roiCompo2);
    CPPUNIT_ASSERT(roiCompo2->find("ROITraits") != roiCompo2->end());
    ::fwData::ROITraits::sptr roi1 = ::fwData::ROITraits::dynamicCast((*roiCompo1)["ROITraits"]);
    ::fwData::ROITraits::sptr roi2 = ::fwData::ROITraits::dynamicCast((*roiCompo2)["ROITraits"]);
    ObjectComparator::compareROITraits(roi1, roi2);
}

//------------------------------------------------------------------------------

void ObjectTest::testReconstructionTraits()
{
    ::fwData::Composite::sptr recCompo1 = ObjectGenerator::createReconstructionTraits();
    ::fwData::Composite::sptr recCompo2 = ::fwData::Composite::dynamicCast(ObjectTest::serializeOldVersion("ReconstructionTraits.xml", recCompo1));
    CPPUNIT_ASSERT(recCompo2);
    CPPUNIT_ASSERT(recCompo2->find("ReconstructionTraits") != recCompo2->end());
    ::fwData::ReconstructionTraits::sptr rec1 = ::fwData::ReconstructionTraits::dynamicCast((*recCompo1)["ReconstructionTraits"]);
    ::fwData::ReconstructionTraits::sptr rec2 = ::fwData::ReconstructionTraits::dynamicCast((*recCompo2)["ReconstructionTraits"]);
    ObjectComparator::compareReconstructionTraits(rec1, rec2);
}

//------------------------------------------------------------------------------