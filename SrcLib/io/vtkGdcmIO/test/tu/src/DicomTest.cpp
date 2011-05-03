/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <fwData/PatientDB.hpp>

#include <vtkGdcmIO/DicomPatientDBWriter.hpp>
#include <vtkGdcmIO/DicomPatientDBReader.hpp>

#include "DicomTest.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DicomTest );

//------------------------------------------------------------------------------

void DicomTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void DicomTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void DicomTest::methode1()
{
    setlocale(LC_NUMERIC,"C");

    // create patientDB
    ::fwData::PatientDB::sptr pPatientDB = createPatientDB();

    // save patientDB in fwXML
    const ::boost::filesystem::path PATH = "dicomTest";

    ::boost::filesystem::create_directories( PATH );

    ::vtkGdcmIO::DicomPatientDBWriter writer;
    writer.setObject(pPatientDB);
    writer.setFolder(PATH);
    writer.write();

    // load patientDB
    ::fwData::PatientDB::NewSptr pPatientDB2;
    ::vtkGdcmIO::DicomPatientDBReader reader;
    reader.setObject(pPatientDB2);
    reader.setFolder(PATH);
    reader.read();

    ::boost::filesystem::remove_all( PATH.string() );

    // check patientDB
    CPPUNIT_ASSERT(pPatientDB2);
    CPPUNIT_ASSERT_EQUAL(pPatientDB->getPatientSize(), pPatientDB2->getPatientSize());

    ::fwData::Patient::sptr pPatient = *pPatientDB->getPatients().first;
    ::fwData::Patient::sptr pPatient2 = *pPatientDB2->getPatients().first;
    CPPUNIT_ASSERT_EQUAL(pPatient->getName(), pPatient2->getName());
    CPPUNIT_ASSERT_EQUAL(pPatient->getFirstname(), pPatient2->getFirstname());
    CPPUNIT_ASSERT_EQUAL(pPatient->getBirthdate(), pPatient2->getBirthdate());
    CPPUNIT_ASSERT_EQUAL(pPatient->getIDDicom(), pPatient2->getIDDicom());
    CPPUNIT_ASSERT_EQUAL(pPatient->getIsMale(), pPatient2->getIsMale());
    CPPUNIT_ASSERT_EQUAL(pPatient->getStudySize(), pPatient2->getStudySize());

    ::fwData::Study::sptr pStudy = *pPatient->getStudies().first;
    ::fwData::Study::sptr pStudy2 = *pPatient2->getStudies().first;

    std::string study2Hospital = pStudy2->getHospital();
    ::boost::algorithm::trim(study2Hospital);
    CPPUNIT_ASSERT_EQUAL(pStudy->getHospital(), study2Hospital);
    CPPUNIT_ASSERT_EQUAL(pStudy->getModality(), pStudy2->getModality());
//  CPPUNIT_ASSERT_EQUAL(pStudy->getAcquisitionZone(), pStudy2->getAcquisitionZone());
    CPPUNIT_ASSERT_EQUAL(pStudy->getAcquisitionSize(), pStudy2->getAcquisitionSize());

    ::fwData::Acquisition::sptr pAcq = *pStudy->getAcquisitions().first;
    ::fwData::Acquisition::sptr pAcq2 = *pStudy2->getAcquisitions().first;

    ::fwData::Image::sptr image = pAcq->getImage();
    ::fwData::Image::sptr image2 = pAcq2->getImage();
    CPPUNIT_ASSERT_EQUAL(image->getDimension(), image2->getDimension());
    CPPUNIT_ASSERT_EQUAL(image->getOrigin().back(), image2->getOrigin().back());
    CPPUNIT_ASSERT_EQUAL(image->getWindowCenter(), image2->getWindowCenter());
    CPPUNIT_ASSERT_EQUAL(image->getWindowWidth(), image2->getWindowWidth());
    CPPUNIT_ASSERT_EQUAL(image->getSize()[0], image2->getSize()[0]);
    CPPUNIT_ASSERT_EQUAL(image->getSize()[1], image2->getSize()[1]);
    CPPUNIT_ASSERT_EQUAL(image->getSize()[2], image2->getSize()[2]);
    CPPUNIT_ASSERT_EQUAL(image->getSpacing()[0], image2->getSpacing()[0]);
    CPPUNIT_ASSERT_EQUAL(image->getSpacing()[1], image2->getSpacing()[1]);
    CPPUNIT_ASSERT_EQUAL(image->getSpacing()[2], image2->getSpacing()[2]);
}

