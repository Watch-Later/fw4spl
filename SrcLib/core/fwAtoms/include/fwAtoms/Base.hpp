/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMS_BASE_BASE_HPP__
#define __FWATOMS_BASE_BASE_HPP__


#include <map>
#include <string>

#include <fwCore/base.hpp>
#include <fwCamp/macros.hpp>
#include <fwCamp/camp/customtype.hpp>
#include <fwCamp/camp/MapProperty.hpp>
#include <fwCamp/camp/MapMapper.hpp>
#include <fwCamp/camp/MapValueMapper.hpp>
#include <fwCamp/camp/detail/MapPropertyImpl.hpp>
#include <fwCamp/camp/ExtendedClassVisitor.hpp>


#include "fwAtoms/config.hpp"

fwCampAutoDeclareMacro((fwAtoms)(Base), FWATOMS_API);

namespace fwAtoms
{

/**
 * @brief Base class for all metadata classes.
 */
class FWATOMS_CLASS_API Base : public ::fwCore::BaseObject
{
public:
    fwCoreClassDefinitionsWithFactoryMacro((Base), (()), new Base );
    fwCoreAllowSharedFromThis();

    /// return the sub class classname : an alias of this->getClassname
    std::string className() const { return this->getClassname(); };

    /**
     * @brief clone a data.
     * @return a clone of the current MetaData
     */
    FWATOMS_API virtual Base::sptr clone(){ return Base::New();};

    /**
     *@brief Test if the current base is a value or not (Value = {string;numeric;boolean})
     *@return the test result
     */
    FWATOMS_API virtual bool isValue() const {return false;};

    /**
     *@brief Test if the current base is a boolean
     *@return the test result
     */
    FWATOMS_API virtual bool isBoolean() const {return false;};

    /**
     *@brief Test if the current base is a Numeric (real or integer)
     *@return the test result
     */
    FWATOMS_API virtual bool isNumeric() const {return false;};

    /**
     *@brief Test if the current base is a string
     *@return the test result
     */
    FWATOMS_API virtual bool isString() const {return false;};

    /**
     *@brief Test if the current base is a sequence
     *@return the test result
     */
    FWATOMS_API virtual bool isSequence() const {return false;};

    /**
     *@brief Test if the current base is a mapping
     *@return the test result
     */
    FWATOMS_API virtual bool isMapping() const {return false;};

    /**
     *@brief Test if the current base is a blob (a object with a buffer)
     *@return the test result
     */
    FWATOMS_API virtual bool isBlob() const {return false;};

    /**
     *@brief Test if the current base is an object (representation of fwData)
     *@return the test result
     */
    FWATOMS_API virtual bool isObject() const {return false;};

    /**
     * @brief Return the string value of the current base.
     * @return the string representation. Default value = "Unknown"
     */
    FWATOMS_API virtual std::string getString() const {return "Unknown";};

};

}

#endif /* __FWATOMS_BASE_BASE_HPP__ */
