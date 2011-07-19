/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2011.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <iostream>
#include <fstream>

#include <fwTools/System.hpp>
#include <fwData/TriangularMesh.hpp>

#include <fwDataIO/reader/TriangularMeshReader.hpp>
#include <fwDataIO/writer/TriangularMeshWriter.hpp>
#include <fwDataIO/reader/IObjectReader.hpp>

#include "TrianTest.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TrianTest );

const unsigned int NBPTS   = 100;
const unsigned int NBCELLS = 100;

//------------------------------------------------------------------------------

void TrianTest::setUp()
{
    // Set up context before running a test.
    m_tmpTrianPath1 = ::fwTools::System::getTemporaryFolder() / "TrianTest1.trian";
    m_tmpTrianPath2 = ::fwTools::System::getTemporaryFolder() / "TrianTest2.trian";

    this->generateTrian(m_tmpTrianPath1);

    CPPUNIT_ASSERT(::boost::filesystem::exists(m_tmpTrianPath1));
}

//------------------------------------------------------------------------------

void TrianTest::tearDown()
{
    // Clean up after the test run.
    bool suppr = false;
    if( ::boost::filesystem::exists(m_tmpTrianPath1))
    {
        suppr = ::boost::filesystem::remove(m_tmpTrianPath1);
        CPPUNIT_ASSERT(suppr);
    }
    if( ::boost::filesystem::exists(m_tmpTrianPath2))
    {
        bool suppr = ::boost::filesystem::remove(m_tmpTrianPath2);
        CPPUNIT_ASSERT(suppr);
    }
}

//------------------------------------------------------------------------------

void TrianTest::test_1()
{
    ::fwData::TriangularMesh::NewSptr trianMesh;
    ::fwDataIO::reader::TriangularMeshReader::NewSptr reader;

    reader->setObject(trianMesh);
    reader->setFile(m_tmpTrianPath1);
    reader->read();

    CPPUNIT_ASSERT(trianMesh->getNumPoints() == NBPTS);
    CPPUNIT_ASSERT(trianMesh->getNumCells() == NBCELLS);
}

//------------------------------------------------------------------------------

void TrianTest::test_2()
{
    ::fwData::TriangularMesh::NewSptr trianMesh1;
    ::fwData::TriangularMesh::NewSptr trianMesh2;

    ::fwDataIO::reader::TriangularMeshReader::NewSptr reader;
    ::fwDataIO::writer::TriangularMeshWriter::NewSptr writer;

    reader->setObject(trianMesh1);
    reader->setFile(m_tmpTrianPath1);
    reader->read();

    writer->setObject(trianMesh1);
    writer->setFile(m_tmpTrianPath2);
    writer->write();

    reader->setObject(trianMesh2);
    reader->setFile(m_tmpTrianPath2);
    reader->read();

    CPPUNIT_ASSERT(trianMesh1->getNumPoints() == NBPTS);
    CPPUNIT_ASSERT(trianMesh1->getNumCells()  == NBCELLS);
    CPPUNIT_ASSERT(trianMesh1->getNumCells()  == trianMesh2->getNumCells());
    CPPUNIT_ASSERT(trianMesh1->getNumPoints() == trianMesh2->getNumPoints());
    CPPUNIT_ASSERT(trianMesh1->points() == trianMesh2->points());
    CPPUNIT_ASSERT(trianMesh1->cells()  == trianMesh2->cells());
}

//------------------------------------------------------------------------------

void TrianTest::generateTrian(::boost::filesystem::path trianFile)
{
    std::fstream file;
    file.open(trianFile.string().c_str(), std::fstream::out);
    CPPUNIT_ASSERT(file.is_open());

    unsigned int i;
    file<<NBPTS<<std::endl;
    for( i=0 ; i<NBPTS ; ++i )
    {
        file<<i<<" "<<i+1<<" "<<i+2<<std::endl;
    }

    file<<NBCELLS<<std::endl;
    for( i=0 ; i<NBCELLS ; ++i )
    {
        file<<i%NBPTS<<" "<<(i+1)%NBPTS<<" "<<(i+2)%NBPTS<<" "<<-1<<" "<<-1<<" "<<-1<<std::endl;
    }

    file.close();
}

//------------------------------------------------------------------------------