//------------------------------------------------------------------------------

::fwData::PatientDB::sptr DicomTest::createPatientDB()
{
    // patient informations
    const std::string PATIENT_NAME          = "NomPatient1" ;
    const std::string PATIENT_FIRSTNAME     = "PrenomPatient1" ;
    const std::string PATIENT_IDDICOM       = "IDDICOM" ;
    const std::string PATIENT_BIRTHDATE     = "2009-May-27 00:00:00";
    const bool PATIENT_SEX                  = true;
    const ::boost::int32_t  PATIENT_DBID    = 457;

    // studies informations
    const std::string STUDY1_HOSPITAL       = "Study hospital" ;
    const std::string STUDY1_MODALITY       = "CT" ;
    const std::string STUDY1_ZONE           = "Study zone" ;

    // image informations
    const ::boost::uint8_t IMG1_DIMENSION   = 3 ;
    std::vector<double> IMG1_VSPACING (3,2);
    IMG1_VSPACING[0] = 0.1234;
    IMG1_VSPACING[1] = 0.246;
    IMG1_VSPACING[2] = 0.48;

    std::vector<double> IMG1_ORIGIN (3,0);

    ::boost::int32_t IMG1_SIZEX = 10 ;
    ::boost::int32_t IMG1_SIZEY = 20;
    ::boost::int32_t IMG1_SIZEZ = 30 ;
    std::vector< ::boost::int32_t > IMG1_VSIZE ;
    IMG1_VSIZE.push_back(IMG1_SIZEX);
    IMG1_VSIZE.push_back(IMG1_SIZEY);
    IMG1_VSIZE.push_back(IMG1_SIZEZ);
    int size = IMG1_SIZEX*IMG1_SIZEY*IMG1_SIZEZ;

    const double IMG1_WINDOWCENTER = 86 ;
    const double IMG1_WINDOWWIDTH = 345 ;
    const double IMG1_RESCALEINTERCEPT = 1 ;

    ::fwTools::DynamicType IMG1_PIXELTYPE = ::fwTools::makeDynamicType<signed short>();
    signed short * buffer1 = new signed short[size];
    for (int i=0 ; i< size ; i++)
    {
        buffer1[i] = 100;
    }

    //create patientDB
    ::fwData::PatientDB::NewSptr pPatientDB;
    ::fwData::Patient::NewSptr pPatient;
    pPatientDB->addPatient(pPatient);

    pPatient->setCRefName( PATIENT_NAME );
    pPatient->setCRefFirstname( PATIENT_FIRSTNAME );
    pPatient->setCRefIDDicom(   PATIENT_IDDICOM );
    pPatient->setCRefBirthdate( ::boost::posix_time::time_from_string( PATIENT_BIRTHDATE ));
    pPatient->setIsMale( PATIENT_SEX );
    pPatient->setDbID( PATIENT_DBID );

    ::fwData::Study::NewSptr pStudy1;
    pPatient->addStudy(pStudy1);

    pStudy1->setCRefHospital( STUDY1_HOSPITAL );
    pStudy1->setCRefModality( STUDY1_MODALITY );
    pStudy1->setCRefAcquisitionZone( STUDY1_ZONE );

    ::fwData::Acquisition::NewSptr pAcq1;
    pStudy1->addAcquisition(pAcq1);


    ::fwData::Image::NewSptr pImage1;
    pAcq1->setImage(pImage1);

    pImage1->setDimension( IMG1_DIMENSION );
    pImage1->setCRefSize( IMG1_VSIZE );
    pImage1->setCRefOrigin( IMG1_ORIGIN );
    pImage1->setCRefSpacing( IMG1_VSPACING );

    pImage1->setPixelType(IMG1_PIXELTYPE);
    pImage1->setBuffer( static_cast<void *>(buffer1) );

    pImage1->setWindowCenter(IMG1_WINDOWCENTER);
    pImage1->setWindowWidth(IMG1_WINDOWWIDTH    );
    pImage1->setRescaleIntercept(IMG1_RESCALEINTERCEPT);


    return pPatientDB;
}

//------------------------------------------------------------------------------