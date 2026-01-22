/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------
Licence:
  This file is part of phasicFlow code. It is a free software for simulating
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions.

  phasicFlow is distributed to help others in their research in the field of
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#ifndef __mixingIndex_hpp__
#define __mixingIndex_hpp__

#include "regionPoints.hpp"
#include "dictionary.hpp"
#include "fieldsDataBase.hpp"

namespace pFlow::postprocessData
{


class mixingIndex
{
private:

    const regionPoints&     regPoints_;

    /// Reference to the fields database containing field data.
    fieldsDataBase&         database_;

protected:

    inline
    fieldsDataBase& database()
    {
        return database_;
    }
    inline
    const fieldsDataBase& database()const
    {
        return database_;
    }

    inline 
    const regionPoints& regPoints()const
    {
        return regPoints_;
    } 

public:

    mixingIndex(
        const dictionary& dict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);

    virtual 
    ~mixingIndex()=default;

    create_vCtor
    (
        mixingIndex,
        dictionary,
        (
            const dictionary& dict,
            const regionPoints& regPoints,
            fieldsDataBase& fieldsDB
        ),
        (dict, regPoints, fieldsDB)
    );

    virtual
    bool execute() = 0;

    virtual 
    bool write(iOstream& os)const = 0;

    virtual 
    bool writeHeader(iOstream& os)const = 0;

    static 
    uniquePtr<mixingIndex> create(
        const dictionary& dict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);

};

}

#endif // __mixingIndex_hpp__
