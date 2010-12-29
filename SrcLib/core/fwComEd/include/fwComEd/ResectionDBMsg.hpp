/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_RESECTIONDBMSG_HPP_
#define _FWCOMED_RESECTIONDBMSG_HPP_

#include <fwServices/ObjectMsg.hpp>

#include <fwData/Resection.hpp>
#include <fwData/ResectionDB.hpp>

#include "fwComEd/export.hpp"


namespace fwComEd
{

/**
 * @brief   Object event message specialized for resectionDB : store modification information
 * @class   ResectionDBMsg
 * @author  IRCAD (Research and Development Team).
 * @date    2009
 * @see     ::fwServices::ObjectMsg
 */
class FWCOMED_CLASS_API ResectionDBMsg : public ::fwServices::ObjectMsg
{
public:
    fwCoreClassDefinitionsWithFactoryMacro((ResectionDBMsg)(::fwServices::ObjectMsg), ( () ), new ResectionDBMsg );

    /**
     * @name Event identifier
     */
    /// @{
    /// @brief Event identifier used to inform for modification
    FWCOMED_API static std::string NEW_RESECTIONDB_SELECTED;
    FWCOMED_API static std::string RESECTIONDB_SELECTED;
    FWCOMED_API static std::string NEW_RESECTION_SELECTED;
    FWCOMED_API static std::string NEW_SAFE_PART_SELECTED;
    FWCOMED_API static std::string ADD_RESECTION;
    FWCOMED_API static std::string ADD_SAFE_PART;
    FWCOMED_API static std::string MODIFIED;
    FWCOMED_API static std::string RESECTIONDB_INVALIDATED;
    /// @}

    /**
     * @brief   Constructor : do nothing.
     */
    FWCOMED_API ResectionDBMsg() throw();

    /**
     * @brief   Destuctor : do nothing.
     */
    FWCOMED_API virtual ~ResectionDBMsg() throw();

    /**
     * @brief Return the selected Resection.
     * @pre   The message subject must be a PatientDB.
     * @pre   A Resection must be selected.
     */
    FWCOMED_API ::fwData::Resection::sptr getResectionSelected() const;

    /**
     * @brief Return the selected safe part (Resection).
     * @pre   The message subject must be a PatientDB.
     * @pre   A ResectionDB must be selected.
     */
    FWCOMED_API ::fwData::Resection::sptr getSafePartSelected() const;

    /**
     * @brief Return the selected operation (=ResectionDB).
     * @pre   The message subject must be a PatientDB.
     * @pre   A ResectionDB must be selected.
     */
    FWCOMED_API ::fwData::ResectionDB::sptr getResectionDBSelected() const;

    /**
     * @brief Return the name of the selected operation (=ResectionDB).
     * @pre   The message subject must be a PatientDB.
     * @pre   A ResectionDB must be selected.
     */
    FWCOMED_API std::string getResectionOpName() const;
};

} // namespace fwComEd

#endif //_FWCOMED_RESECTIONDBMSG_HPP_

